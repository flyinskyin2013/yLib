/*
Copyright (c) 2018 - 2021 flyinskyin2013 All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


/*
 * @Author: Sky
 * @Date: 2019-04-23 17:18:50
 * @LastEditors: Sky
 * @LastEditTime: 2021-09-13 14:59:29
 * @Description: 
 */

#include "utility/yshell.hpp"
#include "core/ylog.hpp"

#include <cstring>
#include <iostream>

YLIB_IMPLEMENT_CLASSINFO_CONTENT(yShell)


#if __linux__ || __linux
static int8_t __execute_impl_linux(const std::vector<YLIB_STD_STRING> & cmd, const std::vector<YLIB_STD_STRING> & args, const std::vector<YLIB_STD_STRING> &env, std::vector<YLIB_STD_STRING> & result) noexcept{

    YLIB_STD_CHAR ** parse_cmd_array = new YLIB_STD_CHAR * [cmd.size() + args.size() + 1];
    parse_cmd_array[cmd.size() + args.size()] = nullptr;    

    for(size_t _i = 0; _i < cmd.size(); _i++){

        parse_cmd_array[_i] = const_cast<char *>(cmd[_i].c_str());
    }

    for(size_t _i = 0; _i < args.size(); _i++){

        parse_cmd_array[_i + cmd.size()] = const_cast<char *>(args[_i].c_str());
    }


    YLIB_STD_CHAR ** parse_cmd_env_array = new YLIB_STD_CHAR * [env.size() + 1];
    parse_cmd_env_array[env.size()] = nullptr;    

    for(size_t _i = 0; _i < env.size(); _i++){

        parse_cmd_env_array[_i] = const_cast<char *>(env[_i].c_str());
    }
    parse_cmd_env_array[env.size()] = NULL;


    uint64_t result_line_buf_size = 4096;
    char * result_read_buffer = new (std::nothrow) char [4096];

    //we can call popen , but it can't set env.so we use uname execv and uname-pipe
    int _pipe_fd[2] = {-1, -1};

    if (0 > pipe(_pipe_fd)){

        yLib::yLog::E("create pipe failed. errno is %d", errno);
        
        return -1;
    }

    int pid, status;

    if ( 0 > ( pid = fork() ) ){
        
        yLib::yLog::E("fork  failed. errno is %d", errno);

        close(_pipe_fd[0]);
        close(_pipe_fd[1]);
        return -1;
    } 
    if( pid  > 0) {
        
        close(_pipe_fd[1]);//close pipe write 

        waitpid(pid,&status,0);

        if ( WIFEXITED(status) ){
            
            if ( WEXITSTATUS(status) != 0 ){

#ifndef YLIB_CODECOVERAGE_SKIP_CODE
                yLib::yLog::E("WEXITSTATUS is not 0, WEXITSTATUS is %d .", WEXITSTATUS(status));

                FILE * _ret_read_stream = fdopen(_pipe_fd[0], "r");
                while ( 1 && nullptr != result_read_buffer){
                    
                    memset(result_read_buffer, 0, result_line_buf_size + 1);
                    if ( NULL == fgets(result_read_buffer, result_line_buf_size, _ret_read_stream) ){

                        break;//have no charactor or error
                    }
                    else {
                        
                        int64_t _read_line_num = std::strlen(result_read_buffer);

                        if ('\n' == result_read_buffer[_read_line_num - 1]){

                            result_read_buffer[_read_line_num - 1] = 0x00;//get rid of '\n'
                            result.push_back(result_read_buffer);//push per line to result vec
                        }
                        else
                            result.push_back(result_read_buffer);//push per line to result vec


                        yLib::yLog::E("ErrorInfo is %s", result_read_buffer);
                    }
                }

                fclose(_ret_read_stream);//

                close(_pipe_fd[0]);//close pipe read
                //close(_pipe_fd[1]);
                return -1;
#endif //YLIB_CODECOVERAGE_SKIP_CODE
            }
            else{//command-run is ready

                FILE * _ret_read_stream = fdopen(_pipe_fd[0], "r");
                while ( 1 && nullptr != result_read_buffer){
                    
                    memset(result_read_buffer, 0, result_line_buf_size + 1);
                    if ( NULL == fgets(result_read_buffer, result_line_buf_size, _ret_read_stream) ){

                        break;//have no charactor or error
                    }
                    else {
                        
                        int64_t _read_line_num = std::strlen(result_read_buffer);

                        if ('\n' == result_read_buffer[_read_line_num - 1]){

                            result_read_buffer[_read_line_num - 1] = 0x00;//get rid of '\n'
                            result.push_back(result_read_buffer);//push per line to result vec
                        }
                        else
                            result.push_back(result_read_buffer);//push per line to result vec
                    }
                }

                fclose(_ret_read_stream);//
                close(_pipe_fd[0]);////close pipe read
                
            }
        }
        else{

            yLib::yLog::E("WIFEXITED false");
            close(_pipe_fd[0]);//close pipe read
            //close(_pipe_fd[1]);
            return -1;
        }

    }
    else{//pid == 0

        //build result pipe
        close(_pipe_fd[0]);//close pipe read 
        
        if (STDOUT_FILENO != _pipe_fd[1]){

            if ( STDOUT_FILENO != dup2(_pipe_fd[1], STDOUT_FILENO)){

                yLib::yLog::E("copy stdout to write-pipe failed. errno is %d", errno);
                close(_pipe_fd[1]);
                _exit(127);
            }
        }

        int _ret = 0;
        if ( 0 > (_ret = execve(parse_cmd_array[0], parse_cmd_array + 1, parse_cmd_env_array)) ){
            
            // yLib::yLog::E("execve failed.errno is %d", errno); 
            //we can't use yLib::yLog::X, because stdout is connected with _pipe_fd[1]
            std::string _e_msg = std::string("execve failed. errno is ") + std::to_string(errno);
            ::write(_pipe_fd[1], _e_msg.c_str(), _e_msg.length());

            close(_pipe_fd[1]);
            _exit(127);
        }

    }

    if (nullptr != result_read_buffer) delete [] result_read_buffer;
    if (nullptr != parse_cmd_array) delete [] parse_cmd_array;
    if (nullptr != parse_cmd_env_array) delete [] parse_cmd_env_array;
    return 0;
}
#endif //__linux__ || __linux


