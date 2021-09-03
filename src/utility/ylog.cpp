/*
 * @Author: Sky
 * @Date: 2019-07-04 11:28:53
 * @LastEditors: Sky
 * @LastEditTime: 2021-09-03 16:47:59
 * @Description: 
 */

#include "utility/ylog.hpp"

#include <iomanip>
#include <assert.h>

#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include <unistd.h>
#include <sys/syscall.h>   /* For SYS_xxx definitions */
#include <time.h>


#ifdef __cplusplus
}
#endif//__cplusplus

yLib::EnableyLogFileParam yLib::yLogTagProperty::file_param;

const static char * g_log_severity_str[] = {
    "ALL",
    "TRACE",
    "DEBUG",
    "INFO ",
    "WARN ",
    "ERROR",
    "FATAL"
};

static int  GetThreadId() {

#if defined(linux) || defined(__linux) || defined(__linux__)

    return syscall(__NR_gettid);

#elif defined(WIN32) || defined(_WIN32) || defined(__WIN32__)

  return GetCurrentThreadId();

#else
    return -1;
#endif  // OS_LINUX

}

yLib::yLogMessage::yLogMessage(const std::string & log_tag, yLib::yLogSeverity log_severity, const std::string &file_name, int file_line, bool newline) noexcept
:is_add_newline(newline)
{
    msg_data = std::unique_ptr<yLogMessageData> (new (std::nothrow) yLogMessageData());
    if (nullptr == msg_data)
        return;

    //reset yLogStream
    msg_data->log_stream.fill(0);

    // save info
    clock_gettime(CLOCK_REALTIME, &msg_data->time_spec);
    localtime_r(&msg_data->time_spec.tv_sec, &msg_data->tm_time);
    msg_data->thread_id = GetThreadId();
    msg_data->log_tag = log_tag;
    msg_data->log_severity = log_severity;
    msg_data->file_name = file_name;
    msg_data->file_line = file_line;

    // set msg header
    msg_data->log_stream<<'[' \
                        <<' ' \
                        <<g_log_severity_str[log_severity] \
                        <<' ' \
                        <<std::setfill('0') \
                        << std::setw(4) << 1900 + msg_data->tm_time.tm_year \
                        <<'-' \
                        << std::setw(2) << (1 + msg_data->tm_time.tm_mon) \
                        <<'-' \
                        << std::setw(2) << msg_data->tm_time.tm_mday \
                        <<' ' \
                        << std::setw(2) << msg_data->tm_time.tm_hour \
                        <<':' \
                        << std::setw(2) << msg_data->tm_time.tm_min \
                        <<':' \
                        << std::setw(2) << msg_data->tm_time.tm_sec \
                        <<'.' \
                        <<std::setfill(' ') \
                        <<std::setw(6) << (int)(msg_data->time_spec.tv_nsec*0.001) \
                        <<' ' \
                        << std::setw(9) << msg_data->thread_id \
                        <<' ';

    if (msg_data->log_tag != ""){

        msg_data->log_stream<<msg_data->log_tag \
                            <<' ';
    }
    
    msg_data->log_stream<<']' \
                        <<' ';

    msg_data->log_prefix_len = msg_data->log_stream.stream_buf.buf_cur_ptr() - msg_data->log_stream.stream_buf.buf_base_ptr();
}

yLib::yLogMessage::~yLogMessage(){
    
    if (is_add_newline)
        msg_data->log_stream<<std::endl;//set '\n'


    if (msg_data->log_stream.stream_buf.is_overflow){
        
        std::cout<<"yLogStreamBuf(): buf-overflow, input-string will be truncated by size of YLIB_YLOGMESSAGE_MAX_LEN"<<std::endl;
        *(msg_data->log_stream.stream_buf.buf_cur_ptr() - 1) = '\n';
    }


    yLog::yLog::Log(msg_data->log_tag, msg_data->log_severity, std::string(msg_data->msg_buf,msg_data->log_stream.stream_buf.buf_cur_ptr() - msg_data->log_stream.stream_buf.buf_base_ptr()));
}

