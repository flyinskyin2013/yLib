/*
 * @Author: Sky
 * @Date: 2018-08-20 11:25:48
 * @LastEditors: Sky
 * @LastEditTime: 2020-09-07 15:10:45
 * @Description: 
 */

//========================================================================
//For c header file
//========================================================================
#ifdef __cplusplus
extern "C"{
#endif //__cplusplus

#include <signal.h>
#include <stddef.h>
#include <unistd.h>
#include <getopt.h>
#include <errno.h>

#ifdef __cplusplus
}
#endif //__cplusplus




//========================================================================
//For c++ header file
//========================================================================
#include <cstdint>
#include <cstring>



//========================================================================
//For customer header file
//========================================================================
#include "ylib.hpp"
#include "ystart.hpp"


/**
 * @description: 
 * Our default handler of signal.
 * 
 * @param {type} 
 * sig_ : The num of signal.
 * 
 * @return {type} 
 * void
 */
static void yDefaultSignalHandler(int sig_){

    yLib::yLog::I("Default process sig, sig num is %d", sig_);
    // TODO
}

/**
 * @description: 
 * Register handler of signal.
 * 
 * @param {type} 
 * void
 * 
 * @return {type} 
 * int8_t: 0 success. -1 failed.
 * 
 */
static int8_t ySignalProcess(void){

    struct sigaction _sig_act;

    if ( 0 > ::sigemptyset(&_sig_act.sa_mask) ){

        yLib::yLog::E("sigemptyset() failed.");
        return -1;
    }
    _sig_act.sa_flags = 0;
    _sig_act.sa_handler = &yDefaultSignalHandler;

    //register signal
    //SIGPIPE will exit program,Defaultly,When socket disconnected.
    if ( 0 > ::sigaction(SIGPIPE, &_sig_act, NULL) ){

        yLib::yLog::E("sigaction() register SIGPIPE failed. errno is %d", errno);
        return -1;
    }

    //SIGPIPE will exit program,Defaultly,When process-session go wrong.
    if ( 0 > ::sigaction(SIGHUP, &_sig_act, NULL)){

        yLib::yLog::E("sigaction() register SIGHUP failed. errno is %d", errno);
        return -1;
    }
    
    //Deal with ctrl-C
    if ( 0 > ::sigaction(SIGINT, &_sig_act, NULL)){

        yLib::yLog::E("sigaction() register SIGINT failed. errno is %d", errno);
        return -1;
    }

    //Deal with kill 
    if ( 0 > ::sigaction(SIGTERM, &_sig_act, NULL)){

        yLib::yLog::E("sigaction() register SIGTERM failed. errno is %d", errno);
        return -1;
    }

    //Create core file and program exit
    if ( 0 > ::sigaction(SIGQUIT, &_sig_act, NULL)){

        yLib::yLog::E("sigaction() register SIGQUIT failed. errno is %d", errno);
        return -1;
    }

    //Timer is timeout and program exit
    if ( 0 > ::sigaction(SIGALRM, &_sig_act, NULL)){

        yLib::yLog::E("sigaction() register SIGALRM failed. errno is %d", errno);
        return -1;
    }

    // TODO

    return 0;
}

/**
 * @description: 
 * Example Usage.
 * 
 * @param {type} 
 * void
 *
 * @return {type} 
 * void
 * 
 */
static void yUsage(void){

    yLib::yLog::I("Usage: default usage example.");
}

/**
 * @description: 
 * Parse the arguments of main().
 * 
 * @param {type} 
 * argc_: The num pf argument.
 * argv_: The pointer-array of argument.
 * enable_long_opt_: Is enable long-option' support.
 * 
 * @return {type} 
 * int8_t: 0 success. -1 failed.
 * 
 */
static int8_t yMainArgumentParse(int argc_, char * const argv_[], bool enable_long_opt_ = true){

    //declare this var.
    extern char * optarg;
    extern int optind, opterr, optopt;

    int _opt_idx;
    int _longoptindex;

    const char * _opt_str = "Hhet";

    //for getopt_long
    struct option _long_options[] = {

        {"help", no_argument, NULL, 'h'}, //help 0
        {"HELP", no_argument, NULL, 'H'}, //help 1

        //example required_argument
        //./test_ymain --example abc
        //./test_ymain --example=abc 
        {"example", required_argument, NULL, 'e'}, 

        //test optional_argument
        //./test_ymain --test=abc
        //./test_ymain
        {"test", optional_argument, NULL, 't'},
        {0, 0, 0, 0}
    };
    
    if ( enable_long_opt_ ){ //getopt_long
        
        while( -1 != (_opt_idx = getopt_long(argc_, argv_, _opt_str, _long_options, &_longoptindex)) ){
            
            //optarg is param
            //optind is param's array idx
            
            switch (_opt_idx)
            {
                case 'h':{

                    yUsage();
                    break;
                }
                case 'H':{

                    yUsage();
                    break;
                }
                case 'e':{
                    
                    //yLib::yLog::I("example(required_argument): _opt_idx %c, optarg %s, optind %d, opterr %d, optopt %d", _opt_idx, optarg, optind, opterr, optopt);
                    if (0 == std::strcmp(optarg, "example1")){

                        yLib::yLog::I("example(required_argument): example1.");
                    }
                    else if (0 == std::strcmp(optarg, "example2")){

                        yLib::yLog::I("example(required_argument): example2.");
                    }
                    else {//unkown example

                        yLib::yLog::I("example(required_argument): unkown example.");
                        return -1;
                    }
                    break;
                }
                case 't':{
                    
                    //yLib::yLog::I("test(optional_argument): _opt_idx %c, optarg %s, optind %d, opterr %d, optopt %d", _opt_idx, optarg, optind, opterr, optopt);
                    
                    if (nullptr == optarg){//default test

                        yLib::yLog::I("test(optional_argument): default test.");
                    }
                    else if (0 == std::strcmp(optarg, "test1")){//

                        yLib::yLog::I("test(optional_argument): test1.");
                    }
                    else {//unkown test

                        yLib::yLog::I("test(optional_argument): unkown test.");
                        return -1;
                    }
                    break;
                }
                
                // TODO
                default:{
                    
                    yLib::yLog::E("Invalid argument, More details, see usage.");
                    yUsage();//error param
                    return -1;
                    break;
                }
            }
        }
    }
    else //getopt
    {
        
        while( -1 != (_opt_idx = getopt(argc_, argv_, _opt_str)) ){
            
            //optarg is param
            //optind is param's array idx
            
            switch (_opt_idx)
            {
                case 'h':{

                    yUsage();
                    break;
                }
                case 'H':{

                    yUsage();
                    break;
                }
                case 'e':{
                    
                    if (0 == std::strcmp(optarg, "example1")){

                        yLib::yLog::I("example(required_argument): example1.");
                    }
                    else {//unkown example

                        yLib::yLog::I("example(required_argument): unkown example.");
                        return -1;
                    }
                    break;
                }
                case 't':{

                    if (0 == std::strcmp(optarg, "test1")){//

                        yLib::yLog::I("test(optional_argument): test1.");
                    }
                    else {//unkown test

                        yLib::yLog::I("test(optional_argument): unkown test.");
                        return -1;
                    }
                    break;
                }

                // TODO
                default:{
                    
                    yLib::yLog::E("Invalid argument, More details, see usage.");
                    yUsage();//error param
                    return -1;
                    break;
                }
            }
        }
        
    }
    
    return 0;
}

