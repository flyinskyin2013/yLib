#ifndef _YSHELL_H_
#define _YSHELL_H_

#ifdef __cplusplus
extern "C"{
#endif //   __cplusplus 

#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#ifdef __cplusplus
}
#endif //   __cplusplus 


#include <vector>
#include <string>



#include "ylog.h"

namespace yLib{

#define SHELL_CMDANDPARAM_MAX_NUM 9
#define Y_SHELL_LOC_CMD_LEN SHELL_CMDANDPARAM_MAX_NUM + 1
#define Y_SHELL_ARGV_LEN Y_SHELL_LOC_CMD_LEN
    class yShell{

        public:
        yShell();
        ~yShell();
        //Every param's length must be < 100
        int RunShellCommand(std::vector<std::string> & cmd);
        private:
        protected:
    };

}

#endif //_YSHELL_H_