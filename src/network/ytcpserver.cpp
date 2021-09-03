/*
 * @Author: Sky
 * @Date: 2020-09-08 10:50:08
 * @LastEditors: Sky
 * @LastEditTime: 2021-08-31 13:57:25
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

YLIB_IMPLEMENT_CLASSINFO_CONTENT(yTcpServer)

yTcpServer::yTcpServer(/* args */) noexcept
{

    clean_to_default();

    create_socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    epoll_thread_is_continue = false;
}

yTcpServer::yTcpServer(int domain, int type, int protocol) noexcept
{

    clean_to_default();

    create_socket(domain, type, protocol);

    epoll_thread_is_continue = false;
}

yTcpServer::~yTcpServer()
{

    epoll_thread_is_continue = false;

    if (nullptr != epoll_thread_ptr)
        if (epoll_thread_ptr->joinable())
            epoll_thread_ptr->join();

    epoll_thread_ptr = nullptr;//auto free , thread is detach

    clean_to_default();
}

/**
 * @description: Check socket(), bind(), listen(), accept() and so on.
 * @param {type} 
 * @return {type} 
*/
inline bool yTcpServer::socket_is_ready(void) const noexcept
{

    return (is_sockfd_valid && is_bind_success && is_connect_success&&is_listen_success);
}


/**
 * @description: 
 * @param {type} 
 * @return {type} 
 */
