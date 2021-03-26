/*
 * @Author: Sky
 * @Date: 2021-03-22 15:57:39
 * @LastEditors: Sky
 * @LastEditTime: 2021-03-24 11:40:17
 * @Description: 
 */
#include <stack>
#include <queue>
#include <string>
#include <memory>
#include <iostream>
#include <cstdint>
#include <vector>
#include <cstring>

struct State 
{ 
	int c;

	struct State * out;

	struct State * out1;

	int lastlist; 
};

//Reverse Polish notation



/*
Regular to Abstract Syntax Tree
https://zhuanlan.zhihu.com/p/54291684
*/
struct RegularASTNode{
	enum NodeType:uint16_t{
		
		NONE_TYPE = 0,
		NORMAL_NODE_TYPE = 1,//a-z,A-Z,0-9
		ESCAPE_CHARACTER_TYPE, //'\x'
		OR_NODE_TYPE,//'|'
		CONCATENATE_NODE_TYPE,//ab is a concatenate b
	};
	
	RegularASTNode(NodeType type, char prop)
	:left(nullptr),right(nullptr)
	{
		node_type = type;
		node_prop = prop;
	}
	RegularASTNode(const RegularASTNode& node)
	{
		this->left = node.left;
		this->right = node.right;
		this->node_type = node.node_type;
	}
	RegularASTNode(const RegularASTNode&& node)
	{
		this->left = node.left;
		this->right = node.right;	
		this->node_type = node.node_type;
	}	
	std::shared_ptr<RegularASTNode> left;
	std::shared_ptr<RegularASTNode> right;


	NodeType node_type;
	char node_prop;
};

#define LOG_ERR(msg) std::cout<<msg<<std::endl

int RegularToAST(const std::string &reg_str, std::shared_ptr<RegularASTNode> &root)
{

	std::stack<char> _operator_statck;
	std::stack<std::shared_ptr<RegularASTNode>> _node_stack;

	for(int _i = 0; _i < reg_str.length(); _i++){

		if (reg_str[_i] >= 'a' && reg_str[_i] <= 'z'){//a-z
			
			_node_stack.push(std::make_shared<RegularASTNode>(RegularASTNode::NORMAL_NODE_TYPE, reg_str[_i]));
		}
		else if (reg_str[_i] >= 'A' && reg_str[_i] <= 'Z'){//A-Z
			
			_node_stack.push(std::make_shared<RegularASTNode>(RegularASTNode::NORMAL_NODE_TYPE, reg_str[_i]));
		}
		else if (reg_str[_i] >= '0' && reg_str[_i] <= '9'){//0-9
			
			_node_stack.push(std::make_shared<RegularASTNode>(RegularASTNode::NORMAL_NODE_TYPE, reg_str[_i]));
		}
		else if (reg_str[_i] == '.'){//.

			_node_stack.push(std::make_shared<RegularASTNode>(RegularASTNode::NORMAL_NODE_TYPE, reg_str[_i]));
		}
		else if (reg_str[_i] == '(' || reg_str[_i] == '[' || reg_str[_i] == '{' ){

			_operator_statck.push(reg_str[_i]);
		}
		else if (reg_str[_i] == ')'){

			char _tmp_op_top = _operator_statck.top();
			_operator_statck.pop();
			while('(' != _tmp_op_top){

				if ('|' == _tmp_op_top){//two data op

					std::shared_ptr<RegularASTNode> &_behind = _node_stack.top();
					_node_stack.pop();

					std::shared_ptr<RegularASTNode> &_before = _node_stack.top();
					_node_stack.pop();

					std::shared_ptr<RegularASTNode> _new_node = std::make_shared<RegularASTNode>(RegularASTNode::OR_NODE_TYPE, '|');

					_new_node->left = _before;
					_new_node->right = _behind;

					_node_stack.push(_new_node);
				}

				_tmp_op_top = _operator_statck.top();
				_operator_statck.pop();
			}

			while (1 != _node_stack.size())
			{
					std::shared_ptr<RegularASTNode> &_behind = _node_stack.top();
					_node_stack.pop();

					std::shared_ptr<RegularASTNode> &_before = _node_stack.top();
					_node_stack.pop();

					std::shared_ptr<RegularASTNode> _new_node = std::make_shared<RegularASTNode>(RegularASTNode::CONCATENATE_NODE_TYPE, 0);		

					_new_node->left = _before;
					_new_node->right = _behind;

					_node_stack.push(_new_node);		
			}
			
		}
		else if (reg_str[_i] == ']'){

			
		}
		else if (reg_str[_i] == '}'){

			
		}
		else if (reg_str[_i] == '\\'){//Do't support type like '\x0a'
			
			_i++;
			if (_i < reg_str.length())
				_node_stack.push(std::make_shared<RegularASTNode>(RegularASTNode::ESCAPE_CHARACTER_TYPE, reg_str[_i]));
			else{

				LOG_ERR("The reg_str is invalid. We can't read the second escape-character");
				return -1;
			}
		}
		else if (reg_str[_i] == '*'){

			_operator_statck.push(reg_str[_i]);			
		}
		else if (reg_str[_i] == '+'){

			_operator_statck.push(reg_str[_i]);			
		}
		else if (reg_str[_i] == '?'){
			
			_operator_statck.push(reg_str[_i]);			
		}
		else if (reg_str[_i] == '^'){

			_operator_statck.push(reg_str[_i]);			
		}
		else if (reg_str[_i] == '|'){

			_operator_statck.push(reg_str[_i]);					
		}
		else {

			LOG_ERR("Not support char");
			return -1;
		}

	}

	//
	if (1 != _node_stack.size()){

		while (1 != _node_stack.size())
		{
				std::shared_ptr<RegularASTNode> &_behind = _node_stack.top();
				_node_stack.pop();

				std::shared_ptr<RegularASTNode> &_before = _node_stack.top();
				_node_stack.pop();

				std::shared_ptr<RegularASTNode> _new_node = std::make_shared<RegularASTNode>(RegularASTNode::CONCATENATE_NODE_TYPE, 0);		

				_new_node->left = _before;
				_new_node->right = _behind;

				_node_stack.push(_new_node);		
		}
	}

	root = _node_stack.top();
	_node_stack.pop();

	return 0;
}

