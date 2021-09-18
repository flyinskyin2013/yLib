/*
 * @Author: Sky
 * @Date: 2021-09-18 16:42:20
 * @LastEditors: Sky
 * @LastEditTime: 2021-09-18 16:49:59
 * @Description: 
 */

#include "ylib_c.h"

#include "ylib.hpp"

/********************************yLog c api********************************************/
//yLog c api
YLIB_C_API void ylib_ylog_i(const char * fmt, ...){

    yLib::yLog::I(fmt);
}

YLIB_C_API void ylib_ylog_i_tag(const char * tag, const char * fmt, ...){


}
YLIB_C_API void ylib_ylog_w(const char * fmt, ...){


}

YLIB_C_API void ylib_ylog_w_tag(const char * tag, const char * fmt, ...){


}

YLIB_C_API void ylib_ylog_d(const char * fmt, ...){


}

YLIB_C_API void ylib_ylog_d_tag(const char * tag, const char * fmt, ...){


}
YLIB_C_API void ylib_ylog_e(const char * fmt, ...){



}
YLIB_C_API void ylib_ylog_e_tag(const char * tag, const char * fmt, ...){


}

/********************************yLog c api********************************************/