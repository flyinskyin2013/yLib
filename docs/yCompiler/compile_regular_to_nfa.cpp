/*
 * @Author: Sky
 * @Date: 2021-03-22 15:57:39
 * @LastEditors: Sky
 * @LastEditTime: 2021-04-08 15:56:24
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
#include <list>
#include <map>
#include <set>

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

	std::string GetTypeString(NodeType t){

		switch (t)
		{
		case NONE_TYPE:{
			/* code */
			return std::string("NONE_TYPE");
			break;
		}
		case START_TYPE:{
			/* code */
			return std::string("START_TYPE");
			break;
		}
		case END_TYPE:{
			/* code */
			return std::string("END_TYPE");
			break;
		}
		case SINGLE_TYPE:{
			/* code */
			return std::string("SINGLE_TYPE");
			break;
		}	
		case BOTH_TYPE:{
			/* code */
			return std::string("BOTH_TYPE");
			break;
		}	
		default:
			break;
		}
		return std::string("ERROR_TYPE");
	} 

	RegularNFANode(void) = delete;
	RegularNFANode(NodeType type, int prop0 = EDGE_PROP_EMPTY, \
		const std::shared_ptr<RegularNFANode>& n0 = nullptr, \
		int prop1 = EDGE_PROP_EMPTY, \
		const std::shared_ptr<RegularNFANode>& n1 = nullptr
		)
	:is_acc(false)
	{

		node_type = type;
		edge0_prop = prop0;
		edge1_prop = prop1;

		next0 = n0;
		next1 = n1;

	}
	RegularNFANode(const RegularNFANode& node)
	{
		node_type = node.node_type;
		edge0_prop = node.edge0_prop;
		edge1_prop = node.edge1_prop;

		next0 = node.next0;
		next1 = node.next1;
	}
	RegularNFANode(const RegularNFANode&& node)
	{
		node_type = node.node_type;
		edge0_prop = node.edge0_prop;
		edge1_prop = node.edge1_prop;

		next0 = node.next0;
		next1 = node.next1;
	}

	std::shared_ptr<RegularNFANode> next0;
	std::shared_ptr<RegularNFANode> next1;
	
	NodeType node_type;
	//-1 EPSILON_PROP
	//-2 EMPTY_PROP
	int edge0_prop;
	int edge1_prop;

	bool is_acc;
};

struct RegularNFANodeFragment
{	
	RegularNFANodeFragment(const std::shared_ptr<RegularNFANode> &start, const std::vector<std::shared_ptr<RegularNFANode> *> &next_vec)
	{

		fragment_start = start;
		fragment_next_vec = next_vec;
	}
	std::shared_ptr<RegularNFANode> fragment_start;
	
	std::vector<std::shared_ptr<RegularNFANode> *> fragment_next_vec;
};


std::vector<std::shared_ptr<RegularNFANode> *> AppendFGVec(const std::vector<std::shared_ptr<RegularNFANode> *> & vec0, const std::vector<std::shared_ptr<RegularNFANode> *> &vec1){

	std::vector<std::shared_ptr<RegularNFANode> *> _tmp_vec;

	for(auto _n:vec0){

		_tmp_vec.push_back(_n);
	}

	for(auto _n:vec1){

		_tmp_vec.push_back(_n);
	}

	return _tmp_vec;
}


void PatchFGVec(std::vector<std::shared_ptr<RegularNFANode> *> & vec0, std::shared_ptr<RegularNFANode> & node){

	for(auto _n:vec0){

		*_n = node;
	}
}

std::vector<std::shared_ptr<RegularNFANode> *> CreateFGVec(std::shared_ptr<RegularNFANode> & node)
{

	std::vector<std::shared_ptr<RegularNFANode> *> _tmp_vec;
	_tmp_vec.push_back(&node);

	return _tmp_vec;
}


