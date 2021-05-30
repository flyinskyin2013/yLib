/*
 * @Author: Sky
 * @Date: 2021-05-26 15:15:09
 * @LastEditors: Sky
 * @LastEditTime: 2021-05-26 17:18:53
 * @Description: 
 */
#ifndef __YLIB_CORE_YCOMPILER_CORE_YREGULARENGINE_HPP__
#define __YLIB_CORE_YCOMPILER_CORE_YREGULARENGINE_HPP__

#include "core/yobject.hpp"

#include <list>
#include <memory>
#include <cstdint>
#include <vector>

namespace yLib
{
    class __YLIB_EXPORT__ yRegularEngine MACRO_PUBLIC_INHERIT_YOBJECT
    {
    public:
        class BaseRENode{
        public:
            enum BaseRENodeType:uint8_t{

                NONE_TYPE = 0,
                NORMAL_VALUE_TYPE = 1,//single char
                RANGE_VALUE_TYPE = 2,//[]
                AND_PROP_TYPE = 3,//ab
                OR_PROP_TYPE = 4,//|
                ASTERISK_PROP_TYPE = 5, //*
                PLUS_SIGN_PROP_TYPE = 6, //+
                QUESTION_MARK_PROP_TYPE = 7, //?
                ESCAPE_TYPE = 8,//\x, 
            };
        public:
            BaseRENode(void) = delete;

            BaseRENode(BaseRENodeType type):
            node_type(NONE_TYPE),
            normal_value(0),
            not_range_value(false){
                node_type = type;
            }

            ~BaseRENode(){}

        public:
            BaseRENodeType node_type;

            char normal_value;
            char escape_value;

            bool not_range_value;//[^abc]
            std::vector<std::pair<char, char>> range_value_vec;
        };
    public:
        yRegularEngine(/* args */);
        ~yRegularEngine();

    private:

        //call by ParseREStrToBaseRENodeList
        int8_t __process_base_re_node__(const std::string & regular_str, int & index, BaseRENode::BaseRENodeType & type);
        //parse regular's str to a list of RE-node.
        int8_t ParseREStrToFormatREStr(const std::string & regular_str);
        //for debug
        void PrintFormatREStr(void);


        //convert the format re-str to postfix re-str
        int8_t ConvertFormatREStrToPostfixREStr(void);
        //for debug
        void PrintPostfixREStr(void);


        //ConvertRPNRegularToNFA
        //PrintNFA
        //SimulateNFA

        //BuildDFAByNFA
        //PrintDFA
        //SimulateDFA




    private:
        //format regular expression string ,data field.
        std::list<std::shared_ptr<BaseRENode>> format_renode_list;

        //postfix regular expression string ,data field.
        std::list<std::shared_ptr<BaseRENode>> postfix_renode_list;
    };
    
    
} // namespace yLib

#endif //__YLIB_CORE_YCOMPILER_CORE_YREGULARENGINE_HPP__    
