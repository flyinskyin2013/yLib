/*
 * @Description: 
 * @Author: Sky
 * @Date: 2019-11-29 11:50:37
 * @LastEditors: Sky
 * @LastEditTime: 2021-01-06 16:32:24
 * @FilePath: \yLib\examples\test_ysharedmemory_r.cpp
 * @Github: https://github.com/flyinskyin2013/yLib
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

	memset(_shm_addr, 0, SHARED_MEM_SIZE);

	while (_shm_addr[0] != (char)0xFF) {

#ifdef _WIN32
		Sleep(1000);
#elif __linux__ || __linux

		sleep(1);
#endif //_WIN32
		
		//std::cout << "waitting msg " << std::endl;
		if (_shm_addr[SHARED_MEM_SIZE - 1] == (char)0xFF) {

			std::cout << "shared mem result is :" << _shm_addr + 1 << std::endl;
			_shm_addr[SHARED_MEM_SIZE - 1] = 0x00;
		}
	}

	system("pause");
	return 0;
}