yLib::yLogDestination * yLib::yLogDestination::instance = nullptr;
yLib::yLogDestination * yLib::yLogDestination::CreateInstance(std::shared_ptr<std::unordered_map<std::string, yLogTagProperty>> tag_prop_map) noexcept{

    if(nullptr == instance)
        instance = new(std::nothrow)yLogDestination(tag_prop_map);
    
    return instance;
}

/************************************************************************************************/
/*this code copy and modified from https://github.com/google/glog/blob/master/src/logging.cc start*/
enum LogColor {
  COLOR_DEFAULT,
  COLOR_RED,
  COLOR_GREEN,
  COLOR_YELLOW
};

static LogColor SeverityToColor(yLib::yLogSeverity severity) {
    
  assert(severity >= yLib::LOG_ALL && severity < yLib::LOG_FATAL);

  LogColor color = COLOR_DEFAULT;
  switch (severity) {

    case yLib::LOG_INFO:
        color = COLOR_GREEN;
        break;
    case yLib::LOG_DEBUG:
    case yLib::LOG_TRACE:
    case yLib::LOG_ALL:
        color = COLOR_DEFAULT;
        break;
    case yLib::LOG_WARN:
        color = COLOR_YELLOW;
        break;
    case yLib::LOG_ERROR:
    case yLib::LOG_FATAL:
        color = COLOR_RED;
        break;
    default:
        std::cout<<"yLog: Convert severity to color failed."<<std::endl;
        break;
  }
  return color;
}

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
// Returns the character attribute for the given color.
static WORD GetColorAttribute(LogColor color) {
  switch (color) {
    case COLOR_RED:    return FOREGROUND_RED;
    case COLOR_GREEN:  return FOREGROUND_GREEN;
    case COLOR_YELLOW: return FOREGROUND_RED | FOREGROUND_GREEN;
    default:           return 0;
  }
}
#else
// Returns the ANSI color code for the given color.
static const char* GetAnsiColorCode(LogColor color) {
  switch (color) {
    case COLOR_RED:     return "1";
    case COLOR_GREEN:   return "2";
    case COLOR_YELLOW:  return "3";
    case COLOR_DEFAULT:  return "";
  }
  return NULL;  // stop warning about return type.
}
#endif  // OS_WINDOWS

static void ColoredWriteToStderr(yLib::yLogSeverity severity,
                                 const char* message, size_t len) {
  const LogColor color = SeverityToColor(severity);

  // Avoid using cerr from this module since we may get called during
  // exit code, and cerr may be partially or fully destroyed by then.
  if (COLOR_DEFAULT == color) {
    fwrite(message, len, 1, stderr);
    return;
  }
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
  const HANDLE stderr_handle = GetStdHandle(STD_ERROR_HANDLE);

  // Gets the current text color.
  CONSOLE_SCREEN_BUFFER_INFO buffer_info;
  GetConsoleScreenBufferInfo(stderr_handle, &buffer_info);
  const WORD old_color_attrs = buffer_info.wAttributes;

  // We need to flush the stream buffers into the console before each
  // SetConsoleTextAttribute call lest it affect the text that is already
  // printed but has not yet reached the console.
  fflush(stderr);
  SetConsoleTextAttribute(stderr_handle,
                          GetColorAttribute(color) | FOREGROUND_INTENSITY);
  fwrite(message, len, 1, stderr);
  fflush(stderr);
  // Restores the text color.
  SetConsoleTextAttribute(stderr_handle, old_color_attrs);
#else
  fprintf(stderr, "\033[0;3%sm", GetAnsiColorCode(color));
  fwrite(message, len, 1, stderr);
  fprintf(stderr, "\033[m");  // Resets the terminal to default.
#endif  // OS_WINDOWS
}
/*this code copy and modified from https://github.com/google/glog/blob/master/src/logging.cc end*/
/************************************************************************************************/
yLib::yLogDestination::yLogDestination(std::shared_ptr<std::unordered_map<std::string, yLogTagProperty>> tag_prop_map)
:log_file(tag_prop_map)
{
    this->tag_prop_map = tag_prop_map;
}

