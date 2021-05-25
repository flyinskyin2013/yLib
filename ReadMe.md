<!--
 * @Author: Sky
 * @Date: 2019-10-21 13:51:28
 * @LastEditors: Sky
 * @LastEditTime: 2021-05-25 17:40:39
 * @Description: 
 -->
## yLib

****
[![codecov](https://codecov.io/gh/flyinskyin2013/yLib/branch/master/graph/badge.svg?token=CIOCB761NA)](https://codecov.io/gh/flyinskyin2013/yLib)
[![Build Status](https://www.travis-ci.org/flyinskyin2013/yLib.svg?branch=master)](https://www.travis-ci.org/flyinskyin2013/yLib)
![GitHub release (latest by date)](https://img.shields.io/github/v/release/flyinskyin2013/yLib)
![GitHub all releases](https://img.shields.io/github/downloads/flyinskyin2013/yLib/total)
[![License](https://img.shields.io/badge/license-BSD--3--Clause-green.svg)](License.txt) 

yLib 学习、收集、整理和验证在我日常工作中一些有用的知识。

yLib studies, collects, collates and verifies some useful knowledges in my daily-work.

此外，在日常工作中，我通常使用一些第三方库来实现一些常见的功能，像处理json、处理xml、处理log等等，这些常见的功能往往有许多的第三方库实现，并且他们的调用逻辑是完全不一致的。在不同的项目中，我有时候必须用不同的第三方库来实现同一个常见的功能，这会使得我的应用和特定的第三方库耦合度比较高，并大大增加了我的工作量。于是，我在寻思，我能不能够对于一些常见的特定功能，抽象出一层好用的ABI，然后屏蔽其实现的细节。这样会使得我自己的应用和这些第三方库解耦，提高我自己应用的稳定性。我希望它能够像POSIX一样帮助我。

In addition, in my daily-work, I usually use some third-libraries to implement some usual functions, like processing json, xml, log and so on, those are implemented by many third-libraries and their call-logic is very different. In different projects, I sometimes must use different third-libraries to implement the same function, it makes the coupling of my project and the third-library more high, and it makes my work more heavier. so, I think that can I abstract some nice ABIs for some usual function to hide their implementation details. At this moment, our applications and the third-library are decoupling, it improves the stability of my application. I hope it helps me like POSIX.




yLib BuildMatrix(GithubAction) <br> 

| Platefrom | Compiler | CPU(32bits)  | CPU (64bits) |
| :---: | :---: | :---: | :---: |
| Ubuntu2004 | GCC 9.x | [![Build Status](https://img.shields.io/github/workflow/status/flyinskyin2013/yLib/linux_x86_gcc)](https://github.com/flyinskyin2013/yLib/actions?query=workflow%3Alinux_x86_gcc) |  [![Build Status](https://img.shields.io/github/workflow/status/flyinskyin2013/yLib/linux_x64_gcc)](https://github.com/flyinskyin2013/yLib/actions?query=workflow%3Alinux_x64_gcc) |  
| Ubuntu2004 | Clang 10.x | [![Build Status](https://img.shields.io/github/workflow/status/flyinskyin2013/yLib/linux_x86_clang)](https://github.com/flyinskyin2013/yLib/actions?query=workflow%3Alinux_x86_clang) |  [![Build Status](https://img.shields.io/github/workflow/status/flyinskyin2013/yLib/linux_x64_clang)](https://github.com/flyinskyin2013/yLib/actions?query=workflow%3Alinux_x64_clang) |  
| Ubuntu2004 | ARM GCC 9.x | [![Build Status](https://img.shields.io/github/workflow/status/flyinskyin2013/yLib/linux_arm_gcc)](https://github.com/flyinskyin2013/yLib/actions?query=workflow%3Alinux_arm_gcc) |  [![Build Status](https://img.shields.io/github/workflow/status/flyinskyin2013/yLib/linux_aarch64_gcc)](https://github.com/flyinskyin2013/yLib/actions?query=workflow%3Alinux_aarch64_gcc) |  
| Ubuntu1804 | GCC 7.x | [![Build Status](https://img.shields.io/github/workflow/status/flyinskyin2013/yLib/ubuntu1804_x86_gcc)](https://github.com/flyinskyin2013/yLib/actions?query=workflow%3Aubuntu1804_x86_gcc) |  [![Build Status](https://img.shields.io/github/workflow/status/flyinskyin2013/yLib/ubuntu1804_x64_gcc)](https://github.com/flyinskyin2013/yLib/actions?query=workflow%3Aubuntu1804_x64_gcc) |  
| Ubuntu1804 | Clang 6.x | [![Build Status](https://img.shields.io/github/workflow/status/flyinskyin2013/yLib/ubuntu1804_x86_clang)](https://github.com/flyinskyin2013/yLib/actions?query=workflow%3Aubuntu1804_x86_clang) |  [![Build Status](https://img.shields.io/github/workflow/status/flyinskyin2013/yLib/ubuntu1804_x64_clang)](https://github.com/flyinskyin2013/yLib/actions?query=workflow%3Aubuntu1804_x64_clang) |  
| Ubuntu1804 | ARM GCC 7.x | [![Build Status](https://img.shields.io/github/workflow/status/flyinskyin2013/yLib/ubuntu1804_arm_gcc)](https://github.com/flyinskyin2013/yLib/actions?query=workflow%3Aubuntu1804_arm_gcc) |  [![Build Status](https://img.shields.io/github/workflow/status/flyinskyin2013/yLib/ubuntu1804_aarch64_gcc)](https://github.com/flyinskyin2013/yLib/actions?query=workflow%3Aubuntu1804_aarch64_gcc) |  
| Ubuntu1604 | GCC 5.x | [![Build Status](https://img.shields.io/github/workflow/status/flyinskyin2013/yLib/ubuntu1604_x86_gcc)](https://github.com/flyinskyin2013/yLib/actions?query=workflow%3Aubuntu1604_x86_gcc) |  [![Build Status](https://img.shields.io/github/workflow/status/flyinskyin2013/yLib/ubuntu1604_x64_gcc)](https://github.com/flyinskyin2013/yLib/actions?query=workflow%3Aubuntu1604_x64_gcc) |  
| Ubuntu1604 | Clang 3.x | [![Build Status](https://img.shields.io/github/workflow/status/flyinskyin2013/yLib/ubuntu1604_x86_clang)](https://github.com/flyinskyin2013/yLib/actions?query=workflow%3Aubuntu1604_x86_clang) |  [![Build Status](https://img.shields.io/github/workflow/status/flyinskyin2013/yLib/ubuntu1604_x64_clang)](https://github.com/flyinskyin2013/yLib/actions?query=workflow%3Aubuntu1604_x64_clang) |  
| Ubuntu1604 | ARM GCC 5.x | [![Build Status](https://img.shields.io/github/workflow/status/flyinskyin2013/yLib/ubuntu1604_arm_gcc)](https://github.com/flyinskyin2013/yLib/actions?query=workflow%3Aubuntu1604_arm_gcc) |  [![Build Status](https://img.shields.io/github/workflow/status/flyinskyin2013/yLib/ubuntu1604_aarch64_gcc)](https://github.com/flyinskyin2013/yLib/actions?query=workflow%3Aubuntu1604_aarch64_gcc) |  
| Windows | VS2015 | [![Build Status](https://img.shields.io/github/workflow/status/flyinskyin2013/yLib/windows_x86_vs2015)](https://github.com/flyinskyin2013/yLib/actions?query=workflow%3Awindows_x86_vs2015) |  [![Build Status](https://img.shields.io/github/workflow/status/flyinskyin2013/yLib/windows_x64_vs2015)](https://github.com/flyinskyin2013/yLib/actions?query=workflow%3Awindows_x64_vs2015) |  
| Windows | VS2017 | [![Build Status](https://img.shields.io/github/workflow/status/flyinskyin2013/yLib/windows_x86_vs2017)](https://github.com/flyinskyin2013/yLib/actions?query=workflow%3Awindows_x86_vs2017) |  [![Build Status](https://img.shields.io/github/workflow/status/flyinskyin2013/yLib/windows_x64_vs2017)](https://github.com/flyinskyin2013/yLib/actions?query=workflow%3Awindows_x64_vs2017) |  
| Windows | VS2019 | [![Build Status](https://img.shields.io/github/workflow/status/flyinskyin2013/yLib/windows_x86_vs2019)](https://github.com/flyinskyin2013/yLib/actions?query=workflow%3Awindows_x86_vs2019) |  [![Build Status](https://img.shields.io/github/workflow/status/flyinskyin2013/yLib/windows_x64_vs2019)](https://github.com/flyinskyin2013/yLib/actions?query=workflow%3Awindows_x64_vs2019) |  


## Version:
****
Current lastest version: V0.0.9<br> 
Major versions maintain interface-compatibility<br> 
More details,please see [Version Notes](VersionNotes.txt)

MajorFeatures:
==========
****
yLib contains that I used some useful skills.<br> 
More details,please see [Supported Notes](SupportedNotes.txt)<br> 

Dependences:
==========
****
[Dependences Lists](DependencesLists.txt)<br> 

## OnlineDocs:
****
[OnlineDocs](http://sky-x.gitee.io/ylib_docs/)<br> 


## InstallAndUsages:
****
[Install and Usages](InstallAndUsages.txt)<br> 

## Licenses:
****
[Licenses Notes](License.txt)<br>