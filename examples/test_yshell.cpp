/*
 * @Author: Sky
 * @Date: 2018-10-23 11:10:02
 * @LastEditors: Sky
 * @LastEditTime: 2018-10-23 11:10:02
 * @Description: 
 */

#include "ylib.hpp"


int main (int argc, char * argv[]){

    yLib::yShell shell;

    std::vector<std::string> cmd_vec;

    cmd_vec.push_back("/bin/ls");
    cmd_vec.push_back("ls");
    cmd_vec.push_back("-al");

    shell.RunShellCommand(cmd_vec);
    //shell.RunShellCommand(cmd_vec);
    //clear all vec memory

    std::vector<std::string>().swap(cmd_vec);

    cmd_vec.push_back("/bin/uname");
    cmd_vec.push_back("-a");
   // cmd_vec.push_back("test yshell class");

    shell.RunShellCommand(cmd_vec);


    return 0;
}