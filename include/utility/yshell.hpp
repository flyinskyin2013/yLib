/*
 * @Author: Sky
 * @Date: 2018-10-23 11:13:37
 * @LastEditors: Sky
 * @LastEditTime: 2020-07-14 18:21:10
 * @Description: 
 */

#ifndef __YLIB_UTILITY_YSHELL_HPP__
#define __YLIB_UTILITY_YSHELL_HPP__

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
        /*
            Default mode:
            cmd_'s size must be less than 20
            cmd_env_'s size must be less than 20
            _result_line_buf_size's size must be less than 1024.(_result_line_buf_size is max line-length of cmd_result)

            Extended mode:
            You can call SetUserModeConfig() reconfig them.
        */
        int8_t RunShellCommandEx(std::vector<std::string> & cmd_, std::vector<std::string> &cmd_env_, std::vector<std::string> & cmd_result_);

        void ReconfigyShellBuffer(uint64_t cmd_vec_size_, uint64_t cmd_env_vec_size_, uint64_t _result_line_buf_size_);

        private:
        
        uint64_t cmd_vec_size = 20;
        uint64_t cmd_env_vec_size = 20;
        uint64_t _result_line_buf_size = 1024;

        char * * parse_cmd_array = nullptr;
        char * * parse_cmd_env_array = nullptr;
        char * result_read_buffer = nullptr;
        protected:
    };

}

#endif //__YLIB_UTILITY_YSHELL_HPP__