/*
 * @Author: Sky
 * @Date: 2020-10-26 18:35:08
 * @LastEditors: Sky
 * @LastEditTime: 2020-10-26 19:10:24
 * @Description: 
 */

package com.ylib.utility;

public class yLog {
    
    static {
		
		System.loadLibrary("libylib.so");
    }


    public native static void SetLog4Cpp(boolean enable_log4cpp_, String log_cfg_path_);
    public native static void SetLog4cppSubCategory(String category_name_);


    //If you want to enable this feature,system must define _POSIX_SHARED_MEMORY_OBJECTS(getconf -a)
    public native static void SetProcessSafetyFeature(boolean enable_feature_);

    public native static void SetLog4cppLogLevel(Integer log_level_);
    public native static void SetLog4cppLogLevel(String category_name_, Integer log_level_);
    
    public native static void SetyLogLogLevel(Integer log_level_);
    public native static void SetyLogLogLevel(String category_name_, Integer log_level_);
    
    
    public native static void D(String fmt_ , Object ... objs_);
    public native static void W(String fmt_ , Object ... objs_);
    public native static void I(String fmt_, Object ... objs_);
    public native static void E(String fmt_ , Object ... objs_);

    public native static void D(String category_name_, String fmt_ , Object ... objs_);
    public native static void W(String category_name_, String fmt_ , Object ... objs_);
    public native static void I(String category_name_, String fmt_ , Object ... objs_);
    public native static void E(String category_name_, String fmt_ , Object ... objs_);

}
