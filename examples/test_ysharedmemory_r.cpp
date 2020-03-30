/*
 * @Description: 
 * @Author: Sky
 * @Date: 2019-11-29 11:50:37
 * @LastEditors: Sky
 * @LastEditTime: 2019-12-02 19:23:34
 * @FilePath: \yLib\examples\test_ysharedmemory_r.cpp
 * @Github: https://github.com/flyinskyin2013/yLib
 */
#include "ysharedmemory.hpp"

#define SHARED_MEM_SIZE 1024 * 1024 * 1024

#include <iostream>

int main(int argc, char * argv[]) {
	yLib::ySharedMemory shm;
	shm.InitSharedMemory(0, 1024 * 1024 * 1024, "MY_SHARED_MEM_TEST");
	char * shm_addr = NULL;
	shm.AttacheSharedMemory((void **)(&shm_addr));

	//memset(shm_addr, 0, 1024 * 1024 * 1024);

	while (shm_addr[0] != (char)0xFF) {

#ifdef _WIN32 || _WIN64
		Sleep(1000);
#elif __linux__ || __linux

		sleep(1);
#elif __unix__ || __unix


#endif //_WIN32
		
		//std::cout << "waitting msg " << std::endl;
		if (shm_addr[SHARED_MEM_SIZE - 1] == (char)0xFF) {

			std::cout << "shared mem result is :" << shm_addr + 1 << std::endl;
			shm_addr[SHARED_MEM_SIZE - 1] = 0x00;
		}
	}
	shm.DetacheSharedMemory(shm_addr);
	shm.DestroySharedMemory();
	system("pause");
	return 0;
}