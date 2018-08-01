yLib
==========
This lib collect and collate usage of common third-lib in my daily-work.



Version Notes:
==========
v0.02
----------
I reconstructed the framework of compiltion and test, and relized ylog-module yshell-module yxml-module  

v0.01
----------
It preliminary relized ycurl-module and establish a framework of compiltion and test.


Module Lists:
==========
ycurl-module<br> 
ylog-module<br> 
yshell-module<br> 
yxml-module<br> 

Dependences:
==========
libcurl-7.55.1<br> 
libxml2-2.7.1<br> 




Install:
==========
run the following commmands.

>>mkdir -p build 

>>cd build && cmake ..

>>make -j4

In build/src dir , it may have libyLib.so or libyLib_s.a. You can use it in your project.

