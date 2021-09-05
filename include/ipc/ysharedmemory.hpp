/*
 * @Description: 
 * @Author: Sky
 * @Date: 2019-11-29 11:35:54
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2021-09-04 15:01:41
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

// ftok
#include <sys/types.h>
#include <sys/ipc.h>

// shmget
#include <sys/shm.h>

#include <stdint.h>
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
     *  @brief This is a implementation of sharedmemory based on linux and windows. 
	 *  On linux: 
	 *  It based on shmget/shmat/shmdt/shmctl. 
	 *  On windows: 
	 *  It based on CreateFileMapping/MapViewOfFile/UnmapViewOfFile/CloseHandle.
     */
	class __YLIB_CLASS_DECLSPEC__ ySharedMemory:
	YLIB_PUBLIC_INHERIT_YOBJECT
	{
	public:
		typedef struct __yshmparam__{
			__yshmparam__():
			shm_key(0),
			shm_flag(0)
			{}
			~__yshmparam__()
			{
				shm_key = 0;
				shm_flag = 0;
			}

			__yshmparam__(const __yshmparam__& param){

				shm_key = param.shm_key;
				shm_flag =  param.shm_flag;
				shm_name =  param.shm_name;
			}

			__yshmparam__ & operator=(const __yshmparam__ & param){

				shm_key = param.shm_key;
				shm_flag =  param.shm_flag;
				shm_name =  param.shm_name;	
				
				return *this;			
			}

			/** @var key_t shm_key
            	@brief it's the shm key, it only used on linux, when we create a shm.
				key_t's defination is #define key_t int.key_t is only support on linux.
        	*/
			int shm_key;

			/** @var int shm_flag
				@brief it's the user-flag for shm. \n
				On linux: \n
				Defaultly, we used 0666, IPC_CREAT and IPC_EXCL, more details: @see shmget
				On windows: \n
				Defaultly, we used PAGE_READWRITE, more details: @see CreateFileMappingA
			*/
			int shm_flag;

			/** @var std::_STD_STRING_ mem_name
            	@brief it's the shm name, it only used on windows, when we create a shm.
        	*/
			std::_STD_STRING_ shm_name;
		}yShmParam;
	public:
		ySharedMemory() = delete;

	    /**
         *  @fn   ySharedMemory(uint64_t shm_size_, const yShmParam & shm_param_) noexcept
         *  @brief Override constructor. if the special shm is not created, we will create and attach it, otherwist we will attach it.
         *  @param shm_size the shm's size.
		 *  @param shm_param the param of shm.
		 *  @param is_delete mark the shm will be deleted by destructor
         */
		ySharedMemory(uint64_t shm_size, const yShmParam & shm_param, bool is_delete=false) noexcept;
		
		/**
         *  @fn    ~ySharedMemory()
         *  @brief Default destructor
         */
		~ySharedMemory();

		/**
         *  @fn    void * GetShmDataPtr(void) noexcept
         *  @brief get the data-ptr of shm
		 *  @return the data-ptr of shm
		 *  @retval NULL shm is not ready, please re init it.
         */
		void * GetShmDataPtr(void) const noexcept {return shm_ptr;}

		/**
         *  @fn    bool ShmIsReady(void) const noexcept
         *  @brief get status of shm
		 *  @return the status of shm
		 *  @retval ok if the shm is ready.
         */
		bool ShmIsReady(void) const noexcept{return (shm_is_init_ready&&shm_is_attach_ready);}
	private:
		/** @var bool shm_is_init_ready
			@brief the shm is init-ready.
		*/
		bool shm_is_init_ready = false;

		/** @var bool shm_is_attach_ready
			@brief the shm is attach-ready.
		*/
		bool shm_is_attach_ready = false;

		/** @var bool is_mark_delete
			@brief mark the shm will be deleted by destructor
		*/
		bool is_mark_delete = false;

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
		int shm_id = 0;
#endif //_WIN32


		YLIB_DECLARE_CLASSINFO_CONTENT(ySharedMemory);
	};

}



#endif //__YLIB_IPC_YSHAREDMEMORY_HPP__