/**
 * @description: main function. 
 * @param {type} 
 * @return {type} 
 */
int main(int argc, char * argv[]){

    //deal with param
    if ( 0 > yMainArgumentParse(argc, argv) ){

        yLib::yLog::E("yMainArgumentParse(): Parse arguments failed.");
        return -1;
    }

    //deal with signal
    if ( 0 > ySignalProcess()){

        yLib::yLog::E("ySignalProcess(): Register signal's handler failed.");
        return -1;
    }

    //guard user process
    //fork
    int _pid = ::fork();

    if ( 0 > _pid ){

        yLib::yLog::E("Create guard process failed.");
        ::exit(EXIT_FAILURE);
    }
    else if ( _pid > 0 ){ //parent

        int _status = 0;
        int _ret = 0;

        if ( 0 > (_ret = ::waitpid(_pid, &_status, 0)) ){
            
            yLib::yLog::E("waitpid failed in guard-process. errno is %d", errno);
            ::exit(EXIT_FAILURE);
        }
        /*
        If wstatus is not NULL, wait() and waitpid() store  status  information
        in  the int to which it points.  This integer can be inspected with the
        following macros (which take the integer itself as an argument,  not  a
        pointer to it, as is done in wait() and waitpid()!):

        WIFEXITED(wstatus)
                returns true if the child terminated normally, that is, by call‐
                ing exit(3) or _exit(2), or by returning from main().

        WEXITSTATUS(wstatus)
                returns the exit status of the  child.   This  consists  of  the
                least  significant  8 bits of the status argument that the child
                specified in a call to exit(3) or _exit(2) or  as  the  argument
                for a return statement in main().  This macro should be employed
                only if WIFEXITED returned true.

        WIFSIGNALED(wstatus)
                returns true if the child process was terminated by a signal.

        WTERMSIG(wstatus)
                returns the number of the signal that caused the  child  process
                to terminate.  This macro should be employed only if WIFSIGNALED
                returned true.

        WCOREDUMP(wstatus)
                returns true if the child produced  a  core  dump.   This  macro
                should be employed only if WIFSIGNALED returned true.

                This macro is not specified in POSIX.1-2001 and is not available
                on some UNIX implementations  (e.g.,  AIX,  SunOS).   Therefore,
                enclose its use inside #ifdef WCOREDUMP ... #endif.

        WIFSTOPPED(wstatus)
                returns  true  if the child process was stopped by delivery of a
                signal; this is possible only if the call was  done  using  WUN‐
                TRACED or when the child is being traced (see ptrace(2)).

        WSTOPSIG(wstatus)
                returns the number of the signal which caused the child to stop.
                This macro should be employed only if WIFSTOPPED returned true.

        WIFCONTINUED(wstatus)
                (since Linux 2.6.10) returns  true  if  the  child  process  was
                resumed by delivery of SIGCONT.

        */
       if ( _ret == _pid ){//deal with our sub-process.
           
           //sub process exit normally
           if ( WIFEXITED(_status) ){
               
               std::string _msg = "Subprocess exit value is " + std::to_string(WEXITSTATUS(_status));
               yLib::yLog::I(_msg);
           }
           else{
               
               yLib::yLog::E("WIFEXITED return false");
           }

            //sub process exit by a signal
           if ( WIFSIGNALED(_status) ){

               std::string _msg = "Subprocess terminate by a signal, signal num is "+ std::to_string(WTERMSIG(_status));
               yLib::yLog::I(_msg);
           }
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
        if ( 0 > yMainContext<yStartParam>::RunMainContext((const MainContextCallBack)ystart_test, param) ){

            std::string _msg = "Subprocess exit abnormally.";
            yLib::yLog::E(_msg);
            ::_exit(EXIT_FAILURE);
        }

        std::string _msg = "Subprocess exit normally.";
        yLib::yLog::E(_msg);
        ::_exit(EXIT_SUCCESS);
    }
    
    return 0;
}