int8_t ConvertRPNRegularToNFA(const std::string &rpn_reg_str, std::shared_ptr<RegularNFANode> &root, std::shared_ptr<RegularNFANode> &end){

	std::shared_ptr<RegularNFANode> & _tmp_input_node = root;
	std::stack<RegularNFANodeFragment> _stack;

	for(int _i = 0; _i < rpn_reg_str.length(); _i++){//process element

		if (is_normal_element(rpn_reg_str, _i)){

			std::shared_ptr<RegularNFANode>  _tmp_node0 = std::make_shared<RegularNFANode>(RegularNFANode::SINGLE_TYPE, rpn_reg_str[_i]);
			
			_stack.push(RegularNFANodeFragment(_tmp_node0, CreateFGVec(_tmp_node0->next0)));
		}
		else if (rpn_reg_str[_i] == '+'){
			
			RegularNFANodeFragment  _tmp_fragment = _stack.top();
			_stack.pop();

			std::shared_ptr<RegularNFANode>  _tmp_node1 = std::make_shared<RegularNFANode>(RegularNFANode::BOTH_TYPE, \
				EDGE_PROP_EPSILON, \
				_tmp_fragment.fragment_start ,\
				EDGE_PROP_EPSILON);

			//add node to next vec
			PatchFGVec(_tmp_fragment.fragment_next_vec, _tmp_node1);


			_stack.push(RegularNFANodeFragment(_tmp_fragment.fragment_start, CreateFGVec(_tmp_node1->next1)));

		}
		else if (rpn_reg_str[_i] == '?'){

			RegularNFANodeFragment  _tmp_fragment = _stack.top();
			_stack.pop();

			std::shared_ptr<RegularNFANode>  _tmp_node1 = std::make_shared<RegularNFANode>(RegularNFANode::BOTH_TYPE, \
				EDGE_PROP_EPSILON, \
				_tmp_fragment.fragment_start ,\
				EDGE_PROP_EPSILON);

			//add node to next vec
			//PatchFGVec(_tmp_fragment.fragment_next_vec, _tmp_node1);

			_stack.push(RegularNFANodeFragment(_tmp_node1, AppendFGVec(_tmp_fragment.fragment_next_vec, CreateFGVec(_tmp_node1->next1))));

		}
		else if (rpn_reg_str[_i] == '*'){
			
			RegularNFANodeFragment  _tmp_fragment = _stack.top();
			_stack.pop();

			std::shared_ptr<RegularNFANode>  _tmp_node1 = std::make_shared<RegularNFANode>(RegularNFANode::BOTH_TYPE, \
				EDGE_PROP_EPSILON, \
				_tmp_fragment.fragment_start ,\
				EDGE_PROP_EPSILON);

			//add node to next vec
			PatchFGVec(_tmp_fragment.fragment_next_vec, _tmp_node1);

			_stack.push(RegularNFANodeFragment(_tmp_node1, CreateFGVec(_tmp_node1->next1)));			
		}
		else if (rpn_reg_str[_i] == ','){
			
			RegularNFANodeFragment  _tmp_fragment1 = _stack.top();
			_stack.pop();	
			RegularNFANodeFragment  _tmp_fragment0 = _stack.top();
			_stack.pop();		

			//connect fg0 fg1
			PatchFGVec(_tmp_fragment0.fragment_next_vec ,_tmp_fragment1.fragment_start);
			

			_stack.push(RegularNFANodeFragment(_tmp_fragment0.fragment_start, _tmp_fragment1.fragment_next_vec));

		}
		else if (rpn_reg_str[_i] == '|'){

			RegularNFANodeFragment  _tmp_fragment1 = _stack.top();
			_stack.pop();	
			RegularNFANodeFragment  _tmp_fragment0 = _stack.top();
			_stack.pop();		


			std::shared_ptr<RegularNFANode>  _tmp_node1 = std::make_shared<RegularNFANode>(RegularNFANode::BOTH_TYPE, \
				EDGE_PROP_EPSILON, \
				_tmp_fragment0.fragment_start ,\
				EDGE_PROP_EPSILON, \
				_tmp_fragment1.fragment_start );

			_stack.push(RegularNFANodeFragment(_tmp_node1, AppendFGVec(_tmp_fragment0.fragment_next_vec, _tmp_fragment1.fragment_next_vec)));		
		}
		else{

			std::cout<<"ConvertRPNRegularToNFA: Don't support op ......"<<std::endl;
		}
		
	}

	RegularNFANodeFragment  _tmp_fragment1 = _stack.top();
	_stack.pop();	

	PatchFGVec(_tmp_fragment1.fragment_next_vec, end);//connect to end

	root->next0 = _tmp_fragment1.fragment_start;//connect to start
	return 0;
}


