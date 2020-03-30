/*
 * @Description: 
 * @Author: Sky
 * @Date: 2019-11-29 11:35:54
 * @LastEditors  : Sky
 * @LastEditTime : 2020-01-14 16:57:01
 * @FilePath: \yLib\include\core\ysharedmemory.hpp
 * @Github: https://github.com/flyinskyin2013/yLib
 */
#ifndef _YLIB_CORE_YSHAREDMEMORY_HPP_
#define _YLIB_CORE_YSHAREDMEMORY_HPP_

#ifdef _WIN32
    #pragma warning(disable:4067) //warning C4067: 预处理器指令后有意外标记 - 应输入换行符
    #pragma warning(disable:4251) //warning C4251: xxxxx需要有dll接口由xxxx的客户端使用
#endif //_WIN32

#ifdef _WIN32 || _WIN64

#include <windows.h>
#include <stdint.h>
#include <string>
#include <iostream>

#ifndef UNICODE
#define _STD_STRING_ string
#else
#define _STD_STRING_ wstring
#endif //UNICODE

#elif __linux__ || __linux



#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include <sys/types.h>
#include <sys/shm.h>
#include <stdint.h>
#include <sys/ipc.h>
#include <stdio.h>
#include <errno.h>

#ifdef __cplusplus
}
#endif//__cplusplus

#define _STD_STRING_ string

#elif __unix__ || __unix


#endif //_WIN32


#include "ycommon.hpp"
#include "ylog.hpp"


namespace yLib {

	class __YLIB_EXPORT__ ySharedMemory
	{
	public:
		ySharedMemory() {}
		~ySharedMemory() {}


		uint32_t InitSharedMemory(uint32_t mem_key, uint64_t mem_size, std::_STD_STRING_ mem_name = "DefaultySharedMemory");
		uint32_t AttacheSharedMemory(void ** shm_addr);
		uint32_t DetacheSharedMemory(void * shm_addr);
		uint32_t DestroySharedMemory(void);
	private:
		bool shm_is_ready = false;
		int cur_shm_id = 0;

#ifdef _WIN32 || _WIN64

		HANDLE hMapFile;

#endif //_WIN32 || _WIN64

	};

}



#endif //_YLIB_CORE_YSHAREDMEMORY_HPP_
