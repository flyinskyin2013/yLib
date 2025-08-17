<!--
 * @Author: Sky
 * @Date: 2019-10-21 13:51:28
 * @LastEditors: Sky
 * @LastEditTime: 2021-12-01 17:02:56
 * @Description: 
 -->
## yLib

****
[![codecov](https://codecov.io/gh/flyinskyin2013/yLib/branch/master/graph/badge.svg?token=CIOCB761NA)](https://codecov.io/gh/flyinskyin2013/yLib)
[![Build Status](https://github.com/flyinskyin2013/yLib/actions/workflows/linux.yml/badge.svg?branch=master)](https://github.com/flyinskyin2013/yLib/actions?query=workflow%linux)
[![GitHub release (latest by date)](https://img.shields.io/github/v/release/flyinskyin2013/yLib)](https://github.com/flyinskyin2013/yLib/releases)
[![License](https://img.shields.io/badge/license-BSD--3--Clause-green.svg)](License.txt) 

yLib 学习、收集、整理和验证在我日常工作中一些有用的知识。

yLib studies, collects, collates and verifies some useful knowledges in my daily-work.

此外，在日常工作中，我通常使用一些第三方库来实现一些常见的功能，像处理json、处理xml、处理log等等，这些常见的功能往往有许多的第三方库实现，并且他们的调用逻辑是完全不一致的。在不同的项目中，我有时候必须用不同的第三方库来实现同一个常见的功能，这会使得我的应用和特定的第三方库耦合度比较高，并大大增加了我的工作量。于是，我在寻思，我能不能够对于一些常见的特定功能，抽象出一层好用的API，然后屏蔽其实现的细节。这样会使得我自己的应用和这些第三方库解耦，提高我自己应用的稳定性。我希望它能够像POSIX一样帮助我。

In addition, in my daily-work, I usually use some third-libraries to implement some usual functions, like processing json, xml, log and so on, those are implemented by many third-libraries and their call-logic is very different. In different projects, I sometimes must use different third-libraries to implement the same function, it makes the coupling of my project and the third-library more high, and it makes my work more heavier. so, I think that can I abstract some nice APIs for some usual function to hide their implementation details. At this moment, our applications and the third-library are decoupling, it improves the stability of my application. I hope it helps me like POSIX.




yLib BuildMatrix(GithubAction) <br> 

| Platefrom | Compiler |  Arch | BuildStatus |
| :---: | :---: | :---: | :---: |
| Linux | GCC    |  x64/x86  | [![Build Status][build_linux_gcc]][ci_linux_gcc] |
| Linux | Clang  |  x64/x86  | [![Build Status][build_linux_clang]][ci_linux_clang] |
| Linux | GCC  |  aarch64/armeabihf/armeabi  | [![Build Status][build_linux_arm_gcc]][ci_linux_arm_gcc] |
| Windows | VS2019(v142) |  x64/Win32  | [![Build Status][build_windows_v142]][ci_windows_v142] | 
| Windows | VS2022(v143) |  x64/Win32  | [![Build Status][build_windows_v143]][ci_windows_v143] |
| Android | NDK-26d |  armv8-a  | [![Build Status][build_android]][ci_android] | 


[build_linux_gcc]: https://img.shields.io/github/actions/workflow/status/flyinskyin2013/yLib/linux.yml?branch=master
[ci_linux_gcc]: https://github.com/flyinskyin2013/yLib/actions/workflows/linux.yml


[build_linux_clang]:https://img.shields.io/github/actions/workflow/status/flyinskyin2013/yLib/linux.yml?branch=master
[ci_linux_clang]: https://github.com/flyinskyin2013/yLib/actions/workflows/linux.yml


[build_linux_arm_gcc]:https://img.shields.io/github/actions/workflow/status/flyinskyin2013/yLib/linux.yml?branch=master
[ci_linux_arm_gcc]:https://github.com/flyinskyin2013/yLib/actions/workflows/linux.yml


[build_windows_v142]:https://img.shields.io/github/actions/workflow/status/flyinskyin2013/yLib/windows.yml?branch=master
[ci_windows_v142]:https://github.com/flyinskyin2013/yLib/actions/workflows/windows.yml

[build_windows_v143]:https://img.shields.io/github/actions/workflow/status/flyinskyin2013/yLib/windows.yml?branch=master
[ci_windows_v143]:https://github.com/flyinskyin2013/yLib/actions/workflows/windows.yml


[build_android]:https://img.shields.io/github/actions/workflow/status/flyinskyin2013/yLib/android.yml?branch=master
[ci_android]:https://github.com/flyinskyin2013/yLib/actions/workflows/android.yml



## Version:
****
Current lastest version: V0.2.0<br> 
Major versions maintain interface-compatibility<br> 
More details,please see [Version Notes](VersionNotes.txt)<br> 

当前最新的版本是:  V0.2.0<br> 
主版本号保持接口的稳定性。 <br> 
更多的详情，请查看 [Version Notes](VersionNotes.txt)<br> 

MajorFeatures:
==========
****
yLib contains that I used some useful skills.<br>
<br> 
There are some interesting contents:<br> 
**yConfig** is implemented by **ycompiler** likes llvm-clang's frontend.<br> 
**yLog** likes google-glog and log4cpp.<br> 
<br> 
More details,please see [Supported Notes](SupportedNotes.txt)<br> 

yLib包含了我用过的一些有用的技巧。<br>
<br>
这里有一些有趣的内容:<br> 
**yConfig** 是通过 **ycompiler** 来实现的，它是一个像 llvm-clang's 前端工程.<br> 
**yLog** 像glog和log4cpp.<br> 
<br> 
更多的详情，请查看 [Supported Notes](SupportedNotes.txt)<br> 

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