void PrintNFA(std::shared_ptr<RegularNFANode> & nfa){

	//wfs
	std::queue<std::shared_ptr<RegularNFANode>> _nfa_node_queue;
	_nfa_node_queue.push(nfa);


	while(_nfa_node_queue.size() != 0){

		printf("layer+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");	
		int _cur_layer_size = _nfa_node_queue.size();
		for (int _i = 0; _i < _cur_layer_size; _i++){

			std::shared_ptr<RegularNFANode> & _cur_node = _nfa_node_queue.front();
			
			if (_cur_node->node_type == RegularNFANode::SINGLE_TYPE){
				
				if (_cur_node->is_acc)
					printf("-----------------RegularNFANode node is accessed----------------\n");

				printf("RegularNFANode: type %s , node addr 0x%x\n", _cur_node->GetTypeString(_cur_node->node_type).c_str(), _cur_node.get());
				printf("RegularNFANode: Prop %c \n", _cur_node->edge0_prop);
				printf("RegularNFANode: next node addr 0x%x\n",_cur_node->next0.get());

				if (_cur_node->next0 != nullptr && !_cur_node->is_acc)
					_nfa_node_queue.push(_cur_node->next0);
					
				_cur_node->is_acc = true;
				printf("======\n");	
			}
			else if (_cur_node->node_type == RegularNFANode::BOTH_TYPE){

				if (_cur_node->is_acc)
					printf("-----------------RegularNFANode node is accessed----------------\n");
					
				printf("RegularNFANode: type %s, node addr 0x%x \n", "BOTH_TYPE", _cur_node.get());
				_cur_node->is_acc = true;

				if (_cur_node->next0 != nullptr && !_cur_node->next0->is_acc){

					printf("RegularNFANode: type BOTH_TYPE, next0 node addr 0x%x\n", _cur_node->next0.get());
					printf("RegularNFANode: type BOTH_TYPE, LastNode: type %s, Prop0 0x%x, Prop1 0x%x\n", \
						_cur_node->GetTypeString(_cur_node->next0->node_type).c_str(), \
						_cur_node->next0->edge0_prop, _cur_node->next0->edge1_prop);
						
					_nfa_node_queue.push(_cur_node->next0);
					printf("++++++\n");

				}

				if (_cur_node->next1 != nullptr && !_cur_node->next1->is_acc){
					
					printf("RegularNFANode: type BOTH_TYPE, next1 node addr 0x%x \n", _cur_node->next1.get());
					printf("RegularNFANode: type BOTH_TYPE, LastNode: type %s, Prop0 0x%x, Prop1 0x%x\n", \
						_cur_node->GetTypeString(_cur_node->next1->node_type).c_str(), \
						_cur_node->next1->edge0_prop, _cur_node->next1->edge1_prop);
						
					_nfa_node_queue.push(_cur_node->next1);
					printf("++++++\n");	
				}	
				
				printf("======\n");	
			}
			else if (_cur_node->node_type == RegularNFANode::START_TYPE){

				if (_cur_node->is_acc)
					printf("-----------------RegularNFANode node is accessed----------------\n");
					
				printf("RegularNFANode: type %s , node addr 0x%x\n", "START_TYPE", _cur_node.get());
				printf("RegularNFANode: next node addr 0x%x\n",_cur_node->next0.get());
				_cur_node->is_acc = true;

				if (_cur_node->next0 != nullptr)
					_nfa_node_queue.push(_cur_node->next0);		
				
				printf("======\n");

				
			}
			else if (_cur_node->node_type == RegularNFANode::END_TYPE){

				if (_cur_node->is_acc)
					printf("-----------------RegularNFANode node is accessed----------------\n");
					
				printf("RegularNFANode: type %s , node addr 0x%x\n", "END_TYPE", _cur_node.get());
				printf("RegularNFANode: next node addr 0x%x\n",_cur_node->next0.get());
				_cur_node->is_acc = true;
				printf("======\n");
			}
			else{

				printf("Invalid RegularNFANode type .\n");
			}

			_nfa_node_queue.pop();
		}
	}


	return;
}

void AddState(std::map<uintptr_t, std::shared_ptr<RegularNFANode>> & state_map, std::shared_ptr<RegularNFANode> & node)
{

	if (nullptr == node)
		return;
	
	if (state_map.find((uintptr_t)node.get()) == state_map.end()){//not found node


		if (node->node_type == RegularNFANode::BOTH_TYPE){

			AddState(state_map, node->next0);
			AddState(state_map, node->next1);
		}
		else//not both type
			state_map.insert(std::make_pair((uintptr_t)node.get(), node));
	}
}

