/*
 * @Author: Sky
 * @Date: 2018-10-23 11:13:37
 * @LastEditors: Sky
 * @LastEditTime: 2019-10-28 17:18:01
 * @Description: 
 */

#ifndef _YLIB_CORE_YSHELL_HPP_
#define _YLIB_CORE_YSHELL_HPP_

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


#include "ycommon.hpp"
#include "ylog.hpp"

namespace yLib{

#define SHELL_CMDANDPARAM_MAX_NUM 9
#define Y_SHELL_LOC_CMD_LEN SHELL_CMDANDPARAM_MAX_NUM + 1
#define Y_SHELL_ARGV_LEN Y_SHELL_LOC_CMD_LEN
    class yShell MACRO_PUBLIC_INHERIT_YOBJECT{

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