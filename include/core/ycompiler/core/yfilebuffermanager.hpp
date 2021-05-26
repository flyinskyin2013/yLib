/*
 * @Author: Sky
 * @Date: 2021-03-20 14:31:59
 * @LastEditors: Sky
 * @LastEditTime: 2021-05-26 15:15:36
 * @Description: 
 */

#ifndef __YLIB_CORE_YCOMPILER_CORE_YFILEBUFFERMANAGER_HPP__
#define __YLIB_CORE_YCOMPILER_CORE_YFILEBUFFERMANAGER_HPP__

#include <fstream>
#include <cstdint>
#include <string>
#include <vector>


#include "core/yobject.hpp"

namespace yLib
{
    
    class __YLIB_EXPORT__ yFileBufferManager MACRO_PUBLIC_INHERIT_YOBJECT
    {
    private:
        /* data */
        std::vector<int8_t> file_buffer;
        uint64_t process_pos = 0;
        uint64_t cur_row = 0;
        uint64_t cur_column = 0;
    public:
        yFileBufferManager(/* args */);
        ~yFileBufferManager();

        //read api
        int8_t LoadSourceFile(const std::string &src_file);
        int8_t LoadSourceFile(const char * src_file);

        //
        int8_t GetNext(void);
        void BackPos(uint64_t num);

        uint64_t GetCurColumn(void);
        uint64_t GetCurRow(void);
    };

} // namespace yLib


#endif //__YLIB_CORE_YCOMPILER_CORE_YFILEBUFFERMANAGER_HPP__