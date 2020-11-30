/*
 * @Author: Sky
 * @Date: 2020-09-15 16:49:43
 * @LastEditors: Sky
 * @LastEditTime: 2020-11-30 16:33:43
 * @Description: 
 */
#ifndef __YLIB_CORE_YLIB_EXTRA_INFOS_HPP__
#define __YLIB_CORE_YLIB_EXTRA_INFOS_HPP__

#include <string>

namespace yLib{

    /**
     *  @fn      const std::string & GetyLibBuildType(void)
     *  @brief   get build-type-string of yLib.
     *  @warning 
     *  @return The build-type-string 
     */
    const std::string & GetyLibBuildType(void);

    /**
     *  @fn      const std::string & GetyLibBuildArch(void)
     *  @brief   get build-arch-string of yLib.
     *  @warning 
     *  @return The build-arch-string 
     */
    const std::string & GetyLibBuildArch(void);

    /**
     *  @fn      const std::string & GetyLibBuildPlatform(void)
     *  @brief   get build-platform-string of yLib.
     *  @warning 
     *  @return The build-platform-string 
     */
    const std::string & GetyLibBuildPlatform(void);

    /**
     *  @fn      const int GetyLibBuildVersionMajor(void)
     *  @brief   get build-version-major-string of yLib.
     *  @warning 
     *  @return The build-version-major-string
     */
    const int GetyLibBuildVersionMajor(void);

    /**
     *  @fn      const int GetyLibBuildVersionMinor(void)
     *  @brief   get build-version-minor-string of yLib.
     *  @warning 
     *  @return The build-version-minor-string
     */
    const int GetyLibBuildVersionMinor(void);

    /**
     *  @fn      const int GetyLibBuildVersionPatch(void)
     *  @brief   get build-version-patch-string of yLib.
     *  @warning 
     *  @return The build-version-patch-string
     */
    const int GetyLibBuildVersionPatch(void);

    /**
     *  @fn      const int GetyLibBuildTimeStamp(void)
     *  @brief   get build-timestamp-string of yLib.
     *  @warning 
     *  @return The build-timestamp-string
     */    
    const std::string & GetyLibBuildTimeStamp(void);

    /**
     *  @fn      const std::string & GetyLibBuildGitCommitId(void)
     *  @brief   get build-git-commit-id-string of yLib.
     *  @warning 
     *  @return The build-git-commit-id-string
     */    
    const std::string & GetyLibBuildGitCommitId(void);

    /**
     *  @fn      const std::string & GetyLibBuildBuiltinModule(void)
     *  @brief   get build-in-module-string of yLib.
     *  @warning 
     *  @return The build-in-module-string
     */    
    const std::string & GetyLibBuildBuiltinModule(void);
}


#endif //__YLIB_CORE_YLIB_EXTRA_INFOS_HPP__