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
        //BaseRENode
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

        //RENFANode
        class RENFANode{
            public:
            const uint16_t EDGE_PROP_EPSILON = 256;
            const uint16_t EDGE_PROP_EMPTY = 257;

            public:
            enum RENFANodeType:uint8_t{

                NONE_TYPE = 0, //next_edge_vec.size() == 0
                START_TYPE,    //next_edge_vec.size() == 1, next_edge_vec[0] EPSILON
                END_TYPE,      //next_edge_vec.size() == 1, next_edge_vec[0] EPSILON
                SINGLE_TYPE,   //next_edge_vec.size() == 1, next_edge_vec[0] edge-prop
                MULTI_TYPE,     //next_edge_vec.size() == 1+, 
            };

            public:
            RENFANode(void) = delete;
            RENFANode(RENFANodeType type)
            {
                node_type = type;
            }
            ~RENFANode(){}

            public:
            //next RENFANode, next edge-prop
            std::vector<std::pair<std::shared_ptr<RENFANode>, uint16_t>> next_edge_vec;

            RENFANodeType node_type;
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
        //Shunting Yard Algorithm, author : Edsger Dijkstra
        int8_t ConvertFormatREStrToPostfixREStr(void);
        //for debug
        void PrintPostfixREStr(void);


        //ConvertRPNRegularToNFA
        int8_t ConvertRPNRegularToNFA(void);

        //PrintNFA
        void PrintNFA(void);

        //SimulateNFA
        bool SimulateNFA(void);

        //BuildDFAByNFA
        //PrintDFA
        //SimulateDFA




    private:
        //format regular expression string ,data field.
        std::list<std::shared_ptr<BaseRENode>> format_renode_list;

        //postfix regular expression string ,data field.
        std::list<std::shared_ptr<BaseRENode>> postfix_renode_list;

        //nfa start_node
        std::shared_ptr<RENFANode> nfa_start_node;
    };
    
    
} // namespace yLib

#endif //__YLIB_CORE_YCOMPILER_CORE_YREGULARENGINE_HPP__    