//wfs
void PrintBinaryTree(const std::shared_ptr<RegularASTNode> &root){

	auto _file_blank_by_n = [](std::string & str, int n)->void{
		
		for(int _ii = 0; _ii < n - 2; _ii++)
			str+= " ";
	};

	std::string _blank = "  ";
	std::vector<std::string> _layer_vec;
	std::queue<std::shared_ptr<RegularASTNode>> _reg_ast_queue;
	std::queue<int> _parent_node_pos;

	
	std::string _tmp_node_str = "TreeRoot";
	_layer_vec.push_back(_tmp_node_str);
	

	_reg_ast_queue.push(root->left);
	_reg_ast_queue.push(root->right);
	bool _is_end = false;
	while(_reg_ast_queue.size() != 0 && !_is_end){

		int _nullptr_node_count = 0;
		int _cur_layer_node_count = _reg_ast_queue.size();
		std::string _cur_layer_tmp_str;
		std::string _cur_layer_tmp_str_line;
		for (int _i = 0; _i < _cur_layer_node_count; _i++){

			std::shared_ptr<RegularASTNode> &_tmp = _reg_ast_queue.front();
			_reg_ast_queue.pop();

			if (nullptr != _tmp){

				char _tmp_arr[30];
				::memset(_tmp_arr, 0, sizeof(_tmp_arr));
				sprintf(_tmp_arr, "N(t: %d, p: %d)", _tmp->node_type, _tmp->node_prop);
				_cur_layer_tmp_str += _tmp_arr + _blank;
				
				_reg_ast_queue.push(_tmp->left);
				_reg_ast_queue.push(_tmp->right);	

				_nullptr_node_count = 0;
			}
			else{

				_cur_layer_tmp_str += "null" + _blank;

				_reg_ast_queue.push(nullptr);
				_reg_ast_queue.push(nullptr);	

				_nullptr_node_count ++;
				//std::cout<<_nullptr_node_count<<std::endl;
				if (_nullptr_node_count >= _cur_layer_node_count){//last layer

					_is_end = true;
					break;
				}
			}

		}

		_layer_vec.push_back(_cur_layer_tmp_str);
		//_layer_vec.push_back(_cur_layer_tmp_str_line);
	}

	for (auto _s:_layer_vec){

		std::cout<<_s<<std::endl;
	}
}




int8_t ArithmeticExpressionToPostfixExpression(const std::string & old_str, std::string & new_str)
{
	std::stack<char> _operator_stack;
	for(auto _c:old_str){

		if ('0' <= _c && _c <= '9'){

			new_str += _c;
		}
		else if (')' == _c){

			char _tmp = _operator_stack.top();
			_operator_stack.pop();

			while('(' != _tmp){//+ - * /]\

				new_str += _tmp;

				_tmp = _operator_stack.top();
				_operator_stack.pop();				
			}
		}
		else if ('+' == _c || '-' == _c){

			while(1){
				
				if (_operator_stack.size() == 0){

					_operator_stack.push(_c);
					break;
				}
					
				if (_operator_stack.top() == '*'|| _operator_stack.top() == '/'){

					char _tmp = _operator_stack.top();
					_operator_stack.pop();	
					new_str += _tmp;			
				}
				else{//'+' '-' '('

					_operator_stack.push(_c);
					break;
				}
			}
		}
		else if ('*' == _c || '/' == _c){

			_operator_stack.push(_c);
		}
		else{

			_operator_stack.push(_c);
		}
	}

	while(_operator_stack.size() != 0){
		
		new_str += _operator_stack.top();
		_operator_stack.pop();
	}
	return 0;
}




