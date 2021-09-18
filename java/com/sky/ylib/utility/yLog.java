/*
 * @Author: Sky
 * @Date: 2020-10-26 18:35:08
 * @LastEditors: Sky
 * @LastEditTime: 2021-09-18 16:37:08
 * @Description: 
 */

package com.sky.ylib.utility;
import com.sun.jna.Library;
import com.sun.jna.Native;
import com.sun.jna.Platform;


public class yLog{

  public interface yLib extends Library {

    yLib jna_instance = (yLib)Native.load((Platform.isWindows() ? "ylib" : "ylib"), yLib.class);
    
    void ylib_ylog_i(String fmt, Object ... objs);
    void ylib_ylog_i_tag(String tag, String fmt, Object ... objs);
    void ylib_ylog_d(String fmt, Object ... objs);
    void ylib_ylog_d_tag(String tag, String fmt, Object ... objs);
    void ylib_ylog_w(String fmt, Object ... objs);
    void ylib_ylog_w_tag(String tag, String fmt, Object ... objs);
    void ylib_ylog_e(String fmt, Object ... objs);
    void ylib_ylog_e_tag(String tag, String fmt, Object ... objs);
  }

  public static void I(String fmt, Object ... objs){

    yLib.jna_instance.ylib_ylog_i(fmt, objs);
  }
  public static void I(String tag, String fmt, Object ... objs){

    yLib.jna_instance.ylib_ylog_i_tag(tag, fmt, objs);
  }

  public static void D(String fmt, Object ... objs){

    yLib.jna_instance.ylib_ylog_d(fmt, objs);
  }
  public static void D(String tag, String fmt, Object ... objs){

    yLib.jna_instance.ylib_ylog_d_tag(tag, fmt, objs);
  }

  public static void W(String fmt, Object ... objs){

    yLib.jna_instance.ylib_ylog_w(fmt, objs);
  }
  public static void W(String tag, String fmt, Object ... objs){

    yLib.jna_instance.ylib_ylog_w_tag(tag, fmt, objs);
  }

  public static void E(String fmt, Object ... objs){

    yLib.jna_instance.ylib_ylog_e(fmt, objs);
  }
  public static void E(String tag, String fmt, Object ... objs){

    yLib.jna_instance.ylib_ylog_e_tag(tag, fmt, objs);
  }
}



