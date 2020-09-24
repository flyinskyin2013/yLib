/*
 * @Author: Sky
 * @Date: 2020-09-08 10:50:08
 * @LastEditors: Sky
 * @LastEditTime: 2020-09-24 16:40:00
 * @Description: 
 */
#include "network/ytcpserver.h"

#ifdef __cplusplus
extern "C"{
#endif //__cplusplus

#include <sys/epoll.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#ifdef __cplusplus
}
#endif //__cplusplus


#include <iostream>
#include <memory>
#include <utility>


#include "ylib.hpp"


using namespace yLib;



yTcpServer::yTcpServer(/* args */)
:epoll_thread_ptr(nullptr)
{
    socket_fd = -1;
    is_bind_success = false;
    epoll_thread_is_continue = false;

    if ( 0 > (socket_fd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) ){

        std::cout<<"yTcpServer create socket failed. errno is "<< errno <<std::endl;
    }

    max_client_num = UINT16_MAX;

    //init bitmap
    uint64_t _tmp_count = UINT16_MAX / 32 + 1;
    used_client_id_bitmap.reserve(_tmp_count);
    for (int _i = 0; _i < _tmp_count; _i++){

        used_client_id_bitmap.push_back(0);
    }
}

yTcpServer::yTcpServer(uint64_t max_client_num_, int socket_flags_, bool is_block_)
:epoll_thread_ptr(nullptr)
{

    socket_fd = -1;
    is_bind_success = false;
    epoll_thread_is_continue = false;

    if (is_block_){

        if ( 0 > (socket_fd = ::socket(AF_INET, SOCK_STREAM, socket_flags_)) ){

            std::cout<<"yTcpServer create socket failed. errno is "<< errno <<std::endl;
        }
    }
    else{
        
        if ( 0 > (socket_fd = ::socket(AF_INET, SOCK_STREAM |SOCK_NONBLOCK, socket_flags_)) ){

            std::cout<<"yTcpServer create socket failed. errno is "<< errno <<std::endl;
        }
    }

    max_client_num = max_client_num_;

    //init bitmap
    uint64_t _tmp_count = max_client_num_ / 32 + 1;
    used_client_id_bitmap.reserve(_tmp_count);
    for (int _i = 0; _i < _tmp_count; _i++){

        used_client_id_bitmap.push_back(0);
    }
}

yTcpServer::~yTcpServer()
{
    if (socket_fd > 0)//close socket_fd
        close(socket_fd);
    
    epoll_thread_is_continue = false;
    epoll_thread_ptr = nullptr;//auto free , thread is detach
    socket_fd = -1;
    is_bind_success = false;
    max_client_num = 0;
}

/**
 * @description: 
 * @param {type} 
 * @return {type} 
 */
int64_t yTcpServer::get_available_client_id(void){

    
    uint64_t _now_count = 0;
    auto _is_unused_id = [this](uint64_t id_)->bool{

        uint32_t _bitmap_idx = id_/32;
        uint8_t _bitmap_offset = id_%32;
        
        uint32_t _get_old_val = this->used_client_id_bitmap[_bitmap_idx];
        if ( 0 == ((_get_old_val>>_bitmap_offset)&0x00000001) ){

            _get_old_val |= (0x1<<_bitmap_offset);
            this->used_client_id_bitmap[_bitmap_idx] = _get_old_val;
            return true;
        }
        else
        {
            return false;
        }
        
    };
    for (_now_count = 0; _now_count < max_client_num; _now_count ++){

        if (_is_unused_id(_now_count)){

            break;
        }
    }

    return _now_count;
}

/**
 * @description: 
 * @param {type} 
 * @return {type} 
 */
bool yTcpServer::set_available_client_id(uint64_t client_id_){

    uint32_t _bitmap_idx = client_id_/32;
    uint8_t _bitmap_offset = client_id_%32;
    
    uint32_t _get_old_val = this->used_client_id_bitmap[_bitmap_idx];
    _get_old_val |= (~(0x1<<_bitmap_offset));
    this->used_client_id_bitmap[_bitmap_idx] = _get_old_val;

    return true;
}

/**
 * @description: Check socket(), bind(), listen(), accept() and so on.
 * @param {type} 
 * @return {type} 
*/
inline bool yTcpServer::socket_is_ready(void)
{
    if (!socket_is_valid())//check if socket_fd < 0
        return false;

    if (!is_bind_success)//check is bind succ
        return false;

    if (nullptr == epoll_thread_ptr)//check if epoll thread is ready
        return false;

    return true;
}

