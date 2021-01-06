/*
 * @Author: Sky
 * @Date: 2019-12-02 19:24:00
 * @LastEditors: Sky
 * @LastEditTime: 2021-01-06 16:31:57
 * @Description: 
 */
#include "ylib.hpp"

#define SHARED_MEM_SIZE 1024 * 1024 * 1024

#include <iostream>

int main(int argc, char * argv[]) {

	yLib::ySharedMemory::yShmParam _shm_param;
	#ifdef _WIN32
	_shm_param.mem_name = "MY_SHARED_MEM_TEST";
	#elif __linux__ || __linux
	_shm_param.mem_key = 0x123456a;
	#endif //_WIN32

	yLib::ySharedMemory _shm(SHARED_MEM_SIZE, _shm_param);


	char * _shm_addr = (char *)_shm.GetShmDataPtr();

	std::cout << "ySharedMemory write 1" << std::endl;
	memset(_shm_addr, 0, 1024 * 1024 * 1024);

	std::string msg0 = "sal;dkfj;alkdfj;lasdjflkjasdf;ja;lsdfkj";

	memcpy(_shm_addr + 1, msg0.c_str(), msg0.length());

	_shm_addr[SHARED_MEM_SIZE - 1] = 0xFF;

	while (_shm_addr[SHARED_MEM_SIZE - 1] == (char)0xFF) {
	
		std::cout << "wait reader ... ..." << std::endl;
#ifdef _WIN32
		Sleep(1000);
#elif __linux__ || __linux

		sleep(1);
#elif __unix__ || __unix


#endif //_WIN32
	}

	std::cout << "ySharedMemory write 2" << std::endl;
	memset(_shm_addr, 0, 1024 * 1024 * 1024);
	std::string msg1 = "test111111111111111111111111111111";
	memcpy(_shm_addr + 1, msg1.c_str(), msg1.length());

	_shm_addr[SHARED_MEM_SIZE - 1] = 0xFF;

	while (_shm_addr[SHARED_MEM_SIZE - 1] == (char)0xFF) {

#ifdef _WIN32
		Sleep(1000);
#elif __linux__ || __linux

		sleep(1);
#elif __unix__ || __unix


#endif //_WIN32
	}
	std::cout << "ySharedMemory write 3" << std::endl;
	memset(_shm_addr, 0, 1024 * 1024 * 1024);
	std::string msg2 = "test1111111111111111111111111111112222222222222222222223333333333333333";
	memcpy(_shm_addr + 1, msg2.c_str(), msg2.length());

	_shm_addr[SHARED_MEM_SIZE - 1] = 0xFF;

	while (_shm_addr[SHARED_MEM_SIZE - 1] == (char)0xFF) {

#ifdef _WIN32
		Sleep(1000);
#elif __linux__ || __linux

		sleep(1);
#elif __unix__ || __unix


#endif //_WIN32
	}

	_shm_addr[0] = 0xFF;

	system("pause");
	return 0;
}