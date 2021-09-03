/*
 * @Author: Sky
 * @Date: 2019-04-23 17:18:50
 * @LastEditors: Sky
 * @LastEditTime: 2021-08-31 16:22:43
 * @Description: 
 */

#include "utility/yshell.hpp"

#include <cstring>

YLIB_IMPLEMENT_CLASSINFO_CONTENT(yShell)

yLib::yShell::yShell() MACRO_INIT_YOBJECT_PROPERTY(yShell){

    parse_cmd_array = new char * [cmd_vec_size + 1];
    parse_cmd_env_array = new char * [cmd_env_vec_size + 1];
    result_read_buffer = new char [_result_line_buf_size + 1];

    memset(parse_cmd_array, 0, cmd_vec_size + 1);
    memset(parse_cmd_env_array, 0, cmd_env_vec_size + 1);
    memset(result_read_buffer, 0, _result_line_buf_size + 1);
}

yLib::yShell::~yShell(){

    delete []parse_cmd_array;
    delete []parse_cmd_env_array;
    delete []result_read_buffer;
}

#define SHELL_CMDANDPARAM_MAX_NUM 9
#define Y_SHELL_LOC_CMD_LEN SHELL_CMDANDPARAM_MAX_NUM + 1
#define Y_SHELL_ARGV_LEN Y_SHELL_LOC_CMD_LEN

int8_t yLib::yShell::RunShellCommandEx(std::vector<std::string> & cmd_, std::vector<std::string> &cmd_env_, std::vector<std::string> & cmd_result_){

    if (cmd_.size() > cmd_vec_size || cmd_env_.size() > cmd_env_vec_size){

        yLib::yLog::E("input cmd_'s size or cmd_env_' size is not valid.");
        return -1;//
    }

    for (auto _iter = cmd_.begin(); _iter != cmd_.end(); _iter ++){

        parse_cmd_array[_iter - cmd_.begin()] = (char *)_iter->c_str();
    }
    parse_cmd_array[cmd_.size()] = NULL;


    for (auto _iter = cmd_env_.begin(); _iter != cmd_env_.end(); _iter ++){

        parse_cmd_env_array[_iter - cmd_env_.begin()] = (char *)_iter->c_str();
    }
    parse_cmd_env_array[cmd_env_.size()] = NULL;


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
                
                yLib::yLog::E("WEXITSTATUS is not 0, WEXITSTATUS is %d .", WEXITSTATUS(status));

                FILE * _ret_read_stream = fdopen(_pipe_fd[0], "r");
                while ( 1 ){
                    
                    memset(result_read_buffer, 0, _result_line_buf_size + 1);
                    if ( NULL == fgets(result_read_buffer, _result_line_buf_size, _ret_read_stream) ){

                        break;//have no charactor or error
                    }
                    else {
                        
                        int64_t _read_line_num = std::strlen(result_read_buffer);

                        if ('\n' == result_read_buffer[_read_line_num - 1]){

                            result_read_buffer[_read_line_num - 1] = 0x00;//get rid of '\n'
                            cmd_result_.push_back(result_read_buffer);//push per line to result vec
                        }
                        else
                            cmd_result_.push_back(result_read_buffer);//push per line to result vec


                        yLib::yLog::E("ErrorInfo is %s", result_read_buffer);
                    }
                }

                fclose(_ret_read_stream);//

                close(_pipe_fd[0]);//close pipe read
                //close(_pipe_fd[1]);
                return -1;
            }
            else{//command-run is ready

                FILE * _ret_read_stream = fdopen(_pipe_fd[0], "r");
                while ( 1 ){
                    
                    memset(result_read_buffer, 0, _result_line_buf_size + 1);
                    if ( NULL == fgets(result_read_buffer, _result_line_buf_size, _ret_read_stream) ){

                        break;//have no charactor or error
                    }
                    else {
                        
                        int64_t _read_line_num = std::strlen(result_read_buffer);

                        if ('\n' == result_read_buffer[_read_line_num - 1]){

                            result_read_buffer[_read_line_num - 1] = 0x00;//get rid of '\n'
                            cmd_result_.push_back(result_read_buffer);//push per line to result vec
                        }
                        else
                            cmd_result_.push_back(result_read_buffer);//push per line to result vec
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

    return 0;
}


void yLib::yShell::ReconfigyShellBuffer(uint64_t cmd_vec_size_, uint64_t cmd_env_vec_size_, uint64_t _result_line_buf_size_){

    cmd_env_vec_size = cmd_env_vec_size_;
    cmd_vec_size = cmd_vec_size_;
    _result_line_buf_size = _result_line_buf_size_;

    delete []parse_cmd_array;
    delete []parse_cmd_env_array;
    delete []result_read_buffer;

    parse_cmd_array = new char * [cmd_vec_size + 1];
    parse_cmd_env_array = new char * [cmd_env_vec_size + 1];
    result_read_buffer = new char [_result_line_buf_size + 1];

    memset(parse_cmd_array, NULL, cmd_vec_size + 1);
    memset(parse_cmd_env_array, NULL, cmd_env_vec_size + 1);
    memset(result_read_buffer, 0, _result_line_buf_size + 1);
}