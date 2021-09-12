/*
 * @Author: Sky
 * @Date: 2018-10-23 11:13:37
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2021-09-12 12:24:18
 * @Description: 
 */

#ifndef __YLIB_UTILITY_YSHELL_HPP__
#define __YLIB_UTILITY_YSHELL_HPP__

#ifdef __cplusplus
extern "C"{
#endif //   __cplusplus 

#include <string.h>

#ifdef _WIN32

    #include <Windows.h>

#elif __linux__ || __linux

    #include <sys/types.h>
    #include <unistd.h>
    #include <sys/wait.h>

#elif __unix__ || __unix

#endif //__unix__ || __unix


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
     *  @brief This class can run our cmd in linux(unmae-pipe/fork/execve) and in windows(CreateProcess/CreatePipe).
     *  @warning 
     */
    class __YLIB_CLASS_DECLSPEC__ yShell MACRO_PUBLIC_INHERIT_YOBJECT{

        public:

        /**
         *  @fn      static int8_t Execute(const std::vector<YLIB_STD_STRING> & cmd, const std::vector<YLIB_STD_STRING> & args, const std::vector<YLIB_STD_STRING> &env, std::vector<YLIB_STD_STRING> & result) noexcept
         *  @brief   run user's cmd with env and args, then get the result of rum-cmd.
         * 
         *  @param   cmd we want to run
         *  @param   args the args of cmd
         *  @param   cmd_env the env of cmd
         *  @param   cmd_result the result of cmd
         * 
         *  @return The status of run-cmd
         *  @retval 0 run cmd ok.
         *  @retval -1 run cmd error.
         * 
         *  @warning 
         * 
         */
        static int8_t Execute(const std::vector<YLIB_STD_STRING> & cmd, const std::vector<YLIB_STD_STRING> & args, const std::vector<YLIB_STD_STRING> &env, std::vector<YLIB_STD_STRING> & result) noexcept;

        private:
        
        protected:

        YLIB_DECLARE_CLASSINFO_CONTENT(yShell);
    };

}

#endif //__YLIB_UTILITY_YSHELL_HPP__