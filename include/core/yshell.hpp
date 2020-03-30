/*
 * @Author: Sky
 * @Date: 2018-10-23 11:13:37
 * @LastEditors: Sky
 * @LastEditTime: 2020-03-26 16:56:36
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
#include <cstdint>


#include "ycommon.hpp"
#include "ylog.hpp"

namespace yLib{


    class yShell MACRO_PUBLIC_INHERIT_YOBJECT{

        public:
        yShell();
        ~yShell();
        //Every param's length must be < 100
        __YLIB_DEPRECATED_ATTRIBUTE__ int RunShellCommand(std::vector<std::string> & cmd);
        
        int8_t RunShellCommandEx(std::vector<std::string> & cmd_, std::vector<std::string> & cmd_result_);
        private:
        static const 
        protected:
    };

}

#endif //_YSHELL_H_