void yLib::yLogDestination::LogToStdIO(yLib::yLogSeverity severity, const char* message, size_t message_len) noexcept
{
    ColoredWriteToStderr(severity, message, message_len);
}


void yLib::yLogDestination::LogToFile(const char* message, size_t message_len, bool flush, const std::string &tag) noexcept
{
    log_file.write(message, message_len, flush, tag);
}


yLib::yLogFile::yLogFile(std::shared_ptr<std::unordered_map<std::string, yLogTagProperty>> tag_prop_map)
{
    this->tag_prop_map = tag_prop_map;
    
    write_thread_ptr = std::unique_ptr<std::thread>(new (std::nothrow) std::thread(&yLib::yLogFile::write_thread_func, this));
}
yLib::yLogFile::~yLogFile(){

    write_thread_is_continue.store(false);
    if (write_thread_ptr != nullptr)
        if (write_thread_ptr->joinable())
            write_thread_ptr->join();

    flush_log_file();

    for(auto _iter:log_file_handle_map){

        _iter.second.second->close();//close handle
    }
}

void yLib::yLogFile::write(const char* message, size_t message_len, bool flush, const std::string &tag){

    is_flush.store(flush);

    while(!write_thread_is_ready.load()){//write-log-thread is not ready, waiting ... ...

        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }

    if (write_thread_is_continue.load()){

        std::unique_lock<std::mutex> _lck(file_cache_queue_mtx);
        file_cache_queue.push(std::make_pair(tag, std::string(message, message_len)));
    }
}
void yLib::yLogFile::flush_log_file(void){

    for(auto _iter:this->log_file_handle_map){
        
        _iter.second.second->flush();
    }
};
bool yLib::yLogFile::create_log_file(void){

    std::string & _file_dir = yLib::yLogTagProperty::file_param.file_dir;
    std::string & _file_base_name = yLib::yLogTagProperty::file_param.file_base_name;
    if ("" != _file_dir) { //

        #if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
            const char& back_char = _file_dir.back();
            _file_dir = (back_char == '\\') ? _file_dir : _file_dir + "\\";
        #else
            const char& back_char = _file_dir.back();
            _file_dir = (back_char == '/') ? _file_dir : _file_dir + "/";
        #endif
    } 
    else{

        _file_dir = "./";
    }

    if (_file_base_name == "")
        _file_base_name = "LogFile";

    if (-1 == access(_file_dir.c_str(), F_OK)){
        
        std::string _err_msg = "file dir(" + _file_dir +  ") don't exist.\n";
        ColoredWriteToStderr(yLib::LOG_ERROR, _err_msg.c_str(), _err_msg.length());
        return false;
    }

    std::string _log_file_path = _file_dir + _file_base_name;

    auto _open_log_file = [this](const std::string &tag, const std::string & full_file_name){

        if (this->log_file_handle_map.count(tag) == 0){//this tag don't have log-file-handle

            std::shared_ptr<std::ofstream> _out_file = std::shared_ptr<std::ofstream>(new (std::nothrow) std::ofstream());
            _out_file->open(full_file_name, std::ofstream::out | std::ofstream::app);

            if (!_out_file->is_open()){
                
                std::string _err_msg = "create file(" + full_file_name +  ") failed.\n";
                ColoredWriteToStderr(yLib::LOG_ERROR, _err_msg.c_str(), _err_msg.length());
                return false;
            }
            
            this->log_file_handle_map.insert(std::make_pair(tag, std::make_pair(full_file_name, _out_file)));
        }

        return true;
    };

    // multi log file feature
    if(yLib::yLogTagProperty::file_param.multi_log_file){

        for(auto _iter:*tag_prop_map){
            
            if (_iter.first != ""){//not default log file

                std::string _full_file_name = _file_base_name + "_" + _iter.first + ".log";
                if (!_open_log_file(_iter.first ,_full_file_name))
                    return false;
            }

        }
    }

    // create default log-file
    std::string _full_file_name = _file_base_name + ".log";
    if (!_open_log_file("", _full_file_name))
        return false;
    
    
    return true;
}