/**
 * @description: bind ip and port
 * @param {type} 
 * @return {type} 
 */
int8_t yTcpServer::bind(const std::string & ip_, int32_t port_)
{

    if (!socket_is_valid()){

        std::cout<<"yTcpServer: socket is invalid. please check."<<std::endl;
        return -1;
    }

	::memset(static_cast<void *>(&server_socket_addr), 0, sizeof(server_socket_addr));

    //	socket_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (0 == ::inet_aton(ip_.c_str(), &server_socket_addr.sin_addr)){//invalid ip

        std::cout<<"yTcpServer: Invalid ip_. please check."<<std::endl;
        return -1;
    }
	server_socket_addr.sin_family = AF_INET;
	server_socket_addr.sin_port = htons(port_);

    if (0 > ::bind(socket_fd, reinterpret_cast<const struct sockaddr *>(&server_socket_addr), sizeof(server_socket_addr)) ){

        is_bind_success = false;
        std::cout<<"yTcpServer socket bind failed. errno is "<< errno <<std::endl;
        return -1;
    }

    is_bind_success = true;
    return 0;

}


/**
 * @description: 
 * @param {type} 
 * @return {type} 
 */
void yTcpServer::epoll_thread_context(OnClientConnectCB con_cb_, OnClientDisconnectCB discon_cb_){

    int _epoll_fd = 0;
    /*
        epoll_create() creates a new epoll(7) instance.  Since Linux 2.6.8, the
       size argument is ignored, but must be  greater  than  zero;  see  NOTES
       below.

    */
    if (0 > (_epoll_fd = ::epoll_create(max_client_num))) {
        
        std::cout<<"yTcpServer: server call epoll_create() failed, error num is" << errno<<std::endl;
        return ;
    }

    struct epoll_event _tmp_reg_event;

    struct epoll_event _ret_events_array[max_client_num + 1];

    _tmp_reg_event.data.fd = socket_fd;
    //EPOLLIN for read, EPOLLET for fast deal
    _tmp_reg_event.events = EPOLLIN | EPOLLET;

    //reg server_sockfd to epoll
    if (0 > ::epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, socket_fd, &_tmp_reg_event)) {

        std::cout<<"yTcpServer: server call epoll_ctl() failed, error num is" << errno<<std::endl;
        return ;
    }

    int64_t _epoll_wait_ret = 0;

    epoll_thread_is_continue = true;
    while (epoll_thread_is_continue)
    {
        /*
            When successful, epoll_wait() returns the number  of  file  descriptors
        ready for the requested I/O, or zero if no file descriptor became ready
        during the requested  timeout  milliseconds.   When  an  error  occurs,
        epoll_wait() returns -1 and errno is set appropriately.
        */
        _epoll_wait_ret = ::epoll_wait(_epoll_fd, _ret_events_array, max_client_num + 1, 500);
        if (0 > _epoll_wait_ret){//

            std::cout<<"yTcpServer: server call epoll_wait() failed, error num is" << errno<<std::endl;
            break;
        }
        else if (0 == _epoll_wait_ret){//time out

            continue;
        }

        for (int i = 0; i < _epoll_wait_ret; i++) {
            
            if (_ret_events_array[i].data.fd == socket_fd) {
                
                // for new socket
                while (1) {

                    struct sockaddr_in _client_addr;
                    socklen_t _client_addr_len = sizeof(_client_addr);
                    int _new_client_fd;
                    if (0 > (_new_client_fd = ::accept(socket_fd, (sockaddr *)&_client_addr, &_client_addr_len))) {

                        if (EWOULDBLOCK == errno || EAGAIN == errno) {
                            
                            // The socket is marked nonblocking and no connections are  present to  be  accepted.
                            break;
                        }
                        
                        std::cout<<"yTcpServer: server call accept() failed, error num is" << errno<<std::endl;
                        break;
                    }

                    std::string _accept_client_ip = ::inet_ntoa(_client_addr.sin_addr);

                    /* server don't control client-fd
                    //set nonblocking
                    if (0 > ::fcntl(_new_client_fd, F_SETFL, ::fcntl(_new_client_fd, F_GETFL, 0) | O_NONBLOCK)) {
                        
                        //1:非阻塞 0:阻塞
                        std::cout<<"yTcpServer: server call accept() failed, error num is" << errno<<std::endl;
                        continue;
                    }

                    _tmp_reg_event.data.fd = _new_client_fd;
                    _tmp_reg_event.events = EPOLLIN | EPOLLOUT | EPOLLRDHUP | EPOLLERR | EPOLLET;


                    if (0 > ::epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, _new_client_fd, &_tmp_reg_event)) {
                        
                        std::cout<<"yTcpServer: server call accept() failed, error num is" << errno<<std::endl;
                        continue;
                    }
                    */

                    client_info_map.insert(std::make_pair(_new_client_fd, _client_addr));

                    //call OnClientConnectCB
                    con_cb_(_new_client_fd, _accept_client_ip, ::ntohs(_client_addr.sin_port));

                }//while

                continue; // to epoll_wait
            } //ret_events[i].data.fd != server_sockfd
            else if (_ret_events_array[i].events & EPOLLIN) {//do nothing

                yLib::yLog::I("client fd = %d, data in " ,_ret_events_array[i].data.fd);
                // while (1) {
                    
                //     int _read_num = 0;
                //     //we don't set read_buffer's val to 0x0.
                //     if (0 > (_read_num = ::read(_ret_events_array[i].data.fd, read_buffer, read_buffer_max_size))) {

                //         if (EAGAIN == errno || EINTR == errno || EWOULDBLOCK == errno) {
                            
                //             //The file descriptor fd refers to a socket and  has  been  marked nonblocking    (O_NONBLOCK)
                //         } 
                //         else {

                //             yLib::yLog::E("call read() failed, closing this socket %d, error num is :%d", _ret_events_array[i].data.fd, errno);

                //             //close this socket
                //             close(_ret_events_array[i].data.fd);

                //             //remove this socket from epoll
                //             _tmp_reg_event.data.fd = _ret_events_array[i].data.fd;
                //             _tmp_reg_event.events = EPOLLIN | EPOLLOUT | EPOLLRDHUP | EPOLLERR | EPOLLET;

                //             /*
                //                    EPOLL_CTL_DEL
                //                     Remove (deregister) the target file descriptor fd from the epoll
                //                     instance  referred  to by epfd.  The event is ignored and can be
                //                     NULL (but see BUGS below).
                //             */
                //             int _ret_val = 0;
                //             if (0 > (_ret_val = epoll_ctl(_epoll_fd, EPOLL_CTL_DEL, _ret_events_array[i].data.fd, &_tmp_reg_event))) {

                //                 yLib::yLog::E("call epoll_ctl() failed, error num is :%d", errno);
                //                 perror("call epoll_ctl()");
                //             }
                //         }
                //         break;
                //     } 
                //     else if (0 == _read_num) {
                        
                //         //zero indicates end of
                //         yLib::yLog::E("read() return 0, this socket normal disconnect,closing this socket %d", _ret_events_array[i].data.fd);

                //         //remove this socket from epoll
                //         _tmp_reg_event.data.fd = _ret_events_array[i].data.fd;
                //         _tmp_reg_event.events = EPOLLIN | EPOLLOUT | EPOLLRDHUP | EPOLLERR | EPOLLET;

                //         int _ret_val = 0;
                //         if (0 > (_ret_val = epoll_ctl(_epoll_fd, EPOLL_CTL_DEL, _ret_events_array[i].data.fd, &_tmp_reg_event))) {

                //             yLib::yLog::E("call epoll_ctl() failed, error num is :%d", errno);
                //             perror("call epoll_ctl()");
                //         }

                //         //close this socket
                //         close(_ret_events_array[i].data.fd);

                //         //delete client from yTcpServerSocketMap  and call OnClientDisconnectCB 2020/09/14
                //         yTcpServerSocketMap::iterator _tmp_iter = client_info_map.find(_ret_events_array[i].data.fd);

                //         std::string _close_client_ip = ::inet_ntoa(_tmp_iter->second.sin_addr);
                //         discon_cb_(_ret_events_array[i].data.fd, _close_client_ip, ::ntohs(_tmp_iter->second.sin_port));
                //         client_info_map.erase(_ret_events_array[i].data.fd);

                //         break;
                //     } 
                //     else {//real user datas

                //         break;
                //     }
                // }//while
            } 
            else if (_ret_events_array[i].events & EPOLLOUT) {

                yLib::yLog::I("client fd = %d, data out", _ret_events_array[i].data.fd);
                continue;

            } else if (_ret_events_array[i].events & EPOLLHUP || _ret_events_array[i].events & EPOLLERR) {

                yLib::yLog::I("client fd = %d, event EPOLLHUP || EPOLLERR", _ret_events_array[i].data.fd);

                //close this socket
                close(_ret_events_array[i].data.fd);
                
                //remove this socket from epoll
                _tmp_reg_event.data.fd = _ret_events_array[i].data.fd;
                _tmp_reg_event.events = EPOLLIN | EPOLLOUT | EPOLLRDHUP | EPOLLERR | EPOLLET;

                int _ret_val = 0;
                if (0 > (_ret_val = epoll_ctl(_epoll_fd, EPOLL_CTL_DEL, _ret_events_array[i].data.fd, &_tmp_reg_event))) {

                    yLib::yLog::E("call epoll_ctl() failed, error num is :%d", errno);
                    perror("call epoll_ctl()");
                }

                //delete client from yTcpServerSocketMap  and call OnClientDisconnectCB 2020/09/14
                yTcpServerSocketMap::iterator _tmp_iter = client_info_map.find(_ret_events_array[i].data.fd);

                std::string _close_client_ip = ::inet_ntoa(_tmp_iter->second.sin_addr);
                discon_cb_(_ret_events_array[i].data.fd, _close_client_ip, ::ntohs(_tmp_iter->second.sin_port));
                client_info_map.erase(_ret_events_array[i].data.fd);

                continue;
            } 
            else {
                //sleep for release cpu

                usleep(20 * 1000);
            }
        }
    }
    
}

