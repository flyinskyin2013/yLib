#include "ysharedmemory.hpp"


uint32_t yLib::ySharedMemory::InitSharedMemory(uint32_t mem_key, uint64_t mem_size, std::string mem_name) {

#ifdef _WIN32 || _WIN64
	hMapFile = CreateFileMapping( \
			INVALID_HANDLE_VALUE, \
			NULL, \
			PAGE_READWRITE, \
			(DWORD)((mem_size >> 32) & 0x00000000FFFFFFFF), \
			(DWORD)((mem_size) & 0x00000000FFFFFFFF), \
			mem_name.c_str() \
		);
	if (NO_ERROR == GetLastError()) {
		
		return 0;
	}
	else if (ERROR_ALREADY_EXISTS == GetLastError()) {// shared memory is exist
		
		yLib::yLog::W("yLib::ySharedMemory::InitSharedMemory(): shared memory(name = %s ) is exist", mem_name.c_str());
		return 1;
	}
	else { // create shared memory failed.

		yLib::yLog::E("yLib::ySharedMemory::InitSharedMemory(): create shared memory(name = %s ) failed.", mem_name.c_str());
		return -1;
	}

#elif __linux__ || __linux
	if (0 >  (cur_shm_id = shmget(mem_key, mem_size, IPC_CREAT | IPC_EXCL | 0666))) {

		if (errno == EEXIST) {

			if (0 > (cur_shm_id = shmget(mem_key, mem_size, IPC_CREAT | 0666))) {

				yLib::yLog::E("Re-attach shm failed, shm's key is %d", mem_key);
				return -1;
			}
			else {

				yLib::yLog::W("shared-mem exist, shm's id is %d", cur_shm_id);
			}

		}
		else {

			perror("shmget failed:");
			return -1;
		}
	}
	shm_is_ready = true;
	return 0;
#endif //__linux__ || __linux
}

uint32_t yLib::ySharedMemory::AttacheSharedMemory(void ** shm_addr) {
#ifdef _WIN32 || _WIN64
	*shm_addr = MapViewOfFile(\
			hMapFile, \
			FILE_MAP_ALL_ACCESS,
			0,
			0,
			0 //If this parameter is 0 (zero), the mapping extends from the specified offset to the end of the file mapping.
		);

	if (NULL != shm_addr) {
		
		return 0;
	}
	else {

		HLOCAL LocalAddress = NULL;
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_FROM_SYSTEM,
			NULL, GetLastError(), 0, (PTSTR)&LocalAddress, 0, NULL);
		yLib::yLog::E("yLib::ySharedMemory::AttacheSharedMemory(): attach shared memory failed, error info is %s", (char *)LocalAddress);
		return -1;
	}
#elif __linux__ || __linux
	if (!shm_is_ready) {

		yLib::yLog::E("shm is not ready.");
		return -1;
	}

	if ((void *)-1 == (*shm_addr = shmat(cur_shm_id, NULL, 0))) {

		perror("shmat failed:");
		yLib::yLog::E("attache shm failed.");
		return -1;
	}

	return 0;
#endif //__linux__ || __linux

}

uint32_t yLib::ySharedMemory::DetacheSharedMemory(void * shm_addr) {

#ifdef _WIN32 || _WIN64

	if (!UnmapViewOfFile(shm_addr) && NO_ERROR != GetLastError() ) {

		HLOCAL LocalAddress = NULL;
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_FROM_SYSTEM,
			NULL, GetLastError(), 0, (PTSTR)&LocalAddress, 0, NULL);
		yLib::yLog::E("yLib::ySharedMemory::DetacheSharedMemory(): detache shared memory is failed. error info is %s", (char *)LocalAddress);
		return -1;
	}
	else {
		
		return 0;
	}
	
#elif __linux__ || __linux

	if (0 > shmdt(shm_addr)) {

		perror("shmdt failed");
		yLib::yLog::E("detache shm failed.");
		return -1;
	}

	return 0;
#endif //__linux__ || __linux

}

uint32_t yLib::ySharedMemory::DestroySharedMemory(void) {
#ifdef _WIN32 || _WIN64

	if (!CloseHandle(hMapFile) && NO_ERROR != GetLastError()) {
	
			HLOCAL LocalAddress = NULL;
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_FROM_SYSTEM,
			NULL, GetLastError(), 0, (PTSTR)&LocalAddress, 0, NULL);
		yLib::yLog::E("yLib::ySharedMemory::DestroySharedMemory(): destory shared memory is failed. error info is %s", (char *)LocalAddress);
		return -1;
	}
	else {

		return 0;
	}

#elif __linux__ || __linux

	if (0 > shmctl(cur_shm_id, IPC_RMID, NULL)) {

		perror("shmctl failed");
		yLib::yLog::E("destroy shm failed.");
		return -1;
	}
	return 0;

#endif //__linux__ || __linux

}