void yTcpServer::epoll_thread_context(OnClientConnectCB con_cb, OnClientDisconnectCB discon_cb) noexcept{

    int _epoll_fd = 0;
    /*
        epoll_create() creates a new epoll(7) instance.  Since Linux 2.6.8, the
       size argument is ignored, but must be  greater  than  zero;  see  NOTES
       below.

    */
    if (0 > (_epoll_fd = ::epoll_create(1))) {
        
        std::cout<<"yTcpServer: server call epoll_create() failed, error num is" << errno<<std::endl;
        return ;
    }

    struct epoll_event _tmp_reg_event;

    struct epoll_event _ret_events_array[max_listen_num + 1];

    _tmp_reg_event.data.fd = socket_fd;
    //EPOLLIN for read, EPOLLET for fast deal, set server in Level  Triggered mode.
    _tmp_reg_event.events = EPOLLERR | EPOLLHUP | EPOLLRDHUP | EPOLLIN;

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
        _epoll_wait_ret = ::epoll_wait(_epoll_fd, _ret_events_array, max_listen_num + 1, 500);
        if (0 > _epoll_wait_ret){//

            std::cout<<"yTcpServer: server call epoll_wait() failed, error num is" << errno<<std::endl;
            break;
        }
        else if (0 == _epoll_wait_ret){//time out

            continue;
        }

        for (int i = 0; i < _epoll_wait_ret; i++) {

            /*
                EPOLLERR
                        Error condition happened  on  the  associated  file  descriptor.
                        This event is also reported for the write end of a pipe when the
                        read end has been closed.  epoll_wait(2) will always report  for
                        this event; it is not necessary to set it in events.

                EPOLLHUP
                        Hang   up   happened   on   the   associated   file  descriptor.
                        epoll_wait(2) will always wait for this event; it is not  neces‐
                        sary to set it in events.

                        Note that when reading from a channel such as a pipe or a stream
                        socket, this event merely indicates that the peer closed its end
                        of the channel.  Subsequent reads from the channel will return 0
                        (end of file) only after all outstanding data in the channel has
                        been consumed.

            */
            if (_ret_events_array[i].events & EPOLLHUP || _ret_events_array[i].events & EPOLLERR) {

                yLib::yLog::I("server detect EPOLLHUP/EPOLLERR.");

                //close server socket
                close(_ret_events_array[i].data.fd);
                
                //remove this socket from epoll
                _tmp_reg_event.data.fd = _ret_events_array[i].data.fd;
                _tmp_reg_event.events = EPOLLERR | EPOLLHUP | EPOLLRDHUP | EPOLLIN;

                int _ret_val = 0;
                if (0 > (_ret_val = epoll_ctl(_epoll_fd, EPOLL_CTL_DEL, _ret_events_array[i].data.fd, &_tmp_reg_event))) {

                    yLib::yLog::E("call epoll_ctl() failed, error num is :%d", errno);
                    perror("call epoll_ctl()");
                }

            } 
            else if (_ret_events_array[i].events & EPOLLRDHUP) {
/*
       EPOLLRDHUP (since Linux 2.6.17)
              Stream socket peer closed connection, or shut down writing  half
              of connection.  (This flag is especially useful for writing sim‐
              ple code to detect peer shutdown when using Edge Triggered moni‐
              toring.)

*/
                yLib::yLog::I("server detect EPOLLRDHUP");

                //close server socket
                close(_ret_events_array[i].data.fd);
                
                //remove this socket from epoll
                _tmp_reg_event.data.fd = _ret_events_array[i].data.fd;
                _tmp_reg_event.events = EPOLLERR | EPOLLHUP | EPOLLRDHUP | EPOLLIN;

                int _ret_val = 0;
                if (0 > (_ret_val = epoll_ctl(_epoll_fd, EPOLL_CTL_DEL, _ret_events_array[i].data.fd, &_tmp_reg_event))) {

                    yLib::yLog::E("call epoll_ctl() failed, error num is :%d", errno);
                    perror("call epoll_ctl()");
                }

            } 
            else if (_ret_events_array[i].events & EPOLLIN) {//do nothing

                // server fd data in

                //new fd
                if (_ret_events_array[i].data.fd == socket_fd) {
                                
                    // for new socket
                    struct sockaddr_in _client_addr;
                    socklen_t _client_addr_len = sizeof(_client_addr);
                    int _new_client_fd;
                    if (0 > (_new_client_fd = ::accept(socket_fd, (sockaddr *)&_client_addr, &_client_addr_len))) {

                        if (EWOULDBLOCK == errno || EAGAIN == errno) {
                            
                            // The socket is marked nonblocking and no connections are  present to  be  accepted.
   
                        }
                        
                        std::cout<<"yTcpServer: server call accept() failed, error num is" << errno<<std::endl;
                    }
                    else{

                        std::string _accept_client_ip = ::inet_ntoa(_client_addr.sin_addr);

                        //server don't control client-fd
                        con_cb(_new_client_fd, _accept_client_ip, ntohs(_client_addr.sin_port));
                    }


                } 

                //ret_events[i].data.fd != server_sockfd other client fd.
                // DO Nothing
            } 
            else if (_ret_events_array[i].events & EPOLLOUT) {
                
                // server fd data out

            }
            else {//other event
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
int8_t yTcpServer::start_epoll_thread(OnClientConnectCB con_cb, OnClientDisconnectCB discon_cb, uint64_t max_listen_num) noexcept
{
    if (epoll_thread_is_continue)
        return 1;

    epoll_thread_is_continue = true;

    if (!is_sockfd_valid){

        std::cout<<"yTcpServer: socket is invalid. please check."<<std::endl;
        return -1;
    }

    if (!is_bind_success){

        std::cout<<"yTcpServer: bind is invalid. please check."<<std::endl;
        return -1;
    }

    if ( 0 > ::listen(socket_fd, max_listen_num) ){

        std::cout<<"yTcpServer listen failed. errno is "<< errno <<std::endl;
        is_listen_success = false;
        return -1;
    }

    this->max_listen_num = max_listen_num;

    is_listen_success = true;

    //create epoll thread
    //epoll_thread_ptr = std::make_unique<std::thread>(&yTcpServer::epoll_thread_context, this, con_cb_, discon_cb_);
    epoll_thread_ptr = std::unique_ptr<std::thread>( new std::thread(&yTcpServer::epoll_thread_context, this, con_cb, discon_cb));
    // can't detach this thread, because epoll_thread_context use some data in yTcpServer's obj.

    
    return 0;
}

void yTcpServer::clean_to_default(void) noexcept {

    is_sockfd_valid = false;
    is_bind_success = false;

    // In yTcpServer, we don't need to call connect
    is_connect_success = true;

    is_listen_success = false;

    socket_fd = -1; 

}

int8_t yTcpServer::connect(const std::string & ip, int32_t port, int domain ) noexcept {

    is_connect_success = true;
    // DO Nothing
    
    return 0;
}