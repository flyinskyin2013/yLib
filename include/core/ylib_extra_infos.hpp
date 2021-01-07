/*
 * @Author: Sky
 * @Date: 2020-09-15 16:49:43
 * @LastEditors: Sky
 * @LastEditTime: 2021-01-07 16:29:06
 * @Description: 
 */
#ifndef __YLIB_CORE_YLIB_EXTRA_INFOS_HPP__
#define __YLIB_CORE_YLIB_EXTRA_INFOS_HPP__

#include <string>
#include <core/yobject.hpp>

namespace yLib{

    /**
     *  @fn      const std::string & GetyLibBuildType(void)
     *  @brief   get build-type-string of yLib.
     *  @warning 
     *  @return The build-type-string 
     */
    __YLIB_EXPORT__ const std::string & GetyLibBuildType(void);

    /**
     *  @fn      const std::string & GetyLibBuildArch(void)
     *  @brief   get build-arch-string of yLib.
     *  @warning 
     *  @return The build-arch-string 
     */
    __YLIB_EXPORT__ const std::string & GetyLibBuildArch(void);

    /**
     *  @fn      const std::string & GetyLibBuildPlatform(void)
     *  @brief   get build-platform-string of yLib.
     *  @warning 
     *  @return The build-platform-string 
     */
    __YLIB_EXPORT__ const std::string & GetyLibBuildPlatform(void);

    /**
     *  @fn      const int GetyLibBuildVersionMajor(void)
     *  @brief   get build-version-major-string of yLib.
     *  @warning 
     *  @return The build-version-major-string
     */
    __YLIB_EXPORT__ const int GetyLibBuildVersionMajor(void);

    /**
     *  @fn      const int GetyLibBuildVersionMinor(void)
     *  @brief   get build-version-minor-string of yLib.
     *  @warning 
     *  @return The build-version-minor-string
     */
    __YLIB_EXPORT__ const int GetyLibBuildVersionMinor(void);

    /**
     *  @fn      const int GetyLibBuildVersionPatch(void)
     *  @brief   get build-version-patch-string of yLib.
     *  @warning 
     *  @return The build-version-patch-string
     */
    __YLIB_EXPORT__ const int GetyLibBuildVersionPatch(void);

    /**
     *  @fn      const int GetyLibBuildTimeStamp(void)
     *  @brief   get build-timestamp-string of yLib.
     *  @warning 
     *  @return The build-timestamp-string
     */    
    __YLIB_EXPORT__ const std::string & GetyLibBuildTimeStamp(void);

    /**
     *  @fn      const std::string & GetyLibBuildGitCommitId(void)
     *  @brief   get build-git-commit-id-string of yLib.
     *  @warning 
     *  @return The build-git-commit-id-string
     */    
    __YLIB_EXPORT__ const std::string & GetyLibBuildGitCommitId(void);

    /**
     *  @fn      const std::string & GetyLibBuildBuiltinModule(void)
     *  @brief   get build-in-module-string of yLib.
     *  @warning 
     *  @return The build-in-module-string
     */    
    __YLIB_EXPORT__ const std::string & GetyLibBuildBuiltinModule(void);
}


#endif //__YLIB_CORE_YLIB_EXTRA_INFOS_HPP__