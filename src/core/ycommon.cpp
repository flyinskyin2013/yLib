/*
 * @Author: Sky
 * @Date: 2018-10-23 11:09:07
 * @LastEditors: Sky
 * @LastEditTime: 2021-09-01 11:57:18
 * @Description: 
 */

#include "core/ycommon.hpp" 


#ifdef __cplusplus
extern "C"{
#endif //__cplusplus

#include <string.h>
#include <time.h>

#ifdef __cplusplus
}
#endif //__cplusplus


YLIB_IMPLEMENT_CLASSINFO_CONTENT(yCommon)

/**
 *  @fn    std::string ConvertErrnoToStr(uint64_t err_num) noexcept
 *  @brief convert errno to str, it wrapper strerror_r() and FormatMessage().
 */
std::string yLib::yCommon::ConvertErrnoToStr(uint64_t err_num) noexcept
{
#ifdef _WIN32

    //FormatMessage
#elif __linux__ || __linux
    
    char _str_buf[50];
    ::memset(_str_buf, 0, 50);
    return std::string(strerror_r(err_num, _str_buf, 50));
#endif 
}
