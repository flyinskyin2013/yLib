#include "yshell.hpp"

yLib::yShell::yShell(){


}

yLib::yShell::~yShell(){


}

int yLib::yShell::RunShellCommand(std::vector<std::string> & cmd){

    int pid;
    int status;
    //int cmd_param_num;
    // char (*loc_cmd)[100] = nullptr;
    // int num_arg;
    // char * (* argv) = nullptr; 

    char loc_cmd[Y_SHELL_LOC_CMD_LEN][100];
    char * argv[Y_SHELL_ARGV_LEN];

   // cmd_param_num = cmd.size();
    std::vector<std::string>::iterator it = cmd.begin();
    for ( ; it != cmd.end(); it++ ){

        memcpy(loc_cmd[it-cmd.begin()], it->c_str(), it->length());
        argv[it-cmd.begin()] = loc_cmd[it-cmd.begin()];
    }
    // execv  need (char *)'s array , the last elment must be null
    argv[it-cmd.begin()] = NULL;

    if ( 0 > ( pid = fork() ) ){
        
        perror("fork error!");
    } 
    if( pid  > 0) {
        
        waitpid(pid,&status,0);

        //sub-process returned,we must be release memory
        // delete [] loc_cmd;
        // delete [] argv;

        if ( WIFEXITED(status) ){
            
            if ( WEXITSTATUS(status) != 0 ){

                yLib::yLog::E("WEXITSTATUS is not 0, WEXITSTATUS is %d", WEXITSTATUS(status));
                return -1;
            }
        }
        else{

            yLib::yLog::E("WIFEXITED false");
            return -1;
        }
    }
    else{//pid == 0

        int ret = 0;
        if ( 0 > (ret = execv(argv[0],argv+1)) ){
            
            perror("call execv error, info:");
        }
        _exit(127);
    }
    return 0;
}