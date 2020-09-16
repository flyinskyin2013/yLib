/*
 * @Author: Sky
 * @Date: 2020-09-08 14:53:08
 * @LastEditors: Sky
 * @LastEditTime: 2020-09-16 14:07:27
 * @Description: 
 */

#include "network/yudpserver.h"

#ifdef __cplusplus
extern "C"{
#endif //__cplusplus

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#ifdef __cplusplus
}
#endif //__cplusplus


#include "ylib.hpp"

using namespace yLib;

yUdpServer::yUdpServer(){

    socket_fd = -1;
    is_bind_success = false;


    if ( 0 > (socket_fd = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) ){

        std::cout<<"yUdpServer create socket failed. errno is "<< errno <<std::endl;
    }

    max_client_num = UINT32_MAX;
}


yUdpServer::yUdpServer(uint64_t max_client_num_, int socket_flags_, bool is_block_){

    socket_fd = -1;
    is_bind_success = false;

    if (is_block_){

        if ( 0 > (socket_fd = ::socket(AF_INET, SOCK_DGRAM, socket_flags_)) ){

            std::cout<<"yUdpServer create socket failed. errno is "<< errno <<std::endl;
        }
    }
    else{
        
        if ( 0 > (socket_fd = ::socket(AF_INET, SOCK_DGRAM|SOCK_NONBLOCK, socket_flags_)) ){

            std::cout<<"yUdpServer create socket failed. errno is "<< errno <<std::endl;
        }
    }

    max_client_num = max_client_num_;
}

yUdpServer::~yUdpServer()
{
    if (socket_fd > 0)//close socket_fd
        close(socket_fd);

    socket_fd = -1;
    is_bind_success = false;
    max_client_num = 0;
}
/**
 * @description: Check socket(), bind(), listen(), accept() and so on.
 * @param {type} 
 * @return {type} 
*/
inline bool yUdpServer::socket_is_ready(void){

    if (!socket_is_valid())
        return false;
    
    if (!is_bind_success)
        return false;
    
    return true;
}

int8_t yUdpServer::bind(const std::string & ip_, int32_t port_){

    if (!socket_is_valid()){

        std::cout<<"yUdpServer: socket is invalid. please check."<<std::endl;
        return -1;
    }

	::memset(static_cast<void *>(&server_socket_addr), 0, sizeof(server_socket_addr));

    //	socket_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (0 == ::inet_aton(ip_.c_str(), &server_socket_addr.sin_addr)){//invalid ip

        std::cout<<"yUdpServer: Invalid ip_. please check."<<std::endl;
        return -1;
    }
	server_socket_addr.sin_family = AF_INET;
	server_socket_addr.sin_port = htons(port_);

    if (0 > ::bind(socket_fd, reinterpret_cast<const struct sockaddr *>(&server_socket_addr), sizeof(server_socket_addr)) ){

        is_bind_success = false;
        std::cout<<"yUdpServer socket bind failed. errno is "<< errno <<std::endl;
        return -1;
    }

    is_bind_success = true;
    return 0;
}

/**
 * @description: 
 * @param {type} 
 * 
 * @return {type} 
 */ 
int64_t yUdpServer::sendto(const void * buffer_, uint64_t size_to_send_, const std::string &client_ip_, uint64_t client_port_, int flags_){

    if (!socket_is_ready()){

        std::cout<<"yUdpServer: socket is not ready. please check."<<std::endl;
        return -1;
    }

    ssize_t _complete_send_len = 0;
    uint64_t _binary_ip = translate_ip_to_binary(client_ip_);
    uint64_t _binary_port = translate_port_to_binary(client_port_);

    std::vector<struct sockaddr_in>::iterator _tmp_iter = client_socket_addr_vec.begin();
    for (; _tmp_iter != client_socket_addr_vec.end(); _tmp_iter++){

        if ( _binary_ip == _tmp_iter->sin_addr.s_addr){

            if (_binary_port == _tmp_iter->sin_port){

                break;
            }
        }
    }

    if (client_socket_addr_vec.end() == _tmp_iter){

        std::cout<<"yUdpServer: don't find this client. client ip is "<<client_ip_<<std::endl;
        return -1;
    }


    if ( 0 > (_complete_send_len = ::sendto(socket_fd, buffer_, size_to_send_, 0, reinterpret_cast<const struct sockaddr *>(&(*_tmp_iter)), sizeof(struct sockaddr_in))) ){

        if (errno == EAGAIN || errno == EWOULDBLOCK){
            /*
                EAGAIN or EWOULDBLOCK
                        The  socket  is  marked  nonblocking and the requested operation
                        would block.  POSIX.1-2001 allows either error  to  be  returned
                        for  this case, and does not require these constants to have the
                        same value, so a portable application should check for both pos‐
                        sibilities.

            */
            return 0;
        }        
        std::cout<<"yUdpServer: send data failed. errno is "<<errno<<std::endl;
        return -1;
    }

    return _complete_send_len;

}

/**
 * @description: 
 * @param {type} 
 * 
 * @return {type} 
 */
int64_t yUdpServer::recvfrom(void * buffer_, uint64_t size_to_read_, std::string &client_ip_, uint64_t &client_port_, int flags_){

    if (!socket_is_ready()){

        std::cout<<"yUdpServer: socket is not ready. please check."<<std::endl;
        return -1;
    }

    ssize_t _complete_read_len = 0;
    struct sockaddr_in _tmp_sockaddr_in;
    socklen_t _tmp_sockaddr_in_len = sizeof(_tmp_sockaddr_in);

    ::memset(&_tmp_sockaddr_in, 0, sizeof(_tmp_sockaddr_in));

    if ( 0 > (_complete_read_len = ::recvfrom(socket_fd, buffer_, size_to_read_, flags_, \
        reinterpret_cast<struct sockaddr *>(&_tmp_sockaddr_in), &_tmp_sockaddr_in_len)) ){

        if (errno == EAGAIN || errno == EWOULDBLOCK){
            /*
                EAGAIN or EWOULDBLOCK
                        The  socket  is  marked  nonblocking and the requested operation
                        would block.  POSIX.1-2001 allows either error  to  be  returned
                        for  this case, and does not require these constants to have the
                        same value, so a portable application should check for both pos‐
                        sibilities.

            */
            return 0;
        }       
        std::cout<<"yUdpServer: recvfrom failed. errno is "<< errno <<std::endl;
        return -1;
    }

    client_ip_ = get_ip_from_binary(_tmp_sockaddr_in.sin_addr.s_addr);
    client_port_ = get_port_from_binary(_tmp_sockaddr_in.sin_port);

    client_socket_addr_vec.push_back(_tmp_sockaddr_in);

    return _complete_read_len;
}

