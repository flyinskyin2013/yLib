#ifndef _YLOG_H_
#define _YLOG_H_

#include <iostream>
#include <string>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>

#include "ycommon.h"

namespace yLib{

    #define MSG_BUF_SIZE 2048
    class yLog{

        public:
            yLog();
            ~yLog();
            static void D(const char * fmt , ...);
            static void W(const char * fmt , ...);
            static void I(const char * fmt , ...);
            static void E(const char * fmt , ...);
        protected:
        private:
        static char * m_ptr_msg_buf;
        static bool m_b_is_class_access;
    };
}



#endif //_YLOG_H_