void yLib::yLogFile::write_thread_func(void){

    //set timeout point
    clock_gettime(CLOCK_REALTIME, &old_time_spec);

    // create log-file
    if(!create_log_file()){

        write_thread_is_continue.store(false);
        return;
    }

    auto _check_file_max_size = [](std::shared_ptr<std::ofstream> file){

        uint64_t _cur_log_file_size = file->tellp();

        if (_cur_log_file_size >= yLib::yLogTagProperty::file_param.log_file_max_size)
            return true;//the size-log-file is max size, then we should backup it.
        
        return false;
    };

    auto _backup_file = [=](const std::string &tag){

        // check the oldest backup-file
        {
            std::string _the_oldest_bak_file_name = this->log_file_handle_map[tag].first \
                                                + ".bak" + std::to_string(yLib::yLogTagProperty::file_param.log_file_max_backup_num);
            if (-1 != access(_the_oldest_bak_file_name.c_str(), F_OK)){ //if this file exist, we remove it
                
                if (0 > remove(_the_oldest_bak_file_name.c_str())){
                    
                    std::string _err_msg_str = "remove this oldest-log-bak-file("+_the_oldest_bak_file_name+")failed.";
                    perror(_err_msg_str.c_str());
                    return;
                }
            }
        }
        
        //check max-backup-num
        for(int _i = yLib::yLogTagProperty::file_param.log_file_max_backup_num; _i > 1; _i --){
            
            std::string _the_old_log_file_name = this->log_file_handle_map[tag].first \
                                            + ".bak" + std::to_string(_i - 1);
            std::string _the_new_log_file_name = this->log_file_handle_map[tag].first \
                                            + ".bak" + std::to_string(_i);

            if (-1 != access(_the_old_log_file_name.c_str(), F_OK)){ //if this file exist, we remove it
            
                if (0 > rename(_the_old_log_file_name.c_str(), _the_new_log_file_name.c_str())){

                    std::string _err_msg_str = "rename this olde-log-bak-file("+_the_old_log_file_name+")failed.";
                    perror(_err_msg_str.c_str());
                    return;
                }
            }
        }

        //For logfile.log to logfile.log.bak1
        //close logfile.log
        this->log_file_handle_map[tag].second->flush();
        this->log_file_handle_map[tag].second->close();

        std::string _the_old_log_file_name = this->log_file_handle_map[tag].first;
        
        std::string _the_new_log_file_name = this->log_file_handle_map[tag].first \
                                            + ".bak1";
        if (0 > rename(_the_old_log_file_name.c_str(), _the_new_log_file_name.c_str())){

            std::string _err_msg_str = "rename this old-log-bak-file("+_the_old_log_file_name+")failed.";
            perror(_err_msg_str.c_str());
            return;
        }

        //create new logfile.log
        std::shared_ptr<std::ofstream> _out_file = std::shared_ptr<std::ofstream>(new (std::nothrow) std::ofstream());
        _out_file->open(_the_old_log_file_name, std::ofstream::out | std::ofstream::app);

        if (!_out_file->is_open()){
            
            std::string _err_msg = "create file(" + _the_old_log_file_name +  ") failed.\n";
            ColoredWriteToStderr(yLib::LOG_ERROR, _err_msg.c_str(), _err_msg.length());
            return;
        }
        
        // replace the handle of old-log-file
        this->log_file_handle_map[tag].second = _out_file;
    };

    auto _check_timeout_flush = [this](void){

        struct ::timespec _new_time_spec;

        clock_gettime(CLOCK_REALTIME, &_new_time_spec);

        double _diff_time = (_new_time_spec.tv_sec - this->old_time_spec.tv_sec)*1000;
        _diff_time += (_new_time_spec.tv_nsec - this->old_time_spec.tv_nsec)*0.000001;

        if (_diff_time > yLib::yLogTagProperty::file_param.flush_timeout){
            
            //update old timespec
            this->old_time_spec.tv_nsec = _new_time_spec.tv_nsec;
            this->old_time_spec.tv_sec = _new_time_spec.tv_sec;


            return true;
        }
        return false;
    };

    auto _write_msg_to_log_file = [=](const std::string &tag, const std::string &msg){

        std::string _get_tag = "";
        if (yLib::yLogTagProperty::file_param.multi_log_file){
            
            if (this->log_file_handle_map.count(tag) > 0)
                _get_tag = tag;
        }

        //check file max size
        if (_check_file_max_size(this->log_file_handle_map[_get_tag].second)){

            //backup old file and rm old file
            _backup_file(tag);
        }

        // write msg
        (*this->log_file_handle_map[_get_tag].second)<<msg;

        // set flush flag ???
        if (yLib::yLogTagProperty::file_param.flush_every_times){

            this->log_file_handle_map[_get_tag].second->flush();
        }
        
    };

    std::string _cur_msg = "";
    std::string _cur_msg_tag = "";
    // log queue-msg to file
    write_thread_is_continue.store(true);
    write_thread_is_ready.store(true);
    while (write_thread_is_continue.load())
    {
        //get msg from cache-queue.
        _cur_msg = "";
        _cur_msg_tag = "";

        bool _has_msg = false;
        {
            std::unique_lock<std::mutex> _lck(file_cache_queue_mtx, std::defer_lock);
            if(_lck.try_lock()){
                
                if (0 != file_cache_queue.size()){

                    _cur_msg = file_cache_queue.front().second;
                    _cur_msg_tag = file_cache_queue.front().first;
                    file_cache_queue.pop();
                    _has_msg = true;
                }

            }
        }

        if (_has_msg){

            _write_msg_to_log_file(_cur_msg_tag, _cur_msg);
        }
        else{

            // sleep 5ms
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }

        // timeout to flush
        if (_check_timeout_flush()){

            flush_log_file();
        }
        
    }
    // exiting log thread , force write cache-queue to log file
    {
        std::unique_lock<std::mutex> _lck(file_cache_queue_mtx);


        while(0 != file_cache_queue.size()){
            
            // must set to "" per times
            _cur_msg = "";
            _cur_msg_tag = "";

            _cur_msg = file_cache_queue.front().second;
            _cur_msg_tag = file_cache_queue.front().first;
            
            _write_msg_to_log_file(_cur_msg_tag, _cur_msg);

            file_cache_queue.pop();
        }
    }
    
    flush_log_file();
}


