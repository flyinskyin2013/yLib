/*
Copyright (c) 2018 - 2021 flyinskyin2013 All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
/*
 * @Author: Sky
 * @Date: 2021-05-26 15:17:53
 * @LastEditors: Sky
 * @LastEditTime: 2021-05-26 17:57:40
 * @Description: 
 */

#include "core/ycompiler/basic/yregularengine.hpp"

#include "core/ylog/ylog.hpp"

#include <stack>
#include <cstdio>

using namespace yLib;






static void __print_re_basenode_list__(const std::list<std::shared_ptr<yRegularEngine::BaseRENode>> &list)
{

    printf("Attention: ',' is used to mark AND_PROP_TYPE. the '\\,' is real ','. \n");
    for(auto _renode:list){

        switch (_renode->node_type)
        {
            case yRegularEngine::BaseRENode::RANGE_VALUE_TYPE:{

                // []
                printf("[");
                for(auto _p:_renode->range_value_vec){

                    if (_p.first == _p.second){

                        printf("%c", _p.first);
                    }
                    else{

                        printf("%c-%c", _p.first, _p.second);
                    }
                }
                printf("]");
                break;
            }
            case yRegularEngine::BaseRENode::OR_PROP_TYPE:{

                // |
                printf("|");
                break;
            }
            case yRegularEngine::BaseRENode::ASTERISK_PROP_TYPE:{

                // *
                printf("*");
                break;
            }
            case yRegularEngine::BaseRENode::PLUS_SIGN_PROP_TYPE:{

                // +
                printf("+");
                break;
            }
            case yRegularEngine::BaseRENode::QUESTION_MARK_PROP_TYPE:{

                // ?
                printf("?");
                break;
            }
            case yRegularEngine::BaseRENode::ESCAPE_TYPE:{

                // '\'
                printf("\\%c", (char)_renode->escape_value);
                break;
            }
            case yRegularEngine::BaseRENode::AND_PROP_TYPE:{

                printf(",");
                break;
            }
            default:{//normal char

                //BaseRENode::NORMAL_VALUE_TYPE
                if ((char)_renode->normal_value == ','){

                    printf("\\%c", (char)_renode->normal_value);
                }
                else{

                    printf("%c", (char)_renode->normal_value);
                }
                
                break;
            }
        }        
    }
    printf("\n");
}

