#include "ySharedMemory.hpp"

#define SHARED_MEM_SIZE 1024 * 1024 * 1024

#include <iostream>

int main(int argc, char * argv[]) {

	yLib::ySharedMemory shm;
	shm.InitSharedMemory(0, 1024 * 1024 * 1024, "MY_SHARED_MEM_TEST");


	char * shm_addr = NULL;
	shm.AttacheSharedMemory((void **)(&shm_addr));

	std::cout << "ySharedMemory write 1" << std::endl;
	memset(shm_addr, 0, 1024 * 1024 * 1024);

	std::string msg0 = "sal;dkfj;alkdfj;lasdjflkjasdf;ja;lsdfkj";

	memcpy(shm_addr + 1, msg0.c_str(), msg0.length());

	shm_addr[SHARED_MEM_SIZE - 1] = 0xFF;

	while (shm_addr[SHARED_MEM_SIZE - 1] == (char)0xFF) {
	
		std::cout << "wait reader ... ..." << std::endl;
		Sleep(1000);
	}

	std::cout << "ySharedMemory write 2" << std::endl;
	memset(shm_addr, 0, 1024 * 1024 * 1024);
	std::string msg1 = "test111111111111111111111111111111";
	memcpy(shm_addr + 1, msg1.c_str(), msg1.length());

	shm_addr[SHARED_MEM_SIZE - 1] = 0xFF;

	while (shm_addr[SHARED_MEM_SIZE - 1] == (char)0xFF) {

		Sleep(1000);
	}
	std::cout << "ySharedMemory write 3" << std::endl;
	memset(shm_addr, 0, 1024 * 1024 * 1024);
	std::string msg2 = "test1111111111111111111111111111112222222222222222222223333333333333333";
	memcpy(shm_addr + 1, msg2.c_str(), msg2.length());

	shm_addr[SHARED_MEM_SIZE - 1] = 0xFF;

	while (shm_addr[SHARED_MEM_SIZE - 1] == (char)0xFF) {

		Sleep(1000);
	}

	shm_addr[0] = 0xFF;

	shm.DetacheSharedMemory(shm_addr);
	//shm.DestroySharedMemory();

	system("pause");
	return 0;
}