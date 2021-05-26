/*
 * @Author: Sky
 * @Date: 2021-05-26 15:17:53
 * @LastEditors: Sky
 * @LastEditTime: 2021-05-26 17:57:40
 * @Description: 
 */

#include "core/ycompiler/core/yregularengine.hpp"

#include "utility/ylog/ylog.hpp"

#include <stack>

using namespace yLib;


//////////////////////////////////////////////////
//////////////////////////////////////////////////
//////////yRegularEngine
//////////////////////////////////////////////////
//////////////////////////////////////////////////

yRegularEngine::yRegularEngine(/* args */)
MACRO_INIT_YOBJECT_PROPERTY(yLexicalAnalyzer)
{
}

yRegularEngine::~yRegularEngine()
{
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
 

            re_node_list.push_back(_tmp_and_node); 

            type = BaseRENode::RANGE_VALUE_TYPE; 
            break;
        }
        case '|':{

            std::shared_ptr<BaseRENode> _tmp_and_node = std::make_shared<BaseRENode>(BaseRENode::OR_PROP_TYPE);

            re_node_list.push_back(_tmp_and_node);   

            type = BaseRENode::OR_PROP_TYPE; 
            break;
        }
        case '*':{

            std::shared_ptr<BaseRENode> _tmp_and_node = std::make_shared<BaseRENode>(BaseRENode::ASTERISK_PROP_TYPE);

            re_node_list.push_back(_tmp_and_node);   

            type = BaseRENode::ASTERISK_PROP_TYPE; 
            break;
        }
        case '+':{

            std::shared_ptr<BaseRENode> _tmp_and_node = std::make_shared<BaseRENode>(BaseRENode::PLUS_SIGN_PROP_TYPE);

            re_node_list.push_back(_tmp_and_node);  

            type = BaseRENode::PLUS_SIGN_PROP_TYPE;  
            break;
        }
        case '?':{

            std::shared_ptr<BaseRENode> _tmp_and_node = std::make_shared<BaseRENode>(BaseRENode::QUESTION_MARK_PROP_TYPE);

            re_node_list.push_back(_tmp_and_node);   

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
            
            re_node_list.push_back(_tmp_and_node);  

            type = BaseRENode::ESCAPE_TYPE; 
            break;
        }
        default:{//normal char

            std::shared_ptr<BaseRENode> _tmp_and_node = std::make_shared<BaseRENode>(BaseRENode::NORMAL_VALUE_TYPE);
            _tmp_and_node-> = regular_str[index];

            re_node_list.push_back(_tmp_and_node);   

            type = BaseRENode::NORMAL_VALUE_TYPE;
            break;
        }
    }

    return 0;;
}

int8_t yRegularEngine::ParseREStrToBaseRENodeList(const std::string & regular_str){

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
                re_node_list.push_back(_tmp_and_node);     
            }
        }

        if (0 != __process_base_re_node__(regular_str, _i, _last_base_node_type) ){

            return -1;
        }
    }

    return 0;
}

void yRegularEngine::PrintBaseRENodeList(void){

    
}