// ylog implement
uint32_t yLib::yLog::YLIB_YLOG_MSG_BUF_MAX_SIZE = 1024*8; //8k ,linux-func-stack max size is 8MB
std::shared_ptr<std::unordered_map<std::string, yLib::yLogTagProperty>> \
    yLib::yLog::ylog_tag_prop_map = \
        std::shared_ptr<std::unordered_map<std::string, yLib::yLogTagProperty>> \
            (new (std::nothrow) std::unordered_map<std::string, yLib::yLogTagProperty>());
std::unique_ptr<char[]> yLib::yLog::ylog_msg_buf = std::unique_ptr<char[]>(new (std::nothrow) char[YLIB_YLOG_MSG_BUF_MAX_SIZE]);
std::mutex yLib::yLog::ylog_msg_buf_mtx;
std::unique_ptr<yLib::yLogDestination> yLib::yLog::ylog_destination;


void yLib::yLog::Init(const yLogTagProperty & tag_prop, const std::string &tag) noexcept{

    if (ylog_tag_prop_map->count(tag) == 0){

        ylog_tag_prop_map->insert(std::pair<std::string, yLogTagProperty>(tag, tag_prop));
    }
    else{
        //Do nothing. we don't update the file_param.
    }
}

void yLib::yLog::check_tag(const std::string &tag){

    if (ylog_tag_prop_map->count(tag) == 0){//insert tag

        // insert default tag prop
        ylog_tag_prop_map->insert(std::pair<std::string, yLogTagProperty>(tag, yLogTagProperty()));
    }

    return ;
}
void yLib::yLog::ylog_log_impl(yLib::yLogSeverity log_type, const std::string & msg, const std::string &tag)
{
    //check tag
    check_tag(tag);

    // add msg header
    yLogMessage(tag, log_type, "", 0, false).stream()<<msg;

}

