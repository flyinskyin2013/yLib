/*
 * @Author: Sky
 * @Date: 2020-09-25 11:45:26
 * @LastEditors: Sky
 * @LastEditTime: 2020-11-30 16:40:12
 * @Description: 
 */

#include <cstdint>
#include <string>

#include "utility/ylog.hpp"


namespace yLib{

    /**
     *  @class ySystemErrnoHelper
     *  @brief This is a helper class of errno.support win32 and linux
     */
    class ySystemErrnoHelper
    {
    private:
        /* data */
        /**
         *  @fn    ySystemErrnoHelper()
         *  @brief Default constructor
         *  @warning This is a static class, it can't be instanced. 
         */
        ySystemErrnoHelper();
    public:
        /**
         *  @fn    ~ySystemErrnoHelper()
         *  @brief Default destructor
         */
        ~ySystemErrnoHelper();

        /**
         *  @fn      static const std::string & GetDetailByErrno(uint16_t errno_)
         *  @brief   Move constructor
         *  @param   errno_ the linux errno.
         *  @return  a detail string about errno.
         *  @warning 
         */
        static const std::string & GetDetailByErrno(uint16_t errno_);

        /**
         *  @fn      static const std::string & GetShortHandByErrno(uint16_t errno_)
         *  @brief   a shorthand string about errno.
         *  @param   errno_ the linux errno.
         *  @warning 
         */
        static const std::string & GetShortHandByErrno(uint16_t errno_);
    };
    

}
