#include "ipc/ysharedmemory.hpp"

/**
 *  @fn   ySharedMemory(uint64_t mem_size_, const yShmParam & shm_param_)
 *  @brief Override constructor
 *  @param mem_size_ the shm's size.
 *  @param shm_param_ the param of shm.
 */
yLib::ySharedMemory::ySharedMemory(uint64_t mem_size_, const yShmParam & shm_param_ )
MACRO_INIT_YOBJECT_PROPERTY(ySharedMemory)
{
#ifdef _WIN32

	//=============================================init shm===============================

	shm_handle = NULL;
	if ("" == shm_param_.mem_name){

		yLib::yLog::E("The shm_param.mem_name is invalid, please init it on windows.");
		shm_is_init_ready = false;
		return ;
	}

	shm_handle = CreateFileMapping( \
			INVALID_HANDLE_VALUE, \
			NULL, \
			PAGE_READWRITE, \
			(DWORD)((mem_size_ >> 32) & 0x00000000FFFFFFFF), \
			(DWORD)((mem_size_) & 0x00000000FFFFFFFF), \
			shm_param_.mem_name.c_str() \
		);

	if (NO_ERROR == GetLastError()) {//check last error
		
		shm_is_init_ready = true;
		
	}
	else if (ERROR_ALREADY_EXISTS == GetLastError()) {// shared memory is exist
		
		yLib::yLog::W("yLib::ySharedMemory: shared memory(name = %s ) is exist", shm_param_.mem_name.c_str());
		shm_is_init_ready = true;
		
	}
	else { // create shared memory failed.

		HLOCAL LocalAddress = NULL;
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_FROM_SYSTEM,
			NULL, GetLastError(), 0, (PTSTR)&LocalAddress, 0, NULL);
		yLib::yLog::E("yLib::ySharedMemory: create shared memory(name = %s ) failed. error info is %s", shm_param_.mem_name.c_str(), (char *)LocalAddress);
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
		yLib::yLog::E("yLib::ySharedMemory::AttacheSharedMemory(): attach shared memory failed, error info is %s", (char *)LocalAddress);
		shm_is_attach_ready = false;
		return ;
	}

#elif __linux__ || __linux

	//=============================================init shm===============================
	cur_shm_id = 0;

	if (0 == shm_param_.mem_key){//check the user-special key

		yLib::yLog::E("The shm_param.mem_key is invalid, please init it on linux.");
		shm_is_init_ready = false;
		return ;
	}

	if (0 >  (cur_shm_id = shmget(shm_param_.mem_key, mem_size_, IPC_CREAT | IPC_EXCL | 0666))) {

		if (errno == EEXIST) {

			if (0 > (cur_shm_id = shmget(shm_param_.mem_key, mem_size_, IPC_CREAT | 0666))) {

				yLib::yLog::E("Re-attach shm failed, shm's key is %d", shm_param_.mem_key);
				shm_is_init_ready = false;
				return ;
			}
			else {

				yLib::yLog::W("shared-mem exist, shm's id is %d", cur_shm_id);
			}

		}
		else {

			perror("shmget failed:");
			shm_is_init_ready = false;
			return ;
		}
	}

	shm_is_init_ready = true;

	//=============================================attach shm===============================

	if (!shm_is_init_ready) {

		yLib::yLog::E("shm is not ready.");
		return ;
	}

	if ((void *)-1 == (shm_ptr = shmat(cur_shm_id, NULL, 0))) {

		perror("shmat failed:");
		shm_is_attach_ready = false;
		return ;
	}

	shm_is_attach_ready = true;

#endif //_WIN32

	shm_param = shm_param_;
	shm_size = mem_size_;
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
			yLib::yLog::E("yLib::ySharedMemory::DetacheSharedMemory(): detache shared memory is failed. error info is %s", (char *)LocalAddress);
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
			yLib::yLog::E("yLib::ySharedMemory::DestroySharedMemory(): destory shared memory is failed. error info is %s", (char *)LocalAddress);
		}
		else {

		}

#elif __linux__ || __linux

		shmget(shm_param.mem_key, shm_size, 0666);
		if (ENOENT == errno){

			yLib::yLog::W("shared-mem is deleted, we don't need to delete it again.");
		}
		else if (0 > shmctl(cur_shm_id, IPC_RMID, NULL)) {

			perror("shmctl failed");
			yLib::yLog::E("destroy shm failed.");
		
		}

#endif //__linux__ || __linux
	}

}
