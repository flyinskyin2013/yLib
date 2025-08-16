import os
import argparse
import subprocess
import json
from collections import deque

# find .hpp .h .cpp .c
def find_all_file(dir):
    find_files = []
    for root, dirs, files in os.walk(dir):
        for file in files:
            if file.endswith('.hpp') or file.endswith('.cpp') or file.endswith('.c') or file.endswith('.h'):
                # find_files.append(os.path.realpath(os.path.join(root, file)) + "\n")
                find_files.append("     \"" + file + "\", \n")
    return find_files

def find_all_src_file(dir):
    find_files = []
    for root, dirs, files in os.walk(dir):
        for file in files:
            if file.endswith('.cpp') or file.endswith('.c'):
                find_files.append(os.path.realpath(os.path.join(root, file)) )
    return find_files

def generate_ylibfiles_cpp(ylib_root):
    find_files = []

    find_files.extend(find_all_file(ylib_root + '/' + 'include'))
    find_files.extend(find_all_file(ylib_root + '/' + 'src'))

    with open('ylibfiles.cpp', 'w', encoding='utf-8') as file:
        file.write("#include <unordered_set>\n")
        file.write("static std::unordered_set<std::string> g_file_set = { \n")
        file.writelines(find_files)
        file.write("};\n")
    print(find_files)


def parser_class_ana(stdout_str):
    json_str = "{\"ROOT\" : [" + stdout_str[:-2] + "]}"
    # print(json_str)
    # f = open('t.txt', "w")
    # f.write(json_str)
    # f.close()

    jj = json.loads(json_str)
    # jj_root = jj["ROOT"]
    # print(jj_root[0])
    return jj

def find_matching_strings(search_list, search_string):
    """
    在一个列表中查找并打印包含特定子字符串的元素。
    
    Args:
        search_list (list): 要搜索的列表。
        search_string (str): 要匹配的子字符串。
    """
    for item in search_list:
        if search_string in item:
            print(f"找到匹配项：item = {item}, search_string = {search_string}")
            return True

    return False

def run_class_ana(input_file, ylib_root):
    third_part_build_dir = "build_linux_x64_class_ana"
    if os.name == "nt":
        search_list = ["socket", "tcp", "udp", "YMAIN"]
        if find_matching_strings(search_list, input_file) == True:
            print("we skip this file")
            return
        
        third_part_build_dir = "build_v143_x64"

    # 执行 ls 命令并捕获输出
    result = subprocess.run(["./class_ana", input_file, "--", \
                            "-I" + ylib_root + "/include/", \
                            "-I" + ylib_root + f"/third_part/{third_part_build_dir}/install/include", \
                            "-I" + ylib_root + f"/third_part/{third_part_build_dir}/install/include/libxml2", \
                            "-DBUILD_YLIB_WITH_EXPORT=1"], cwd='.', capture_output=True, text=True)

    # 获取标准输出和错误输出
    if result.returncode == 0:  # 判断命令是否成功执行
        print(f"class_ana({input_file}) 命令执行成功")
        # print(result.stdout)
        return parser_class_ana(result.stdout)

    else:
        print(f"class_ana({input_file}) 命令执行失败:")
        print(result.stdout)
        print(result.stderr)
        exit(-1)


class TreeNode:
    def __init__(self, value):
        self.value = value  # 节点的值
        self.children = []  # 子节点列表

    def add_child(self, child_node):
        """将子节点添加到当前节点的 children 列表中"""
        self.children.append(child_node)

    def __repr__(self):
        """定义节点的字符串表示形式"""
        return f"TreeNode({self.value})"
    
def build_recursive_tree(relations):
    tree = {}  # 用于存储节点对象，键是节点值，值是对应的 TreeNode 对象
    all_nodes = set()  # 所有出现的节点
    children = set()  # 所有作为子节点出现的节点

    # 构建初步树结构
    for child, parent in relations:
        # 如果父节点不存在，则创建父节点
        if parent not in tree:
            tree[parent] = TreeNode(parent)
        # 如果子节点不存在，则创建子节点
        if child not in tree:
            tree[child] = TreeNode(child)
        # 将子节点添加到父节点的 children 列表中
        tree[parent].add_child(tree[child])

        # 更新节点集合
        all_nodes.add(parent)
        all_nodes.add(child)
        children.add(child)

    # 找到根节点（不在 children 中的节点）
    root_nodes = [tree[node] for node in all_nodes if node not in children]

    return root_nodes

