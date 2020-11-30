/*
 * @Author: Sky
 * @Date: 2018-10-23 11:07:58
 * @LastEditors: Sky
 * @LastEditTime: 2020-11-30 16:20:19
 * @Description: 
 */

#ifndef __YLIB_CORE_YCOMMON_HPP__
#define __YLIB_CORE_YCOMMON_HPP__

#include <string>

#include "yobject.hpp"
#include "ylib_extra_infos.hpp"

namespace yLib{
    

    /**
     *  @class yCommon
     *  @brief This is common-class in yLib.
     */
    class __YLIB_EXPORT__ yCommon MACRO_PUBLIC_INHERIT_YOBJECT
    {
    private:
        /**
         * @var ylib_version
         * @brief the current version of yLib
         */
        std::string ylib_version;
    public:
        /**
         *  @fn    yCommon() noexcept 
         *  @brief Default constructor
         */
        yCommon(/* args */) noexcept MACRO_INIT_YOBJECT_PROPERTY(yCommon){}

        /**
         *  @fn    ~yCommon()noexcept
         *  @brief Default destructor
         */
        ~yCommon()noexcept{}

        /**
         *  @fn      yCommon(const yCommon & common) = delete
         *  @brief   Copy constructor
         *  @param   common exsited obj.
         *  @warning This op is deleted.
         *  @return 
         */        
        yCommon(const yCommon & common) = delete;

        /**
         *  @fn      yCommon & operator=(const yCommon & common) = delete
         *  @brief   Assignment constructor
         *  @param   common exsited obj.
         *  @warning This op is deleted.
         *  @return The reference of yCommon's object.
         */
        yCommon & operator=(const yCommon & common) = delete;

        /**
         *  @fn     yCommon(const yCommon && common) = delete
         *  @brief   Move constructor
         *  @param   common exsited obj.
         *  @warning This op is deleted.
         *  @return 
         */
        yCommon(const yCommon && common) = delete;

        /**
         *  @fn      yCommon & operator=(const yCommon && common) =delete
         *  @brief   Assignment constructor
         *  @param   common exsited obj.
         *  @warning This op is deleted.
         *  @return The reference of yCommon's object.
         */
        yCommon & operator=(const yCommon && common) =delete;


        /**
         *  @fn      std::string GetyLibVersion() const noexcept
         *  @brief   get version-string of yLib.
         *  @warning 
         *  @return the version-string
         */
        std::string GetyLibVersion() const noexcept;


        /**
         *  @fn      std::string GetyLibBuildInfo() const noexcept
         *  @brief   get build-info-string of yLib.
         *  @warning 
         *  @return The build-info-string 
         */
        std::string GetyLibBuildInfo() const noexcept;
    };

 }

#endif //__YLIB_CORE_YCOMMON_HPP__