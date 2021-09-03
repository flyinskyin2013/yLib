/*
 * @Author: Sky
 * @Date: 2020-09-14 17:38:34
 * @LastEditors: Sky
 * @LastEditTime: 2021-08-30 17:26:50
 * @Description: 
 */
#ifndef __YLIB_NETWORK_YNETWORK_HPP__
#define __YLIB_NETWORK_YNETWORK_HPP__

#if defined(_WIN32) || defined(_WIN64)


#elif defined(__linux__) || defined(__linux)
    
    #include "ytcpserver.h"
    #include "ytcpsocket.h"
    #include "yudpsocket.h"
    
#elif defined(__unix__) || defined(__unix)

#endif //__unix__ || __unix

#endif //__YLIB_NETWORK_YNETWORK_HPP__