// 操作符
// 优先级 符号 运算顺序
// 
// 2 | 从左至右
// 3 , 从左至右
// 4 + ? * 从左至右
int op_priority(const char c)
{
    switch(c)    {

        case ',':
            return 3;
        case '|':
            return 2;
        default:
            return 1;
    }
    return 0;
}

unsigned int op_arg_count(const char c)
{
    switch(c)  {
        case '+': case '?': case '*':
            return 1;
        case ',': case '|':
            return 2;
        default:
            return 0;
    }
    return 0;
}

bool is_rang_element(const std::string & old_str, int start_idx){


	if ((start_idx+1) < old_str.length() && (start_idx+2) < old_str.length()){

		if (old_str[start_idx + 1] == '-'){

			return true;
		}
	}
	return false;
}

bool is_special_element(const std::string & old_str, int start_idx){//+ ? *

	if ((start_idx+1) < old_str.length()){

		if (old_str[start_idx + 1] == '+' || old_str[start_idx + 1] == '?' || old_str[start_idx + 1] == '*'){

			return true;
		}
	}
	return false;
}

bool is_normal_element(const std::string & old_str, int start_idx){//a-z A-Z 0-9

	if ( (old_str[start_idx] >= '0' && old_str[start_idx] <= '9')|| \
	(old_str[start_idx] >= 'a' && old_str[start_idx] <= 'z')|| \
	(old_str[start_idx] >= 'A' && old_str[start_idx] <= 'Z') ){

		return true;
	}
	
	return false;
}

bool is_or_exp(const std::string & old_str, int start_idx)
{

	if ((start_idx+1) < old_str.length()){

		if (old_str[start_idx + 1] == '|' ){

			return true;
		}
	}
	return false;
}

void MyFormatRegularExpression(std::string & str)//fill ','
{
	std::string _output = "";
	std::queue<char> _data_s;

	for(int _i = 0; _i < str.length(); _i++){

		if (is_normal_element(str, _i)){

			if (_data_s.size() > 0){

				while(_data_s.size() != 0){

					_output += _data_s.front();
					_data_s.pop();
				}
				_output +=",";
			}
			_data_s.push(str[_i]);
		}
		else if (str[_i] == '+' || str[_i] == '?' || str[_i] == '*'){

			_data_s.push(str[_i]);
		}
		else if (str[_i] == '|'){

			if (_data_s.size() > 0){

				while(_data_s.size() != 0){

					_output += _data_s.front();
					_data_s.pop();
				}
				_output +="|";
			}
			else{

				std::cout<<"'|' arg is invalid ......"<<std::endl;
			}

		}
		else {

			std::cout<<"Don't support char ......"<<std::endl;
		}
	}

	while(_data_s.size() != 0){

		_output += _data_s.front();
		_data_s.pop();
	}

	str = _output;
	return ;
}

int8_t RegularExpressionToPostfixExpression(std::string & old_str, std::string & new_str){
	
	std::stack<char> _operator_stack;

	for(int _i = 0; _i < old_str.length(); _i++){

		if (is_normal_element(old_str, _i)){

			new_str+=old_str[_i];
		}
		else if (old_str[_i] == ','){

			while(_operator_stack.size() != 0){

				if (op_priority(old_str[_i]) <= op_priority(_operator_stack.top())){

					new_str+=_operator_stack.top();
					_operator_stack.pop();
				}
				else {

					break;
				}
			}

			_operator_stack.push(old_str[_i]);
		}
		else if (old_str[_i] == '+' || old_str[_i] == '?' || old_str[_i] == '*'){

			new_str+=old_str[_i];
		}
		else if (old_str[_i] == '|'){

			while(_operator_stack.size() != 0){

				if (op_priority(old_str[_i]) <= op_priority(_operator_stack.top())){

					new_str+=_operator_stack.top();
					_operator_stack.pop();
				}
				else {

					break;
				}
			}

			_operator_stack.push(old_str[_i]);		
		}
		else{

			std::cout<<"Don't support char ......"<<std::endl;
		}
	}

	while(_operator_stack.size() != 0){

		new_str += _operator_stack.top();
		_operator_stack.pop();
	}


	return 0;
}


#define EDGE_PROP_EPSILON 256
#define EDGE_PROP_EMPTY 257
struct RegularNFANode{
	enum NodeType:uint16_t{
		