/**
 * @description: 
 * @param {type} 
 * @return {type} 
 */
int8_t yTcpServer::start_epoll_thread(OnClientConnectCB con_cb_, OnClientDisconnectCB discon_cb_, uint64_t max_listen_num)
{
    if (!socket_is_valid()){

        std::cout<<"yTcpServer: socket is invalid. please check."<<std::endl;
        return -1;
    }

    if ( 0 > ::listen(socket_fd, max_listen_num) ){

        std::cout<<"yTcpServer listen failed. errno is "<< errno <<std::endl;
        return -1;
    }

    //create epoll thread
    //epoll_thread_ptr = std::make_unique<std::thread>(&yTcpServer::epoll_thread_context, this, con_cb_, discon_cb_);
    epoll_thread_ptr = std::unique_ptr<std::thread>( new std::thread(&yTcpServer::epoll_thread_context, this, con_cb_, discon_cb_));
    epoll_thread_ptr->detach();

    return 0;
}

/**
 * @description: 
 * @param {type} 
 * @return {type} 
 * =-1: a error occurred.
 * =-2: no data to read.The file descriptor fd refers to a socket and has been marked nonblocking (O_NONBLOCK)
 * = 0: client maybe close.
 * > 0: read n bytes.
 */
int64_t yTcpServer::read(void *buffer_, size_t count_, uint64_t client_fd_){

    ssize_t _read_len = 0;
    if (0 > (_read_len = ::read(client_fd_, buffer_, count_))){

        if (EAGAIN == errno || EINTR == errno || EWOULDBLOCK == errno) {
            
            //The file descriptor fd refers to a socket and  has  been  marked nonblocking    (O_NONBLOCK)
            return -2;
        } 
        else {

            yLib::yLog::E("call read() failed, closing this socket %d, error num is :%d", client_fd_, errno);
            return -1;
        }
    }

    return _read_len;
}

/**
 * @description: 
 * @param {type} 
 * @return {type}
 *  =-1: a error occurred.
 *  >=0: write n bytes.
 */
int64_t yTcpServer::write(const void *buffer_, size_t count_, uint64_t client_fd_)
{
    ssize_t _write_len = 0;
    if (0 > (_write_len = ::write(client_fd_, buffer_, count_))){

        if (EAGAIN == errno || EINTR == errno || EWOULDBLOCK == errno) {
            
            //The file descriptor fd refers to a socket and  has  been  marked nonblocking    (O_NONBLOCK)
            return 0;
        } 
        else {

            yLib::yLog::E("call write() failed, closing this socket %d, error num is :%d", client_fd_, errno);
            return -1;
        }
        return _write_len;
    }

    return _write_len;
}