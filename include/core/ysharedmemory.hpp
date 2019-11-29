/*
 * @Description: 
 * @Author: Sky
 * @Date: 2019-11-29 11:35:54
 * @LastEditors: Sky
 * @LastEditTime: 2019-11-29 11:39:54
 * @FilePath: \yLib\include\core\ysharedmemory.hpp
 * @Github: https://github.com/flyinskyin2013/yLib
 */
#ifndef _YLIB_CORE_YSHAREDMEMORY_HPP_
#define _YLIB_CORE_YSHAREDMEMORY_HPP_



#ifdef _WIN32 || _WIN64

#include <windows.h>
#include <stdint.h>
#include <string>
#include <iostream>

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


#elif __unix__ || __unix


#endif //_WIN32


#include "ycommon.hpp"
#include "ylog.hpp"


namespace yLib {

	class __yLib_EXPORT__ ySharedMemory
	{
	public:
		ySharedMemory() {}
		~ySharedMemory() {}


		uint32_t InitSharedMemory(uint32_t mem_key, uint64_t mem_size, std::string mem_name = "DefaultySharedMemory");
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