void StepState(std::map<uintptr_t, std::shared_ptr<RegularNFANode>> & cur_map, std::map<uintptr_t, std::shared_ptr<RegularNFANode>> & next_map, char c)
{
	next_map.clear();
	
	for(auto _c_p:cur_map){

		if (_c_p.second->node_type == RegularNFANode::SINGLE_TYPE){

			if (_c_p.second->edge0_prop == c){

				AddState(next_map, _c_p.second->next0);
			}
		}
		else if (_c_p.second->node_type == RegularNFANode::BOTH_TYPE){

			if (_c_p.second->edge1_prop == c){

				AddState(next_map, _c_p.second->next0);
			}

			if (_c_p.second->edge1_prop == c){

				AddState(next_map, _c_p.second->next1);
			}			
		}
	}
}

bool IsMatch(std::map<uintptr_t, std::shared_ptr<RegularNFANode>> & map)
{

	for (auto _c_p: map){

		if (_c_p.second->node_type == RegularNFANode::END_TYPE){

			return true;
		}
	}

	return false;
}

bool SimulateNFA(const std::string & in_str, std::shared_ptr<RegularNFANode> & start_node)
{
	//key is the addr of std::shared_ptr<RegularNFANode>::get()
	std::map<uintptr_t, std::shared_ptr<RegularNFANode>> _cur_state_map;
	std::map<uintptr_t, std::shared_ptr<RegularNFANode>> _next_state_map;

	AddState(_cur_state_map, start_node->next0);//input state to start_node
	
	for(auto c : in_str){

		StepState(_cur_state_map, _next_state_map, c);

		_cur_state_map.swap(_next_state_map);
	}


	return IsMatch(_cur_state_map);
}


#define MAX_REGULAR_NFA_NODE_TYPE 300

struct RegularDFANode
{
	/* data */
	RegularDFANode()
	:next(nullptr){
		dfa_content_vec.resize(MAX_REGULAR_NFA_NODE_TYPE, nullptr);
	}
	~RegularDFANode(){}

	//copy cst
	//move cst

	std::vector<std::shared_ptr<RegularNFANode>> dfa_content_vec;

	std::shared_ptr<RegularDFANode> next;
};


void BuildDFAByNFA(std::shared_ptr<RegularNFANode> & start_nfa_node, std::shared_ptr<RegularDFANode> & start_dfa_node)
{

	//key is the addr of std::shared_ptr<RegularNFANode>::get()
	std::map<uintptr_t, std::shared_ptr<RegularNFANode>> _cur_state_map;
	std::map<uintptr_t, std::shared_ptr<RegularNFANode>> _next_state_map;

	AddState(_cur_state_map, start_nfa_node->next0);//input state to start_node

	std::shared_ptr<RegularDFANode> & _tmp_dfa_node = start_dfa_node;

	while(1){



	}

}

bool SimulateDFA(const std::string & in_str, std::shared_ptr<RegularNFANode> & start_node)
{


	


	return true;
}


int main(int argc, char * argv[])
{
	// std::shared_ptr<RegularASTNode> _root = std::make_shared<RegularASTNode>(RegularASTNode::NONE_TYPE, 0);


	//RegularToAST("abc(def|gh)", _root);
	//PrintBinaryTree(_root);
	//std::string _input = "ad+c|d*";
	std::string _input = "a*b*c*|_a*b*c*";
	std::cout<<"Input: "<<_input<<std::endl;
	MyFormatRegularExpression(_input);
	std::cout<<"Format: "<<_input<<std::endl;
	std::string _ret;
	RegularExpressionToPostfixExpression(_input, _ret);
	std::cout<<"Ret: "<<_ret<<std::endl;

	std::shared_ptr<RegularNFANode> _root = std::make_shared<RegularNFANode>(RegularNFANode::START_TYPE, EDGE_PROP_EPSILON, nullptr);//next0 EPSILON, next1 EMPTY
	std::shared_ptr<RegularNFANode> _end = std::make_shared<RegularNFANode>(RegularNFANode::END_TYPE);//next0 EMPTY, next1 EMPTY
	ConvertRPNRegularToNFA(_ret, _root, _end);

	//
	//PrintNFA(_root);

	std::string _ret_str0 = SimulateNFA("_aaaabbbbbbcccc", _root)?("IsMatch"):("NotMatch");
	std::cout<< _ret_str0 <<std::endl;

	std::shared_ptr<RegularDFANode> _start_dfa = std::make_shared<RegularDFANode>();
	_start_dfa->dfa_content_vec.push_back(_root);
	BuildDFAByNFA(_root, _start_dfa);

	std::string _ret_str1 = SimulateDFA("_aaaabbbbbbcccc", _root)?("IsMatch"):("NotMatch");
	std::cout<< _ret_str1 <<std::endl;

	return 0;
}


