/*
 * @Author: Sky
 * @Date: 2020-09-08 10:27:33
 * @LastEditors: Sky
 * @LastEditTime: 2020-09-14 18:18:40
 * @Description: 
 */

#include "network/yudpsocket.h"

#include "ylib.hpp"

using namespace yLib;

yUdpSocket::yUdpSocket(int socket_flags_, bool is_block_)
{
    socket_fd = -1;
    is_bind_success = false;
    if (is_block_){

        if ( 0 > (socket_fd = ::socket(AF_INET, SOCK_DGRAM, socket_flags_)) ){

            std::cout<<"yUdpSocket create socket failed. errno is "<< errno <<std::endl;
        }
    }
    else{

        if ( 0 > (socket_fd = ::socket(AF_INET, SOCK_DGRAM| SOCK_NONBLOCK, socket_flags_ )) ){

            std::cout<<"yUdpSocket create socket failed. errno is "<< errno <<std::endl;
        }
    }
}

yUdpSocket::yUdpSocket(){

    socket_fd = -1;
    is_bind_success = false;

    if ( 0 > (socket_fd = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) ){

        std::cout<<"yUdpSocket create socket failed. errno is "<< errno <<std::endl;
    }

}

yUdpSocket::~yUdpSocket()
{
    if (socket_fd > 0)//close socket_fd
        close(socket_fd);

    socket_fd = -1;
    is_bind_success = false;
}

inline bool yUdpSocket::socket_is_ready(void){

    return socket_is_valid();
}

int8_t yUdpSocket::bind(const std::string & ip_, int32_t port_){

    if (!socket_is_valid()){

        std::cout<<"yUdpClient: socket is invalid. please check."<<std::endl;
        return -1;
    }

	::memset(static_cast<void *>(&client_socket_addr), 0, sizeof(client_socket_addr));

    // if (0 == ::inet_aton(ip_.c_str(), &client_socket_addr.sin_addr)){//invalid ip

    //     std::cout<<"yUdpClient: Invalid ip_. please check."<<std::endl;
    //     return -1;
    // }
    client_socket_addr.sin_addr.s_addr = 0;
	client_socket_addr.sin_family = AF_INET;
	client_socket_addr.sin_port = htons(port_);

    if (0 > ::bind(socket_fd, reinterpret_cast<const struct sockaddr *>(&client_socket_addr), sizeof(client_socket_addr)) ){

        is_bind_success = false;
        std::cout<<"yUdpClient socket bind failed. errno is "<< errno <<std::endl;
        return -1;
    }

    is_bind_success = true;

    return 0;
}

int64_t yUdpSocket::sendto(const void * buffer_, uint64_t size_to_send_, std::string ip_, uint64_t port_, int flags_){

    if (!socket_is_ready()){

        std::cout<<"yUdpClient: socket is not ready. please check."<<std::endl;
        return -1;
    }

    struct sockaddr_in _send_addr;
    ::memset(&_send_addr, 0, sizeof(_send_addr));

    if (0 == ::inet_aton(ip_.c_str(), &_send_addr.sin_addr)){//invalid ip

        std::cout<<"yUdpClient: Invalid ip_. please check."<<std::endl;
        return -1;
    }

    _send_addr.sin_family = AF_INET;
	_send_addr.sin_port = htons(port_);

    ssize_t _complete_send_len = 0;
    if ( 0 > (_complete_send_len = ::sendto(socket_fd, buffer_, size_to_send_, 0, reinterpret_cast<const struct sockaddr *>(&_send_addr), sizeof(struct sockaddr_in))) ){

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
        std::cout<<"yUdpClient: send data failed. errno is "<<errno<<std::endl;
        return -1;
    }

    return _complete_send_len;
}

int64_t yUdpSocket::recvfrom(void * buffer_, uint64_t size_to_read_,  uint64_t &client_ip_, uint64_t &client_port_, int flags_){
    

    if (!socket_is_ready()){

        std::cout<<"yUdpClient: socket is not ready. please check."<<std::endl;
        return -1;
    }

    ssize_t _complete_read_len = 0;
    struct sockaddr_in _recv_addr;
    socklen_t _sockaddr_in_len = sizeof(_recv_addr);
    ::memset(&_recv_addr, 0, sizeof(_recv_addr));

    if ( 0 > (_complete_read_len = ::recvfrom(socket_fd, buffer_, size_to_read_, flags_, \
        reinterpret_cast<struct sockaddr *>(&_recv_addr), &_sockaddr_in_len)) ){

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
        std::cout<<"yUdpClient: recvfrom failed. errno is "<< errno <<std::endl;
        return -1;
    }

    client_ip_ = _recv_addr.sin_addr.s_addr;
    client_port_ = _recv_addr.sin_port;

    return _complete_read_len;
}