/*
 * @Author: Sky
 * @Date: 2021-09-18 15:23:28
 * @LastEditors: Sky
 * @LastEditTime: 2021-09-24 15:04:03
 * @Description: 
 */

package com.sky;

import com.sky.ylib.utility.yLog;

public class test {

    public static void main(String[] args) {

        yLog.I("abc %d, %d, %s\n", 1, 2, "test");

        yLog.D("abc %s ", "abc\n");// Attention: in this case, we call 'I(String tag, String fmt, Object ... objs)'.
        //Attention: it ouput "abc" instead of "abc abc".

        yLog.E("Tag", "abc\n");
        yLog.W("Tag", "abc %d, %d, %s\n", 1, 2, "test");
    }
}