/*
 * @Author: Sky
 * @Date: 2020-10-26 18:35:08
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2021-09-04 16:31:16
 * @Description: 
 */

package com.ylib.utility;

public class yLog {
    
    static {
		
		System.loadLibrary("libylib.so");
    }

    
    public native static void D(String fmt , Object ... objs);
    public native static void W(String fmt, Object ... objs);
    public native static void I(String fmt, Object ... objs);
    public native static void E(String fmt, Object ... objs);

    public native static void D(String category_name, String fmt, Object ... objs);
    public native static void W(String category_name, String fmt, Object ... objs);
    public native static void I(String category_name, String fmt, Object ... objs);
    public native static void E(String category_name, String fmt, Object ... objs);

}
