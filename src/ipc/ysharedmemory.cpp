/*
Copyright (c) 2018 - 2021 flyinskyin2013 All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#include "ipc/ysharedmemory.hpp"

YLIB_IMPLEMENT_CLASSINFO_CONTENT_S(ySharedMemory)


yLib::ySharedMemory::ySharedMemory(uint64_t shm_size, const yShmParam & shm_param, bool is_delete) noexcept
:shm_is_init_ready(false),
shm_is_attach_ready(false),
shm_ptr(nullptr)
{
#ifdef _WIN32

	//=============================================init shm===============================

	shm_handle = NULL;
	if (CONVERT_STR_TO_YLIB_STD_STRING("") == shm_param.shm_name){

		yLib::yLog::E("The shm_param.shm_name is invalid, please init it on windows.\n");
		shm_is_init_ready = false;
		return ;
	}

	shm_handle = CreateFileMapping( \
			INVALID_HANDLE_VALUE, \
			NULL, \
			PAGE_READWRITE, \
			(DWORD)((shm_size >> 32) & 0x00000000FFFFFFFF), \
			(DWORD)((shm_size) & 0x00000000FFFFFFFF), \
			shm_param.shm_name.c_str() \
		);

	if (NO_ERROR == GetLastError()) {//check last error
		
		shm_is_init_ready = true;
		
	}
	else if (ERROR_ALREADY_EXISTS == GetLastError()) {// shared memory is exist
		
		yLib::yLog::W("yLib::ySharedMemory: shared memory(name = %s ) is exist\n", shm_param.shm_name.c_str());
		shm_is_init_ready = true;
		
	}
	else { // create shared memory failed.

		HLOCAL LocalAddress = NULL;
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_FROM_SYSTEM,
			NULL, GetLastError(), 0, (PTSTR)&LocalAddress, 0, NULL);
		yLib::yLog::E("yLib::ySharedMemory: create shared memory(name = %s ) failed. error info is %s\n", shm_param.shm_name.c_str(), (char *)LocalAddress);
		shm_is_init_ready = false;
		return ;
	}
	shm_is_init_ready = true;

	//=============================================attach shm===============================

	shm_ptr = MapViewOfFile(\
		shm_handle, \
		FILE_MAP_ALL_ACCESS,
		0,
		0,
		0 //If this parameter is 0 (zero), the mapping extends from the specified offset to the end of the file mapping.
	);

	if (NULL != shm_ptr) {
		
		shm_is_attach_ready = true;
	}
	else {

		HLOCAL LocalAddress = NULL;
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_FROM_SYSTEM,
			NULL, GetLastError(), 0, (PTSTR)&LocalAddress, 0, NULL);
		yLib::yLog::E("yLib::ySharedMemory::AttacheSharedMemory(): attach shared memory failed, error info is %s\n", (char *)LocalAddress);
		shm_is_attach_ready = false;
		return ;
	}

#elif __linux__ || __linux

	//=============================================init shm===============================
	shm_id = 0;
	is_mark_delete = is_delete;
	int _shm_flag = 0;
	if (0 == shm_param.shm_flag)//default flags
		_shm_flag = IPC_CREAT | IPC_EXCL | 0666;
	else 
		_shm_flag = shm_param.shm_flag;

	if (0 >  (shm_id = shmget(shm_param.shm_key, shm_size, _shm_flag))) {

		if (errno == EEXIST) {

			if (0 == shm_param.shm_flag) 
				_shm_flag = _shm_flag & (~IPC_EXCL); //clean default flag for IPC_EXCL

			if (0 > (shm_id = shmget(shm_param.shm_key, shm_size, _shm_flag))) {

				yLib::yLog::E("Re-attach shm failed, shm's key is 0x%x, errno is %d\n", shm_param.shm_key, errno);
				shm_is_init_ready = false;
				return ;
			}
			else {

				yLib::yLog::W("shared-mem exist, shm's id is %d\n", shm_id);
			}

		}
		else {

			yLib::yLog::E("create shm failed, shm's key is 0x%x, errno is %d\n", shm_param.shm_key, errno);
			shm_is_init_ready = false;
			return ;
		}
	}

	shm_is_init_ready = true;

	//=============================================attach shm===============================

	if (!shm_is_init_ready) {

		yLib::yLog::E("shm is not ready.\n");
		return ;
	}

	if ((void *)-1 == (shm_ptr = shmat(shm_id, NULL, 0))) {

		perror("shmat failed:");
		shm_is_attach_ready = false;
		return ;
	}

	shm_is_attach_ready = true;

#endif //_WIN32

	this->shm_param = shm_param;
	this->shm_size = shm_size;
}

/**
 *  @fn    ~ySharedMemory()
 *  @brief Default destructor
 */
yLib::ySharedMemory::~ySharedMemory()
{

	//=============================deattach=============================
	if (shm_is_attach_ready){

#ifdef _WIN32

		if (!UnmapViewOfFile(shm_ptr) && NO_ERROR != GetLastError() ) {

			HLOCAL LocalAddress = NULL;
			FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_FROM_SYSTEM,
				NULL, GetLastError(), 0, (PTSTR)&LocalAddress, 0, NULL);
			yLib::yLog::E("yLib::ySharedMemory::DetacheSharedMemory(): detache shared memory is failed. error info is %s\n", (char *)LocalAddress);
		}

		
#elif __linux__ || __linux

		if (0 > shmdt(shm_ptr)) {

			perror("shmdt failed");
			yLib::yLog::E("detache shm failed.");
		}

#endif //__linux__ || __linux
	}

	//=============================clean=============================
	if (shm_is_init_ready){

#ifdef _WIN32

		if (!CloseHandle(shm_handle) && NO_ERROR != GetLastError()) {
		
				HLOCAL LocalAddress = NULL;
			FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_FROM_SYSTEM,
				NULL, GetLastError(), 0, (PTSTR)&LocalAddress, 0, NULL);
			yLib::yLog::E("yLib::ySharedMemory::DestroySharedMemory(): destory shared memory is failed. error info is %s\n", (char *)LocalAddress);
		}
		else {

		}

		// https://docs.microsoft.com/en-us/previous-versions/aa914748(v=msdn.10)  TODO:To fully close a file-mapping object

#elif __linux__ || __linux

		if (is_mark_delete){

			shmget(shm_param.shm_key, shm_size, 0666);
			if (ENOENT == errno){

				yLib::yLog::W("shared-mem is deleted, we don't need to delete it again.\n");
			}
			else if (0 > shmctl(shm_id, IPC_RMID, NULL)) {

				perror("shmctl failed");
				yLib::yLog::E("destroy shm failed.");
			
			}
		}

#endif //__linux__ || __linux
	}

}
