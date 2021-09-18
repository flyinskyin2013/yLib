/*
 * @Author: Sky
 * @Date: 2021-09-18 15:51:57
 * @LastEditors: Sky
 * @LastEditTime: 2021-09-18 16:42:51
 * @Description: 
 */
#ifndef __YLIB_C_HPP__
#define __YLIB_C_HPP__

#define YLIB_C_API \
    extern "C"

// export class ylog, no override
YLIB_C_API void ylib_ylog_i(const char * fmt, ...);
YLIB_C_API void ylib_ylog_i_tag(const char * tag, const char * fmt, ...);
YLIB_C_API void ylib_ylog_w(const char * fmt, ...);
YLIB_C_API void ylib_ylog_w_tag(const char * tag, const char * fmt, ...);
YLIB_C_API void ylib_ylog_d(const char * fmt, ...);
YLIB_C_API void ylib_ylog_d_tag(const char * tag, const char * fmt, ...);
YLIB_C_API void ylib_ylog_e(const char * fmt, ...);
YLIB_C_API void ylib_ylog_e_tag(const char * tag, const char * fmt, ...);


#endif //__YLIB_C_HPP__