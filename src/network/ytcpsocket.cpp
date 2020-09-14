/*
 * @Author: Sky
 * @Date: 2020-09-08 10:27:12
 * @LastEditors: Sky
 * @LastEditTime: 2020-09-14 18:00:59
 * @Description: 
 */
#include "network/ytcpsocket.h"

#include "ylib.hpp"

using namespace yLib;

yTcpSocket::yTcpSocket(/* args */)
{
    socket_fd = -1;
    is_bind_success = false;

    if ( 0 > (socket_fd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) ){

        std::cout<<"yTcpSocket create socket failed. errno is "<< errno <<std::endl;
    }
}

yTcpSocket::yTcpSocket(int socket_flags_, bool is_block_){

    socket_fd = -1;
    is_bind_success = false;
    if (is_block_){

        if ( 0 > (socket_fd = ::socket(AF_INET, SOCK_STREAM, socket_flags_)) ){

            std::cout<<"yTcpSocket create socket failed. errno is "<< errno <<std::endl;
        }
    }
    else{

        if ( 0 > (socket_fd = ::socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, socket_flags_ )) ){

            std::cout<<"yTcpSocket create socket failed. errno is "<< errno <<std::endl;
        }
    }
}

yTcpSocket::~yTcpSocket()
{
    if (socket_fd > 0)//close socket_fd
        close(socket_fd);

    socket_fd = -1;
    is_bind_success = false;
}

/**
 * @description: Check socket(), bind(), listen(), accept() and so on.
 * @param {type} 
 * @return {type} 
*/
inline bool yTcpSocket::socket_is_ready(void){

    return socket_is_valid();
}

/**
 * @description: bind a port for send
 * @param {type} 
 * @return {type} 
 */
int8_t yTcpSocket::bind(const std::string & ip_, int32_t port_){

    if (!socket_is_valid()){

        std::cout<<"yTcpClient: socket is invalid. please check."<<std::endl;
        return -1;
    }

	::memset(static_cast<void *>(&client_socket_addr), 0, sizeof(client_socket_addr));

    client_socket_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	client_socket_addr.sin_family = AF_INET;
	client_socket_addr.sin_port = htons(port_);

    if (0 > ::bind(socket_fd, reinterpret_cast<const struct sockaddr *>(&client_socket_addr), sizeof(client_socket_addr)) ){

        is_bind_success = false;
        std::cout<<"yTcpClient socket bind failed. errno is "<< errno <<std::endl;
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
int8_t yTcpSocket::connect(const std::string & ip_, int32_t port_){
    
    struct sockaddr_in _server_socket_addr;
    socklen_t _server_socket_addr_len = sizeof(_server_socket_addr);
    ::memset(&_server_socket_addr, 0, sizeof(_server_socket_addr));

    _server_socket_addr.sin_port = ::htons(port_);
    if (0 == ::inet_aton(ip_.c_str(), &_server_socket_addr.sin_addr)){//invalid ip

        std::cout<<"yUdpClient: Invalid ip_. please check."<<std::endl;
        return -1;
    }
    _server_socket_addr.sin_family = AF_INET;

    if ( 0 > ::connect(socket_fd, reinterpret_cast<const sockaddr *>(&_server_socket_addr), _server_socket_addr_len)){

        if (EINPROGRESS == errno){//

            /*
            EINPROGRESS
              The socket is nonblocking and the connection cannot be completed
              immediately.  It is possible to select(2) or poll(2) for comple‐
              tion by selecting the socket for writing.  After select(2) indi‐
              cates writability, use getsockopt(2) to read the SO_ERROR option
              at  level  SOL_SOCKET  to  determine whether connect() completed
              successfully (SO_ERROR is zero) or unsuccessfully  (SO_ERROR  is
              one  of the usual error codes listed here, explaining the reason
              for the failure).
            */
           while(1){

                fd_set _client_fd_set;
                FD_ZERO(&_client_fd_set);
                FD_SET(socket_fd, &_client_fd_set);
                struct timeval _wait_time;
                _wait_time.tv_usec = 1000*1000; //wait for 1s
                int _select_ret = select(socket_fd + 1, NULL, &_client_fd_set, NULL, &_wait_time);
                if (_select_ret > 0){
                    
                    int _opt_val = 0;
                    socklen_t _opt_len = sizeof(_opt_val);

                    if ( 0 > getsockopt(socket_fd, SOL_SOCKET, SO_ERROR, &_opt_val, &_opt_len)){

                        yLib::yLog::E("yTcpClient: connect failed. In EINPROGRESS state. getsockopt error, errno is %d.", errno);
                    }

                    if (0 == _opt_val){//determine whether connect() completed successfully (SO_ERROR is zero)
                     
                        return 0;
                    }
                    else
                    {
                        yLib::yLog::E("yTcpClient: connect failed. In EINPROGRESS state. in select/getsockopt: socket is not ready.");
                        break;
                    }
                    

                }
                else{//timeout 0 ==_select_ret; occur error 0 > _select_ret

                    yLib::yLog::E("yTcpClient: connect failed. In EINPROGRESS state. select timeout or error.");
                    if (0 > _select_ret){

                        yLib::yLog::E("yTcpClient: connect failed. In EINPROGRESS state. select error, errno is %d.", errno);
                    }

                    break;
                }
           }

        }

        std::cout<<"yTcpClient: connect failed. errno is "<<errno<<std::endl;
        return -1;
    }

    return 0;
}

/**
 * @description: 
 * @param {type} 
 * @return {type} 
 */
int64_t yTcpSocket::read(void *buffer_, size_t count_){

    ssize_t _read_len = 0;

    if ( 0 > (_read_len = ::read(socket_fd, buffer_, count_))){

        if (errno == EAGAIN || errno == EWOULDBLOCK){

            return -2;//no data to read
        }

        std::cout<<"yTcpClient: read data failed. errno is "<<errno<<std::endl;
        return -1;
    }

    return _read_len;
}

/**
 * @description: 
 * @param {type} 
 * @return {type} 
 */
int64_t yTcpSocket::write(const void *buffer_, size_t count_){

    ssize_t _write_len = 0;

    if ( 0 > (_write_len = ::write(socket_fd, buffer_, count_))){

        if (errno == EAGAIN || errno == EWOULDBLOCK){

            return 0;//no data to write
        }

        std::cout<<"yTcpClient: write data failed. errno is "<<errno<<std::endl;
        return -1;
    }

    return _write_len;
}