		NONE_TYPE = 0,
		START_TYPE,//next0 EPSILON, next1 EMPTY
		END_TYPE,  //next0 EMPTY, next1 EMPTY
		SINGLE_TYPE,
		BOTH_TYPE,
	};
	RegularNFANode(void) = delete;
	RegularNFANode(NodeType type, int prop0 = EDGE_PROP_EMPTY, int prop1 = EDGE_PROP_EMPTY)
	:next0(nullptr),next1(nullptr)
	{

		node_type = type;
		edge0_prop = prop0;
		edge1_prop = prop1;

	}
	RegularNFANode(const RegularNFANode& node)
	{

	}
	RegularNFANode(const RegularNFANode&& node)
	{

	}

	std::shared_ptr<RegularNFANode> next0;
	std::shared_ptr<RegularNFANode> next1;
	
	NodeType node_type;
	//-1 EPSILON_PROP
	//-2 EMPTY_PROP
	int edge0_prop;
	int edge1_prop;
};

struct RegularNFANodeFragment
{	
	RegularNFANodeFragment(std::shared_ptr<RegularNFANode> start, std::shared_ptr<RegularNFANode> * next)
	:fragment_next(nullptr)
	{

		fragment_start = start;
		fragment_next = next;
	}
	std::shared_ptr<RegularNFANode> fragment_start;
	
	std::shared_ptr<RegularNFANode> * fragment_next;
};

int8_t ConvertRPNRegularToNFA(const std::string &rpn_reg_str, std::shared_ptr<RegularNFANode> &root, std::shared_ptr<RegularNFANode> &end){

	std::shared_ptr<RegularNFANode> & _tmp_input_node = root;
	std::stack<RegularNFANodeFragment> _stack;

	for(int _i = 0; _i < rpn_reg_str.length(); _i++){//process element

		if (is_normal_element(rpn_reg_str, _i)){

			std::shared_ptr<RegularNFANode>  _tmp_node0 = std::make_shared<RegularNFANode>(RegularNFANode::SINGLE_TYPE, rpn_reg_str[_i]);
			_stack.push(RegularNFANodeFragment(_tmp_node0, &_tmp_node0->next0));
		}
		else if (rpn_reg_str[_i] == '+'){
			
			RegularNFANodeFragment  _tmp_fragment = _stack.top();
			_stack.pop();

			std::shared_ptr<RegularNFANode>  _tmp_node1 = std::make_shared<RegularNFANode>(RegularNFANode::BOTH_TYPE, EDGE_PROP_EPSILON, EDGE_PROP_EPSILON);


		}
		else if (rpn_reg_str[_i] == '?'){
			
		}
		else if (rpn_reg_str[_i] == '*'){
			
		}
		else if (rpn_reg_str[_i] == ','){
			
			RegularNFANodeFragment  _tmp_fragment1 = _stack.top();
			_stack.pop();	
			RegularNFANodeFragment  _tmp_fragment0 = _stack.top();
			_stack.pop();		

			//connect fg0 fg1
			*_tmp_fragment0.fragment_next = _tmp_fragment1.fragment_start;
			

			_stack.push(RegularNFANodeFragment(_tmp_fragment0.fragment_start, _tmp_fragment1.fragment_next));

		}
		else if (rpn_reg_str[_i] == '|'){

			std::shared_ptr<RegularNFANode>  _tmp_node_a = _stack.top();
			_stack.pop();
			std::shared_ptr<RegularNFANode>  _tmp_node_b = _stack.top();
			_stack.pop();			

			_tmp_input_node->next0 = _tmp_node_a;
			_tmp_input_node->next1 = _tmp_node_b;
			_tmp_input_node = _tmp_node_b;			
		}
		else{

			std::cout<<"ConvertRPNRegularToNFA: Don't support op ......"<<std::endl;
		}
		

	}

	return 0;
}


int main(int argc, char * argv[])
{
	// std::shared_ptr<RegularASTNode> _root = std::make_shared<RegularASTNode>(RegularASTNode::NONE_TYPE, 0);


	//RegularToAST("abc(def|gh)", _root);
	//PrintBinaryTree(_root);
	std::string _input = "ad+c|d*";
	std::cout<<"Input: "<<_input<<std::endl;
	MyFormatRegularExpression(_input);
	std::cout<<"Format: "<<_input<<std::endl;
	std::string _ret;
	RegularExpressionToPostfixExpression(_input, _ret);
	std::cout<<"Ret: "<<_ret<<std::endl;

	std::shared_ptr<RegularNFANode> _root = std::make_shared<RegularNFANode>(RegularNFANode::START_TYPE, -1, -2);//next0 EPSILON, next1 EMPTY
	std::shared_ptr<RegularNFANode> _end = std::make_shared<RegularNFANode>(RegularNFANode::START_TYPE, -2, -2);//next0 EMPTY, next1 EMPTY
	ConvertRPNRegularToNFA(_ret, _root, _end);
	return 0;
}


