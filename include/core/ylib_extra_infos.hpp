/*
 * @Author: Sky
 * @Date: 2020-09-15 16:49:43
 * @LastEditors: Sky
 * @LastEditTime: 2021-08-26 17:56:46
 * @Description: 
 */
#ifndef __YLIB_CORE_YLIB_EXTRA_INFOS_HPP__
#define __YLIB_CORE_YLIB_EXTRA_INFOS_HPP__

#include <string>

#include "yobject.hpp"

namespace yLib{

    /**
     *  @class yLibExtraInfos
     *  @brief this is a static class, it can get some information for yLib.
     */
    class __YLIB_EXPORT__ yLibExtraInfos:
    YLIB_PUBLIC_INHERIT_YOBJECT,
    YLIB_PUBLIC_INHERIT_YNOTCOPYABLEANDMOVEABLE,
    YLIB_PUBLIC_INHERIT_YSTATICCLASS
    {   
        public:
        /**
         *  @fn      static const std::string & GetyLibBuildType(void) noexcept
         *  @brief   get build-type-string of yLib.
         *  @warning 
         *  @return The build-type-string 
         */
        static const std::string & GetyLibBuildType(void) noexcept;

        /**
         *  @fn      static const std::string & GetyLibBuildArch(void) noexcept
         *  @brief   get build-arch-string of yLib.
         *  @warning 
         *  @return The build-arch-string 
         */
        static const std::string & GetyLibBuildArch(void) noexcept;

        /**
         *  @fn      static const std::string & GetyLibBuildPlatform(void) noexcept
         *  @brief   get build-platform-string of yLib.
         *  @warning 
         *  @return The build-platform-string 
         */
        static const std::string & GetyLibBuildPlatform(void) noexcept;

        /**
         *  @fn      static const int GetyLibBuildVersionMajor(void) noexcept
         *  @brief   get build-version-major-string of yLib.
         *  @warning 
         *  @return The build-version-major-string
         */
        static const int GetyLibBuildVersionMajor(void) noexcept;

        /**
         *  @fn      static const int GetyLibBuildVersionMinor(void) noexcept
         *  @brief   get build-version-minor-string of yLib.
         *  @warning 
         *  @return The build-version-minor-string
         */
        static const int GetyLibBuildVersionMinor(void) noexcept;

        /**
         *  @fn      static const int GetyLibBuildVersionPatch(void) noexcept
         *  @brief   get build-version-patch-string of yLib.
         *  @warning 
         *  @return The build-version-patch-string
         */
        static const int GetyLibBuildVersionPatch(void) noexcept;

        /**
         *  @fn      static const int GetyLibBuildTimeStamp(void) noexcept
         *  @brief   get build-timestamp-string of yLib.
         *  @warning 
         *  @return The build-timestamp-string
         */    
        static const std::string & GetyLibBuildTimeStamp(void) noexcept;

        /**
         *  @fn      static const std::string & GetyLibBuildGitCommitId(void) noexcept
         *  @brief   get build-git-commit-id-string of yLib.
         *  @warning 
         *  @return The build-git-commit-id-string
         */    
        static const std::string & GetyLibBuildGitCommitId(void) noexcept;

        /**
         *  @fn      static std::string GetyLibBuildBuiltinModule(void) noexcept
         *  @brief   get build-in-module-string of yLib.
         *  @warning 
         *  @return The build-in-module-string
         */    
        static std::string GetyLibBuildBuiltinModule(void) noexcept;

        /**
         *  @fn      static std::string GetyLibVersion() noexcept
         *  @brief   get version-string of yLib.
         *  @warning 
         *  @return the version-string
         */
        static std::string GetyLibVersion() noexcept;


        /**
         *  @fn      static std::string GetyLibBuildInfo() noexcept
         *  @brief   get build-info-string of yLib.
         *  @warning 
         *  @return The build-info-string 
         */
        static std::string GetyLibBuildInfo() noexcept;


        YLIB_DECLARE_CLASSINFO_CONTENT(yLibExtraInfos);
    };

}


#endif //__YLIB_CORE_YLIB_EXTRA_INFOS_HPP__