void yLib::yLog::convert_fmt_to_str(yLib::yLogSeverity log_type, const char * fmt, va_list arg_list, const std::string & tag, const char * special_override_func_str)
{

    // thread safety
    std::unique_lock<std::mutex> _lck(ylog_msg_buf_mtx);
    
    ::memset(ylog_msg_buf.get(), 0, YLIB_YLOG_MSG_BUF_MAX_SIZE);//set buffer to '0'

    int _n_printed_num = 0; 

    if (nullptr == special_override_func_str)
        _n_printed_num = vsnprintf(ylog_msg_buf.get(), YLIB_YLOG_MSG_BUF_MAX_SIZE, fmt, arg_list);
    else
        _n_printed_num = snprintf(ylog_msg_buf.get(), YLIB_YLOG_MSG_BUF_MAX_SIZE, fmt, special_override_func_str);

    //std::cout<<"_n_printed_num " << _n_printed_num<<std::endl;
    if ( YLIB_YLOG_MSG_BUF_MAX_SIZE <= _n_printed_num ){

        //W("Size of input-string may be greater than MSG_BUF_SIZE:%d, input-string will be truncated by size of MSG_BUF_SIZE", MSG_BUF_SIZE - 1); //bug 
        std::cout<<"yLog(): Size: "<< _n_printed_num <<" of input-string may be greater than MSG_BUF_SIZE: " << YLIB_YLOG_MSG_BUF_MAX_SIZE<<", input-string will be truncated by size of MSG_BUF_SIZE"<<std::endl;
        ylog_msg_buf[YLIB_YLOG_MSG_BUF_MAX_SIZE - 2] = '\n';
        ylog_msg_buf[YLIB_YLOG_MSG_BUF_MAX_SIZE - 1] = '\0';
        ylog_log_impl(log_type, std::string(ylog_msg_buf.get(), YLIB_YLOG_MSG_BUF_MAX_SIZE - 1), tag);
    }
    else{

        ylog_log_impl(log_type, std::string(ylog_msg_buf.get(), _n_printed_num), tag);
    }
    
}


//log info 
void yLib::yLog::I(const std::string &fmt, ...) noexcept{
    
    va_list arg ;

    va_start(arg, fmt);

    convert_fmt_to_str(LOG_INFO, fmt.c_str(), arg, "");
    
    va_end(arg);
}
void yLib::yLog::I(const std::string &tag, const std::string &fmt, ...) noexcept{
    
    va_list arg ;

    va_start(arg, fmt);

    convert_fmt_to_str(LOG_INFO, fmt.c_str(), arg, tag);
    
    va_end(arg);
}

void yLib::yLog::I(const char *fmt , const char * str) noexcept{

    va_list arg ;
    convert_fmt_to_str(LOG_INFO, fmt, arg, "", str);
}

