'''
Author: Sky
Date: 2021-09-14 11:44:44
LastEditors: Sky
LastEditTime: 2021-09-24 17:52:54
Description: 
'''
import sys

sys.path.append('.')

import yLib

# test yObject
# yObjectClassInfo
obj1=yLib.core.yObject.yLibGetClassInfo()
print('The object name is = '+obj1.class_name)


# test yLog
# yLogClassInfo
# obj1=yLib.yLog.yLibGetClassInfo()
# print(obj1.class_name)


if __name__ == '__main__':
    yLog.I('test ylog')