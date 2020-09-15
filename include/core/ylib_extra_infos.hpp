/*
 * @Author: Sky
 * @Date: 2020-09-15 16:49:43
 * @LastEditors: Sky
 * @LastEditTime: 2020-09-15 17:50:22
 * @Description: 
 */
#ifndef __YLIB_CORE_YLIB_EXTRA_INFOS_HPP__
#define __YLIB_CORE_YLIB_EXTRA_INFOS_HPP__

#include <string>

namespace yLib{

    const std::string & GetyLibBuildType(void);
    const std::string & GetyLibBuildArch(void);
    const std::string & GetyLibBuildPlatform(void);
    const int GetyLibBuildVersionMajor(void);
    const int GetyLibBuildVersionMinor(void);
    const int GetyLibBuildVersionPatch(void);
    const std::string & GetyLibBuildTimeStamp(void);
    const std::string & GetyLibBuildGitCommitId(void);
    const std::string & GetyLibBuildBuiltinModule(void);
}


#endif //__YLIB_CORE_YLIB_EXTRA_INFOS_HPP__