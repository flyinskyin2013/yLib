/*
 * @Description: 
 * @Author: Sky
 * @Date: 2019-11-29 11:35:54
 * @LastEditors: Sky
 * @LastEditTime: 2021-01-15 10:58:35
 * @FilePath: \yLib\include\core\ysharedmemory.hpp
 * @Github: https://github.com/flyinskyin2013/yLib
 */
#ifndef __YLIB_IPC_YSHAREDMEMORY_HPP__
#define __YLIB_IPC_YSHAREDMEMORY_HPP__

#ifdef _WIN32
    #pragma warning(disable:4067) //warning C4067: 预处理器指令后有意外标记 - 应输入换行符
    #pragma warning(disable:4251) //warning C4251: xxxxx需要有dll接口由xxxx的客户端使用
#endif //_WIN32

#ifdef _WIN32

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


#include "core/ycommon.hpp"
#include "utility/ylog.hpp"


namespace yLib {

    /**
     *  @class ySharedMemory
     *  @brief This is a implementation of sharedmemory based on linux and windows. \n
	 *  On linux: \n
	 *  It based on shmget/shmat/shmdt/shmctl. \n
	 *  On windows: \n
	 *  It based on CreateFileMapping/MapViewOfFile/UnmapViewOfFile/CloseHandle.
     */
	class __YLIB_EXPORT__ ySharedMemory MACRO_PUBLIC_INHERIT_YOBJECT
	{
	public:
		typedef struct __yshmparam__{
			__yshmparam__():
			mem_key(0),
			mem_flag(-1),
			mem_name("")
			{}
			~__yshmparam__()
			{
				mem_key = 0;
				mem_flag = -1;
			}

			__yshmparam__(const __yshmparam__& param_){

				mem_key = param_.mem_key;
				mem_flag =  param_.mem_flag;
				mem_name =  param_.mem_name;
			}

			/** @var uint32_t mem_key
            	@brief it's the shm key, it only used on linux, when we create a shm.
        	*/
			uint32_t mem_key;

			/** @var int32_t mem_flag
				@brief it's the user-flag for shm. \n
				On linux: \n
				Defaultly, we used 0666, IPC_CREAT and IPC_EXCL, more details: @see shmget
				On windows: \n
				Defaultly, we used PAGE_READWRITE, more details: @see CreateFileMappingA
			*/
			int32_t mem_flag;

			/** @var std::_STD_STRING_ mem_name
            	@brief it's the shm name, it only used on windows, when we create a shm.
        	*/
			std::_STD_STRING_ mem_name;
		}yShmParam;
	public:
		ySharedMemory() = delete;

	    /**
         *  @fn   ySharedMemory(uint64_t mem_size_, const yShmParam & shm_param_)
         *  @brief Override constructor. if the special shm is not created, we will create and attach it, otherwist we will attach it.
         *  @param mem_size_ the shm's size.
		 *  @param shm_param_ the param of shm.
         */
		ySharedMemory(uint64_t mem_size_, const yShmParam & shm_param_);
		
		/**
         *  @fn    ~ySharedMemory()
         *  @brief Default destructor
         */
		~ySharedMemory();

		/**
         *  @fn    void * GetShmDataPtr(void)
         *  @brief get the data-ptr of shm
		 *  @return the data-ptr of shm
		 *  @retval NULL shm is not ready, please re init it.
         */
		void * GetShmDataPtr(void) const{return shm_ptr;}
	private:
		/** @var bool shm_is_init_ready
			@brief the shm is init-ready.
		*/
		bool shm_is_init_ready = false;

		/** @var bool shm_is_attach_ready
			@brief the shm is attach-ready.
		*/
		bool shm_is_attach_ready = false;

		/** @var void * shm_ptr
			@brief the data-ptr of shm.
		*/
		void * shm_ptr = nullptr;

		/** @var yShmParam cur_shm_param
			@brief the cur param of shm.
		*/
		yShmParam shm_param;

		uint64_t shm_size = 0;

#ifdef _WIN32
		/** @var HANDLE shm_handle
			@brief the shm's handle on windows.
		*/
		HANDLE shm_handle;
#elif __linux__ || __linux
		/** @var int cur_shm_id
			@brief the shm's id on linux.
		*/
		int cur_shm_id = 0;
#endif //_WIN32

	};

}



#endif //__YLIB_IPC_YSHAREDMEMORY_HPP__