def print_tree(node, level=0):
    """递归打印树结构"""
    if node is None:
        return
    print("  " * level + str(node.value))
    for child in node.children:
        print_tree(child, level + 1)

def generate_ylib_enhance_cpp(ylib_root, output_dir):
    find_files = []

    # find all src files
    find_files.extend(find_all_src_file(ylib_root + '/' + 'include'))
    find_files.extend(find_all_src_file(ylib_root + '/' + 'src'))

    # 删除所有 "yregularengine"
    find_files = [item for item in find_files if item.find("yregularengine") == -1 ]

    # get class json-obj
    # key: class_name
    found_class_dict = {}
    for src_file in find_files:

        jj = run_class_ana(src_file, ylib_root)
        if jj is None:
            continue
        jj_root = jj["ROOT"]
        for obj in jj_root:
            if not obj["class_name"] in found_class_dict:
                found_class_dict[obj["class_name"]] = obj
        # break
    
    # build class tree
    # relations = <child, parent>
    relations = []
    for key in found_class_dict:
        # if "::" == found_class_dict[key]["class_type_key"]:
        #     print(found_class_dict[key])
        # print(found_class_dict[key]["class_type_key"])
        if found_class_dict[key]["class_base_class_num"] == 0:
            relations.append((key, "____ROOT"))
        else:
            for i in range(found_class_dict[key]["class_base_class_num"]):
                relations.append((key, found_class_dict[key]["class_base_class_name"][i]))
    
    root_nodes = build_recursive_tree(relations)
    custom_root_node = None
    class_info_num = [0]
    print(root_nodes)

    def dfs_get_class_num(node, num):
        if len(node.children) == 0:
            num[0] = num[0] + 1
            return
        else:
            for child_node in node.children:
                dfs_get_class_num(child_node, num)
            # skip our root-node
            if node.value != '____ROOT':
                num[0] = num[0] + 1

    for node in root_nodes:
        if node.value == "____ROOT":
            print(node.children)
            custom_root_node = node
            dfs_get_class_num(node, class_info_num)

    # node: class json obj
    def write_node_2_cpp(relations_node, node, class_counter, define_str, class_info_var_name_map):

        str_node_vec_init = ""

        # before current node, the base-class has been defined
        for relations_child_node in relations_node.children:
            # print(relations_child_node.value)
            cur_child_class_name = relations_child_node.value
            str_node_vec_init = str_node_vec_init + f"&{class_info_var_name_map[cur_child_class_name]},"


        current_class_name = node["class_name"]
        str_node_define_var = f"g_classinfo_{current_class_name}_{class_counter}"

        str_node_define = f"""
            static ClassInfo {str_node_define_var} = {{
                {{
                    {str_node_vec_init}
                }},
                "{node["class_type_key"]}",
                "{current_class_name}",
                {class_counter}
            }};
        """
        define_str[0] = define_str[0] + str_node_define


    # node: class json obj
    def reverse_order_loop_node(class_counter, define_str, class_info_var_name_map, relation_node_list):
        # 创建一个空队列
        C = class_counter
        print(f'class_counter = {class_counter}, len(relation_node_list) = {len(relation_node_list)}')
        while C != 0:

            cur_node = relation_node_list[C - 1]

            # skip our root-node
            if cur_node.value != '____ROOT':
                write_node_2_cpp(cur_node, found_class_dict[cur_node.value], C, define_str, class_info_var_name_map)

            C = C - 1

    def wfs_node_gen_map(root_node, class_counter, class_info_var_name_map, class_info_var_index_map, relation_node_list):
        # 创建一个空队列
        queue = deque()
        queue.append(root_node)
        while len(queue) != 0:
            cur_node = queue.popleft()
            if len(cur_node.children) != 0:
                for child_node in cur_node.children:
                    queue.append(child_node)

            # skip our root-node
            if cur_node.value != '____ROOT':
                class_counter[0] = class_counter[0] + 1
                node = found_class_dict[cur_node.value]
                current_class_name = node["class_name"]
                str_node_define_var = f"g_classinfo_{current_class_name}_{class_counter[0]}"
                class_info_var_name_map[current_class_name] = str_node_define_var
                class_info_var_index_map[class_counter[0]] = str_node_define_var
                relation_node_list.append(cur_node)


    output_file_path = os.path.join(output_dir, 'ylib_class_info.cpp')
    with open(output_file_path, 'w', encoding='utf-8') as file:

        str_include_header = """
            #include <vector>
            #include <unordered_map>
            #include <string>
            #include <array>
            #include <cstring>
            #include "core/ylib_class_info.hpp"
        """

        # str_node_define = """
        #     struct ClassInfo{
        #         std::vector<ClassInfo*> child_;
        #         const char* class_type_key_;
        #         const char* class_name_;
        #     };
        # """
        str_class_info_name_map_start = """

            //<typekey, classinfo>, exapmle: <"::yLib::yObject", yobject-class-info*>
            static std::unordered_map<std::string, ClassInfo*> g_class_info_name_map = {
        """
        str_class_info_name_map_end = """
            };
            std::unordered_map<std::string, ClassInfo*>& get_classinfo_name_map() {return g_class_info_name_map;}
        """

        str_class_info_index_map_start = """

            //<index, classinfo>, exapmle: <1, yobject-class-info*>
            static std::unordered_map<int32_t, ClassInfo*> g_class_info_index_map = {
        """
        str_class_info_index_map_end = """
            };
            std::unordered_map<int32_t, ClassInfo*>& get_classinfo_index_map() {return g_class_info_index_map;}
        """
        
        str_class_info_vec_start = """

            //<typekey, classinfo>, exapmle: <::yLib::yObject, yobject-class-info>
            static std::vector<ClassInfo*> g_class_info_vec = {
        """
        str_class_info_vec_end = """
            };
            std::vector<ClassInfo*>& get_classinfo_tree_root_vec() {return g_class_info_vec;}
        """

        # include header
        file.write(str_include_header)

        # declare yLib namespace start
        file.write("YLIB_NAMESPACE_START")

        class_counter = [0]
        class_info_var_name_map = {}
        class_info_var_index_map = {}
        relation_node_list = []
        wfs_node_gen_map(custom_root_node, class_counter, class_info_var_name_map, class_info_var_index_map, relation_node_list)


        define_str = [""]
        reverse_order_loop_node(class_counter[0], define_str, class_info_var_name_map, relation_node_list)
        # declare ClassInfo g_classinfo_xxxx_nnnnn;
        file.write(define_str[0])

        define_name_map_str = ""
        
        for key in class_info_var_name_map:
            class_type_key = found_class_dict[key]["class_type_key"]
            tmp = f"""
                {{ "{class_type_key}", &{class_info_var_name_map[key]} }},
            """
            define_name_map_str = define_name_map_str + tmp

        define_index_map_str = ""
        
        for key_index in class_info_var_index_map:

            tmp = f"""
                {{ {key_index}, &{class_info_var_index_map[key_index]} }},
            """
            define_index_map_str = define_index_map_str + tmp


        # declare <class_name, class_info> map
        file.write(str_class_info_name_map_start)
        file.write(define_name_map_str)
        file.write(str_class_info_name_map_end)

        # declare <class_index, class_info> map
        file.write(str_class_info_index_map_start)
        file.write(define_index_map_str)
        file.write(str_class_info_index_map_end)

        define_tree_root_vec_str = ""
        for node in custom_root_node.children:
            tmp = f"""
                &{class_info_var_name_map[node.value]}, 
            """
            define_tree_root_vec_str = define_tree_root_vec_str + tmp

        # declare class_info_tree's root-node
        file.write(str_class_info_vec_start)
        file.write(define_tree_root_vec_str)
        file.write(str_class_info_vec_end)


        # declare yLib namespace end
        file.write("YLIB_NAMESPACE_END")  

    # print(find_files)




if __name__ == '__main__':
    # 创建 ArgumentParser 对象
    parser = argparse.ArgumentParser(description="这是一个简单的命令行参数解析示例")

    parser.add_argument("--ylib_root", required=True, type=str, help="ylib 根目录")
    parser.add_argument("--output", default=".", type=str, help="输出目录")
 
    # 解析命令行参数
    args = parser.parse_args()
    ROOT = os.path.realpath(args.ylib_root)
    OUTPUT_DIR = os.path.realpath(args.output)
    print(f'ROOT = {ROOT}')
    print(f'OUTPUT_DIR = {OUTPUT_DIR}')
    
    generate_ylib_enhance_cpp(ROOT, OUTPUT_DIR)