static int8_t __parse_range_value_vec__(const std::string & regular_str, int & index, std::vector<std::pair<char, char>> & range_value_vec){

    //deal with range value
    //[0-9a-zA-Z]
    //[\\], [\[], [\]], [\|], [\+], [\?], [\*], [\^]
    //[(], [)], [.],other print-char

    uint8_t _layer_count = 1;
    char _last_char = 0; //0 normal, 1 escape, 2 range 
    while(_layer_count != 0 &&index < regular_str.length()){

        if ( regular_str[index] == '-' ){//[0-9a-zA-Z], [abc123ABC]

            _last_char = 2;
        }
        else if (regular_str[index] == '\\'){//[\\], [\[], [\]], [\|], [\+], [\?], [\*]

            if (_last_char == 1){//[\\]

                _tmp_and_node->range_value_vec[_tmp_and_node->range_value_vec.size() - 1] = {regular_str[index], regular_str[index]};
                _last_char = 0;                        
            }
            else{

                _last_char = 1;//mark last char is escape
                _tmp_and_node->range_value_vec.push_back({0, 0});
            }
        }
        else{//[(], [)], [.],other print-char

            if (_last_char == 0){
                
                if (regular_str[index] == ']')
                    _layer_count--;
                else
                    _tmp_and_node->range_value_vec.push_back({regular_str[index], regular_str[index]});
            }
            else if (_last_char == 1){//[\char]
                
                if (regular_str[index] == '['|| \
                    regular_str[index] == ']'|| \
                    regular_str[index] == '|'|| \
                    regular_str[index] == '+'|| \
                    regular_str[index] == '?'|| \
                    regular_str[index] == '*'|| 
                ){

                    _tmp_and_node->range_value_vec[_tmp_and_node->range_value_vec.size() - 1] = {regular_str[index], regular_str[index]};
                    _last_char = 0;  
                }
                else{

                    yLib::yLog::E("The RegularStr is invalid. we can't parse range-value, the escape can be {\\\\, \\[, \\], \\|, \\+, \\?, \\*}.");
                    return -1;
                }
                
            }
            else {// '-'

                if ( (regular_str[index] >= '0' && regular_str[index] <= '9') || \
                        (regular_str[index] >= 'a' && regular_str[index] <= 'z') || \
                        (regular_str[index] >= 'A' && regular_str[index] <= 'Z')
                ){

                    if (regular_str[index] >= '0' && regular_str[index] <= '9'){//num

                        if (_tmp_and_node->range_value_vec[_tmp_and_node->range_value_vec.size() - 1].first >= '0' && \
                            _tmp_and_node->range_value_vec[_tmp_and_node->range_value_vec.size() - 1].first <= '9' && \
                            _tmp_and_node->range_value_vec[_tmp_and_node->range_value_vec.size() - 1].first <= regular_str[index]
                        ){

                            _tmp_and_node->range_value_vec[_tmp_and_node->range_value_vec.size() - 1].second = regular_str[index];
                            _last_char = 0;                                      
                        }
                        else{

                            yLib::yLog::E("The RegularStr is invalid. we can't parse range-value, the range can be {num0-num1, alpha0-alpha1, num0 <= num1, alpha0 <= alpha1, alpha0 and alpha1 must be lowercase or uppercase}.");
                            return -1;                                        
                        }
                    }
                    else if (regular_str[index] >= 'a' && regular_str[index] <= 'z'){//lowercase

                        if (_tmp_and_node->range_value_vec[_tmp_and_node->range_value_vec.size() - 1].first >= 'a' && \
                            _tmp_and_node->range_value_vec[_tmp_and_node->range_value_vec.size() - 1].first <= 'z' && \
                            _tmp_and_node->range_value_vec[_tmp_and_node->range_value_vec.size() - 1].first <= regular_str[index]
                        ){

                            _tmp_and_node->range_value_vec[_tmp_and_node->range_value_vec.size() - 1].second = regular_str[index];
                            _last_char = 0;                                      
                        }
                        else{

                            yLib::yLog::E("The RegularStr is invalid. we can't parse range-value, the range can be {num0-num1, alpha0-alpha1, num0 <= num1, alpha0 <= alpha1, alpha0 and alpha1 must be lowercase or uppercase}.");
                            return -1;                                        
                        }
                    }
                    else{//upcase

                        if (_tmp_and_node->range_value_vec[_tmp_and_node->range_value_vec.size() - 1].first >= 'A' && \
                            _tmp_and_node->range_value_vec[_tmp_and_node->range_value_vec.size() - 1].first <= 'Z' && \
                            _tmp_and_node->range_value_vec[_tmp_and_node->range_value_vec.size() - 1].first <= regular_str[index]
                        ){

                            _tmp_and_node->range_value_vec[_tmp_and_node->range_value_vec.size() - 1].second = regular_str[index];
                            _last_char = 0;                                      
                        }
                        else{

                            yLib::yLog::E("The RegularStr is invalid. we can't parse range-value, the range can be {num0-num1, alpha0-alpha1, num0 <= num1, alpha0 <= alpha1, alpha0 and alpha1 must be lowercase or uppercase}.");
                            return -1;                                        
                        }                                
                    }
                }
                else{

                    yLib::yLog::E("The RegularStr is invalid. we can't parse range-value, the range can be {num0-num1, alpha0-alpha1, num0 <= num1, alpha0 <= alpha1, alpha0 and alpha1 must be lowercase or uppercase}.");
                    return -1;                            
                }
            }
        }


        index ++;
    }

    if (0 < _op_stack.size() || _last_char != 0){

        yLib::yLog::E("The RegularStr is invalid. we can't parse range-value.");
        return -1;
    }
}

//////////////////////////////////////////////////
//////////////////////////////////////////////////
//////////yRegularEngine
//////////////////////////////////////////////////
//////////////////////////////////////////////////

yRegularEngine::yRegularEngine(/* args */)
MACRO_INIT_YOBJECT_PROPERTY(yRegularEngine)
{
    nfa_start_node = std::make_shared<RENFANode>(RENFANode::START_TYPE);//create a start nfa node
}

yRegularEngine::~yRegularEngine()
{
}

