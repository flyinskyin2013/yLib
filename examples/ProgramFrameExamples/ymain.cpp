/*
 * @Author: Sky
 * @Date: 2018-08-20 11:25:48
 * @LastEditors: Sky
 * @LastEditTime: 2020-04-14 15:10:40
 * @Description: 
 */


#ifdef __cplusplus
extern "C"{
#endif //__cplusplus

#include <signal.h>
#include <stddef.h>
#include <unistd.h>
#include <getopt.h>

#ifdef __cplusplus
}
#endif //__cplusplus

#include "ylib.hpp"
#include "ystart.hpp"


void ySignalHandler(int sig){

    yLib::yLog::I("process sig,sig num is %d", sig);
    // TODO
}


int ySignalProcess(void){

    struct sigaction act;

    ::sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    act.sa_handler = &ySignalHandler;

    //register signal
    //SIGPIPE will exit program,Defaultly,When socket disconnected.
    ::sigaction(SIGPIPE, &act, NULL);
    //SIGPIPE will exit program,Defaultly,When process-session go wrong.
    ::sigaction(SIGHUP, &act, NULL);
    //Deal with ctrl-C
    ::sigaction(SIGINT, &act, NULL);
    //Deal with kill 
    ::sigaction(SIGTERM, &act, NULL);
    //Create core file and program exit
    ::sigaction(SIGQUIT, &act, NULL);
    //Timer is timeout and program exit
    ::sigaction(SIGALRM, &act, NULL);

    // TODO

    return 0;
}

void yUsage(void){

    yLib::yLog::I("Usage:");
}

int yOptionProcess(int argc, char * const argv[]){

    bool enable_long_opt = true;
    extern char *optarg;
    extern int optind, opterr, optopt;
    int opt_idx;
    int longoptindex;

    const char * opt_str = "Hh";
    //for getopt_long
    static struct option long_options[] = {
        {"Help", required_argument, NULL, 'H'}, 
        {"help",  no_argument,       NULL, 'h'},
        {"HELP", optional_argument, NULL, 'h'},
        {0, 0, 0, 0}
    };
    
    if ( enable_long_opt ){ //getopt_long
        
        while( -1 != (opt_idx = getopt_long(argc, argv, opt_str, long_options, &longoptindex)) ){
            
            //optarg is param
            //optind is param's array idx
            
            switch (opt_idx)
            {
                case 'h':
                    yUsage();
                    break;
                case 'H':
                    yUsage();
                    break;
                // TODO
                default:
                    yLib::yLog::I("Param error,More details,see usage.");
                    yUsage();//error param
                    break;
            }
        }
    }
    else //getopt
    {
        
        while( -1 != (opt_idx = getopt(argc, argv, opt_str)) ){
            
            //optarg is param
            //optind is param's array idx
            
            switch (opt_idx)
            {
                case 'h':
                    yUsage();
                    break;
                case 'H':
                    yUsage();
                    break;
                // TODO
                default:
                    yLib::yLog::I("Param error,More details,see usage.");
                    yUsage();//error param
                    break;
            }
        }
        
    }
    

    return 0;
}

int main(int argc, char * argv[]){

    //deal with param
    yOptionProcess(argc, argv);
    //deal with signal
    ySignalProcess();

    //guard user process
    //fork
    int pid = ::fork();

    if ( 0 > pid ){

        yLib::yLog::E("Create guard process failed.");
        exit(EXIT_FAILURE);
    }
    else if ( pid > 0 ){ //parent

        int status = 0;
        int ret = 0;

        ret = ::waitpid(pid, &status, 0);
        /*
        WIFEXITED(status)如果子进程正常结束则为非0值。

        WEXITSTATUS(status)取得子进程exit()返回的结束代码，一般会先用WIFEXITED 来判断是否正常结束才能使用此宏。

        WIFSIGNALED(status)如果子进程是因为信号而结束则此宏值为真

        WTERMSIG(status)取得子进程因信号而中止的信号代码，一般会先用WIFSIGNALED 来判断后才使用此宏。

        WIFSTOPPED(status)如果子进程处于暂停执行情况则此宏值为真。一般只有使用WUNTRACED 时才会有此情况。

        WSTOPSIG(status)取得引发子进程暂停的信号代码
        */
       if ( ret == pid ){
           
           //sub process exit normally
           if ( WIFEXITED(status) ){
               
               std::string msg = "Subprocess exit value is "+ std::to_string(WEXITSTATUS(status));
               yLib::yLog::I(msg.c_str());
           }
           else{
               yLib::yLog::E("WIFEXITED return false");
           }
            //sub process exit by a signal
           if ( WIFSIGNALED(status) ){

               std::string msg = "Subprocess terminate by a signal, signal num is "+ std::to_string(WTERMSIG(status));
               yLib::yLog::I(msg.c_str());
           }
       }
       else{
           yLib::yLog::E("Waitpid Error");
       }
    }
    else{ //child
        
        //entrance 
        //old op
        // yStartParam param;
        // ystart(param);


        //new op
        //If user want to use this interface, they must define MainContextCallBack and param firstly by themselves.
        yStartParam param;
        yMainContext<yStartParam>::RunMainContext((const MainContextCallBack)ystart_test, param);
    }
    
    return 0;
}