//log debug
void yLib::yLog::D(const std::string &fmt , ...) noexcept{
    
    va_list arg ;

    va_start(arg, fmt);

    convert_fmt_to_str(LOG_DEBUG, fmt.c_str(), arg, "");

    va_end(arg);
}
void yLib::yLog::D(const std::string &tag, const std::string &fmt , ...) noexcept{

    
    va_list arg ;

    va_start(arg, fmt);

    convert_fmt_to_str(LOG_DEBUG, fmt.c_str(), arg, tag);

    va_end(arg);
}
void yLib::yLog::D(const char *fmt , const char * str) noexcept{

    va_list arg ;
    convert_fmt_to_str(LOG_DEBUG, fmt, arg, "", str);
}

//log warning
void yLib::yLog::W(const std::string &fmt , ...) noexcept{

    va_list arg ;

    va_start(arg, fmt);

    convert_fmt_to_str(LOG_WARN, fmt.c_str(), arg, "");

    va_end(arg);
}
void yLib::yLog::W(const std::string &tag, const std::string &fmt , ...) noexcept{

    va_list arg ;

    va_start(arg, fmt);

    convert_fmt_to_str(LOG_WARN, fmt.c_str(), arg, tag);

    va_end(arg);
}
void yLib::yLog::W(const char *fmt , const char * str) noexcept{

    va_list arg ;
    convert_fmt_to_str(LOG_WARN, fmt, arg, "", str);
}

//log error
void yLib::yLog::E(const std::string &fmt , ...) noexcept{

    va_list arg ;

    va_start(arg, fmt);

    convert_fmt_to_str(LOG_ERROR, fmt.c_str(), arg, "");

    va_end(arg);
}
void yLib::yLog::E(const std::string &tag, const std::string &fmt , ...) noexcept{

    va_list arg ;

    va_start(arg, fmt);

    convert_fmt_to_str(LOG_ERROR, fmt.c_str(), arg, tag);

    va_end(arg);
}
void yLib::yLog::E(const char *fmt , const char * str) noexcept{

    va_list arg ;
    convert_fmt_to_str(LOG_ERROR, fmt, arg, "", str);
}

bool yLib::yLog::check_log_level(yLib::yLogSeverity log_type, const std::string &tag)
{   
    uint16_t _cur_log_level = (*ylog_tag_prop_map)[tag].log_level;

    switch (log_type)
    {
    case yLib::yLogSeverity::LOG_DEBUG:{
        
        /* code */
        if (_cur_log_level & ENABLE_DEBUG_LOG_LEVEL){
            
            return true;
        }
        break;
    }
    case yLib::yLogSeverity::LOG_INFO:{
        
        /* code */
        if (_cur_log_level & ENABLE_INFO_LOG_LEVEL){

            return true;
        }
        break;
    }
    case yLib::yLogSeverity::LOG_WARN:{
        
        /* code */
        if (_cur_log_level & ENABLE_WARN_LOG_LEVEL){

            return true;
        }
        
        break;
    }
    case yLib::yLogSeverity::LOG_ERROR:{
        
        /* code */
        if (_cur_log_level & ENABLE_ERROR_LOG_LEVEL){

            return true;
        }
        
        break;
    }
    default:
    
        E("ylog_log_impl(): input log type error.");
        break;
    }

    return false;
}
void yLib::yLog::Log(const std::string & tag, yLib::yLogSeverity severity, const std::string &msg)
{   
    check_tag(tag);

    // check enable-level
    if (!check_log_level(severity, tag))//not pass
        return ;

    // for yLogDestination
    if (ylog_destination == nullptr)
        ylog_destination = std::unique_ptr<yLib::yLogDestination>(yLib::yLogDestination::CreateInstance(ylog_tag_prop_map));

    if ((*ylog_tag_prop_map)[tag].is_log_to_stdio)
        ylog_destination->LogToStdIO(severity, msg.c_str(), msg.length());

    if ((*ylog_tag_prop_map)[tag].is_log_to_file)
        ylog_destination->LogToFile(msg.c_str(), msg.length(), yLib::yLogTagProperty::file_param.flush_every_times, tag);
    
}