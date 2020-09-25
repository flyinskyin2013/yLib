/*
 * @Author: Sky
 * @Date: 2020-09-25 11:45:26
 * @LastEditors: Sky
 * @LastEditTime: 2020-09-25 16:28:17
 * @Description: 
 */

#include <cstdint>
#include <string>

#include "utility/ylog.hpp"


namespace yLib{

    class yLinuxErrnoHelper
    {
    private:
        /* data */
        yLinuxErrnoHelper(/* args */);
    public:
        ~yLinuxErrnoHelper();

        static const std::string & GetDetailByErrno(uint16_t errno_);
        static const std::string & GetShortHandByErrno(uint16_t errno_);
    };
    

}