#ifdef _WIN32
static int8_t __execute_impl_windows(const std::vector<YLIB_STD_STRING> & cmd, const std::vector<YLIB_STD_STRING> & args, const std::vector<YLIB_STD_STRING> &env, std::vector<YLIB_STD_STRING> & result) noexcept{

    //https://docs.microsoft.com/zh-cn/previous-versions/windows/desktop/legacy/aa379560(v=vs.85)
    SECURITY_ATTRIBUTES _sa;
    _sa.bInheritHandle = TRUE;
    _sa.lpSecurityDescriptor = NULL; 
    _sa.nLength = sizeof(SECURITY_ATTRIBUTES); 

    HANDLE _pipe_rd_handle;
    HANDLE _pipe_wr_handle;

    if (!CreatePipe( &_pipe_rd_handle, &_pipe_rd_handle, &_sa, 8*1024)){

        LOGE("yShell")<<CONVERT_STR_TO_YLIB_STD_STRING(CreatePipe(): )<<GetLastError();
        return -1;
    }


	STARTUPINFO _startup_info;
	::memset(&_startup_info, 0 ,sizeof(_startup_info));
    _startup_info.hStdOutput = _pipe_rd_handle;
    _startup_info.hStdError = _pipe_rd_handle;

	_startup_info.cb = sizeof(_startup_info);
 
	PROCESS_INFORMATION _process_info;
	::memset(&_process_info, 0 ,sizeof(_process_info));
 
    //parse cmd
    YLIB_STD_CHAR * _cmd_parsed_buf = new YLIB_STD_CHAR [cmd.size()*256 + 1];
    ::memset(_cmd_parsed_buf, 0, sizeof(YLIB_STD_CHAR)*(cmd.size()*256+1));

    int _cmd_parsed_buf_id = 0;
    for(int _i = 0; _i < cmd.size(); _i++){

        ::memcpy(_cmd_parsed_buf + _cmd_parsed_buf_id, cmd[_i].c_str(), sizeof(YLIB_STD_CHAR)*cmd[_i].length());
        _cmd_parsed_buf_id += sizeof(YLIB_STD_CHAR)*cmd[_i].length();
        _cmd_parsed_buf[_cmd_parsed_buf_id] = ' ';
        _cmd_parsed_buf_id++;
    }


    //parse args
    YLIB_STD_CHAR * _args_parsed_buf = new YLIB_STD_CHAR [args.size()*256 + 1];
    ::memset(_args_parsed_buf, 0, sizeof(YLIB_STD_CHAR)*(args.size()*256+1));

    int _args_parsed_buf_id = 0;
    for(int _i = 0; _i < args.size(); _i++){

        ::memcpy(_args_parsed_buf+_args_parsed_buf_id, args[_i].c_str(), sizeof(YLIB_STD_CHAR)*args[_i].length());
        _args_parsed_buf_id += sizeof(YLIB_STD_CHAR)*args[_i].length();
        _args_parsed_buf[_args_parsed_buf_id] = ' ';
        _args_parsed_buf_id++;
    }

    //parse env
    const YLIB_STD_CHAR ** _env_parsed_buf = nullptr;
    if (env.size() > 1){

        const YLIB_STD_CHAR ** _env_parsed_buf = new const YLIB_STD_CHAR * [env.size() + 1];
        _env_parsed_buf[env.size()] = nullptr;

        int _env_parsed_buf_id = 0;
        for(int _i = 0; _i < env.size(); _i++){

            _env_parsed_buf[_i] = env[_i].c_str();
        }
    }


    if (!::CreateProcess(_cmd_parsed_buf, _args_parsed_buf, NULL, NULL, false, CREATE_NEW_CONSOLE, _env_parsed_buf, NULL, &_startup_info, &_process_info)){

        LOGE("yShell")<<CONVERT_STR_TO_YLIB_STD_STRING(CreateProcess(): )<<GetLastError();
        return -1;        
    }


    // Wait until child process exits.
    WaitForSingleObject( _process_info.hProcess, INFINITE );

    //get process stdout
    YLIB_STD_CHAR _get_char;
    DWORD _byte_read_num = 0;
    YLIB_STD_STRING _get_tmp_str;
    while(ReadFile(_pipe_rd_handle, &_get_char, 1, &_byte_read_num, NULL)){

        if (1 !=_byte_read_num){
            LOGE("yShell")<<CONVERT_STR_TO_YLIB_STD_STRING(ReadFile(): )<<GetLastError();
            break;
        }

        _get_tmp_str += _get_char;

        if (_get_char == CONVERT_CHAR_TO_YLIB_STD_CHAR('\n')){

            result.push_back(_get_tmp_str);
            _get_tmp_str = CONVERT_STR_TO_YLIB_STD_STRING("");
        }
    }

    // Close process and thread handles. 
    CloseHandle( _process_info.hProcess );
    CloseHandle( _process_info.hThread );

    CloseHandle( _pipe_rd_handle );
    CloseHandle( _pipe_wr_handle );


    delete [] _cmd_parsed_buf;
    delete [] _args_parsed_buf;
    delete [] _env_parsed_buf;

    return 0;
}
#endif //_WIN32

int8_t yLib::yShell::Execute(const std::vector<YLIB_STD_STRING> & cmd, const std::vector<YLIB_STD_STRING> & args, const std::vector<YLIB_STD_STRING> &env, std::vector<YLIB_STD_STRING> & result) noexcept{

    #ifdef _WIN32

    return __execute_impl_windows(cmd, args, env, result);
    #elif __linux__ || __linux

    return __execute_impl_linux(cmd, args, env, result);
    #elif __unix__ || __unix

    return -1;
    #endif //__unix__ || __unix
}

