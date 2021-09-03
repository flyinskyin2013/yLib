/*
 * @Author: Sky
 * @Date: 2018-10-23 11:13:37
 * @LastEditors: Sky
 * @LastEditTime: 2021-08-31 16:22:28
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


#include "core/ycommon.hpp"
#include "ylog.hpp"

namespace yLib{

    /**
     *  @class yShell
     *  @brief This class can run our cmd in linux(it is based on popen/pclose).
     *  @warning We will support windows in the future.
     */
    class yShell MACRO_PUBLIC_INHERIT_YOBJECT{

        public:
        /**
         *  @fn    yShell()
         *  @brief Default constructor
         */
        yShell();

        /**
         *  @fn    ~yShell()
         *  @brief Default destructor
         */
        ~yShell();

        
        /*
            Default mode:
            cmd_'s size must be less than 20
            cmd_env_'s size must be less than 20
            _result_line_buf_size's size must be less than 1024.(_result_line_buf_size is max line-length of cmd_result)

            Extended mode:
            You can call SetUserModeConfig() reconfig them.
        */

        /**
         *  @fn      int8_t RunShellCommandEx(std::vector<std::string> & cmd_, std::vector<std::string> &cmd_env_, std::vector<std::string> & cmd_result_)
         *  @brief   run user's cmd with env and get the result of rum-cmd.
         * 
         *  @param   cmd_ we want to run, cmd_'s size must be less than 20.
         *  @param   cmd_env_ the env of cmd_, cmd_env_'s size must be less than 20.
         *  @param   cmd_result_ the result of cmd_, _result_line_buf_size's size must be less than 1024.(_result_line_buf_size is max line-length of cmd_result).
         * 
         *  @return The status of run-cmd
         *  @retval 0 run cmd ok.
         *  @retval -1 run cmd error.
         * 
         *  @warning 
         * 
         */
        int8_t RunShellCommandEx(std::vector<std::string> & cmd_, std::vector<std::string> &cmd_env_, std::vector<std::string> & cmd_result_);

        /**
         *  @fn      void ReconfigyShellBuffer(uint64_t cmd_vec_size_, uint64_t cmd_env_vec_size_, uint64_t _result_line_buf_size_)
         *  @brief   we can reconfig cmd_, cmd_env_, cmd_result_'s capcity.
         *  @see RunShellCommandEx 's param.
         *  @param   cmd_vec_size_ cmd-vec max size
         *  @param   cmd_env_vec_size_ cmd-env-vec max size
         *  @param   _result_line_buf_size_ result-line-buffer-vec max size
         * 
         * 
         *  @return None.
         * 
         *  @warning 
         */
        void ReconfigyShellBuffer(uint64_t cmd_vec_size_, uint64_t cmd_env_vec_size_, uint64_t _result_line_buf_size_);

        private:
        /** @var uint64_t cmd_vec_size
            @brief The cmd_ max size.
            @see RunShellCommand / RunShellCommandEx 's param
        */
        uint64_t cmd_vec_size = 20;

        /** @var uint64_t cmd_env_vec_size
            @brief The cmd-env max size.
            @see RunShellCommandEx 's param
        */
        uint64_t cmd_env_vec_size = 20;

        /** @var uint64_t _result_line_buf_size
            @brief The result-buffer max size.

            @see RunShellCommandEx 's param
        */
        uint64_t _result_line_buf_size = 1024;

        /** @var char * * parse_cmd_array
            @brief The pointer of cmd-buffer parsed.
            @see RunShellCommand / RunShellCommandEx 's param
        */
        char * * parse_cmd_array = nullptr;

        /** @var char * * parse_cmd_env_array
            @brief The pointer of cmd-env-buffer parsed
            @see RunShellCommandEx 's param
        */
        char * * parse_cmd_env_array = nullptr;

        /** @var char * result_read_buffer
            @brief The pointer of result-buffer
            @see RunShellCommandEx 's param
        */
        char * result_read_buffer = nullptr;
        
        protected:

        YLIB_DECLARE_CLASSINFO_CONTENT(yShell);
    };

}

#endif //__YLIB_UTILITY_YSHELL_HPP__