int8_t yRegularEngine::__process_base_re_node__(const std::string & regular_str, int & index, BaseRENode::BaseRENodeType & type){

    switch (regular_str[index])
    {
        case '[':{

            std::shared_ptr<BaseRENode> _tmp_and_node = std::make_shared<BaseRENode>(BaseRENode::RANGE_VALUE_TYPE);

            index ++;
            if (index >= regular_str.length()){

                yLib::yLog::E("The RegularStr is invalid. we can't parse range-value.");
                return -1;
            }

            if ('^' == regular_str[index]){//parse '^'

                _tmp_and_node->not_range_value = true;
                index ++;
                if (index >= regular_str.length()){

                    yLib::yLog::E("The RegularStr is invalid. we can't parse range-value.");
                    return -1;
                }
            }


            if (0 > __parse_range_value_vec__(regular_str, index, _tmp_and_node->range_value_vec)){

                return -1;
            }
 

            format_renode_list.push_back(_tmp_and_node); 

            type = BaseRENode::RANGE_VALUE_TYPE; 
            break;
        }
        case '|':{

            std::shared_ptr<BaseRENode> _tmp_and_node = std::make_shared<BaseRENode>(BaseRENode::OR_PROP_TYPE);

            format_renode_list.push_back(_tmp_and_node);   

            type = BaseRENode::OR_PROP_TYPE; 
            break;
        }
        case '*':{

            std::shared_ptr<BaseRENode> _tmp_and_node = std::make_shared<BaseRENode>(BaseRENode::ASTERISK_PROP_TYPE);

            format_renode_list.push_back(_tmp_and_node);   

            type = BaseRENode::ASTERISK_PROP_TYPE; 
            break;
        }
        case '+':{

            std::shared_ptr<BaseRENode> _tmp_and_node = std::make_shared<BaseRENode>(BaseRENode::PLUS_SIGN_PROP_TYPE);

            format_renode_list.push_back(_tmp_and_node);  

            type = BaseRENode::PLUS_SIGN_PROP_TYPE;  
            break;
        }
        case '?':{

            std::shared_ptr<BaseRENode> _tmp_and_node = std::make_shared<BaseRENode>(BaseRENode::QUESTION_MARK_PROP_TYPE);

            format_renode_list.push_back(_tmp_and_node);   

            type = BaseRENode::QUESTION_MARK_PROP_TYPE; 
            break;
        }
        case '\\':{

            std::shared_ptr<BaseRENode> _tmp_and_node = std::make_shared<BaseRENode>(BaseRENode::ESCAPE_TYPE);
            
            index ++;//get next char
            if (index >= regular_str.length()){

                yLib::yLog::E("The RegularStr is invalid. we can't parse escape-value.");
                return -1;
            }
            else{

                _tmp_and_node->escape_value = regular_str[index];
            }
            
            format_renode_list.push_back(_tmp_and_node);  

            type = BaseRENode::ESCAPE_TYPE; 
            break;
        }
        default:{//normal char

            std::shared_ptr<BaseRENode> _tmp_and_node = std::make_shared<BaseRENode>(BaseRENode::NORMAL_VALUE_TYPE);
            _tmp_and_node-> = regular_str[index];

            format_renode_list.push_back(_tmp_and_node);   

            type = BaseRENode::NORMAL_VALUE_TYPE;
            break;
        }
    }

    return 0;;
}

int8_t yRegularEngine::ParseREStrToFormatREStr(const std::string & regular_str){

    BaseRENode::BaseRENodeType _last_base_node_type = BaseRENode::NONE_TYPE;

    for (int _i = 0; _i < regular_str.length(); _i++){

        //not first node or last node isn't '|'
        if (_last_base_node_type != BaseRENode::NONE_TYPE && _last_base_node_type != BaseRENode::OR_PROP_TYPE){//insert and type

            //regular_str[_i] can't be |,+,?,*
            if (regular_str[_i] != '|'||\
                regular_str[_i] != '+'||\
                regular_str[_i] != '?'||\
                regular_str[_i] != '*')
            {

                std::shared_ptr<BaseRENode> _tmp_and_node = std::make_shared<BaseRENode>(BaseRENode::AND_PROP_TYPE);
                format_renode_list.push_back(_tmp_and_node);     
            }
        }

        if (0 != __process_base_re_node__(regular_str, _i, _last_base_node_type) ){

            return -1;
        }
    }

    return 0;
}

void yRegularEngine::PrintFormatREStr(void){

    __print_re_basenode_list__(format_renode_list);
}

static bool __is_relation_node__(const std::shared_ptr<BaseRENode> & node)
{

    if (node->node_type == BaseRENode::BaseRENodeType::AND_PROP_TYPE || \
        node->node_type == BaseRENode::BaseRENodeType::OR_PROP_TYPE){

            return true;
    }
    
    return false;
}

// 操作符
// 优先级 符号 运算顺序
// 
// 2 or 从左至右
// 3 and  从左至右

// 'and' > 'or' 
static uint8_t __op_priority__(const std::shared_ptr<BaseRENode> & node)
{
    switch(node->node_type)    {

        case BaseRENode::BaseRENodeType::AND_PROP_TYPE:{

            return 3;
            break;
        }
        case BaseRENode::BaseRENodeType::OR_PROP_TYPE:{

            return 2;
            break;
        }
        default:{

            return 0;
            break;
        }
    }
    return 0;
}

//Shunting Yard Algorithm, author : Edsger Dijkstra
int8_t yRegularEngine::ConvertFormatREStrToPostfixREStr(void)
{

    std::stack<std::shared_ptr<BaseRENode>> _operator_stack;
    
    for(auto _base_re_node:format_renode_list){

        if (__is_relation_node__(_base_re_node)){//'and' or 'or' 

            while(_operator_stack.size() != 0){

                //cur op-prioriy is less than stack-top op-priority
                if (__op_priority__(_base_re_node) < __op_priority__(_operator_stack.top())){

                    postfix_renode_list.push_back(_operator_stack.top());
                    _operator_stack.pop();
                }
                else{

                    break;
                }
            }

            _operator_stack.push(_base_re_node);
        }
        else{//other

            postfix_renode_list.push_back(_base_re_node);
        }
    }
    return 0;
}

void yRegularEngine::PrintPostfixREStr(void)
{
    __print_re_basenode_list__(postfix_renode_list);
}

//ConvertRPNRegularToNFA
int8_t yRegularEngine::ConvertRPNRegularToNFA(void)
{

    return 0;
}

//PrintNFA
void yRegularEngine::PrintNFA(void)
{


}

//SimulateNFA
bool yRegularEngine::SimulateNFA(void)
{

    return true;
}