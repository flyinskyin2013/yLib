import os
import glob
import subprocess
import sys
import argparse

def find_lib_files(directory, prefixes):
    """
    在指定目录下查找以给定前缀开头的 .lib 文件。
    
    Args:
        directory (str): 搜索的目录路径。
        prefixes (list): 库文件名的前缀列表，例如 ["LLVM", "clang"]。
    
    Returns:
        list: 找到的 .lib 文件路径列表。
    """
    found_libs = []
    # 确保目录存在
    if not os.path.isdir(directory):
        print(f"错误: 目录 '{directory}' 不存在。")
        return []

    for prefix in prefixes:
        # 使用 glob 模式匹配文件
        pattern = os.path.join(directory, f"{prefix}*.lib")
        # glob.glob 返回匹配的文件路径列表
        found_libs.extend(glob.glob(pattern))
    
    return found_libs

def merge_libs_with_msvc(lib_files, output_lib_path):
    """
    使用 MSVC 的 lib.exe 工具合并静态库。

    Args:
        lib_files (list): 要合并的 .lib 文件路径列表。
        output_lib_path (str): 输出的新 .lib 文件路径。
    """
    if not lib_files:
        print("没有找到需要合并的库文件。")
        return

    # 构造 lib.exe 命令
    # lib.exe /OUT:NewLibrary.lib Library1.lib Library2.lib ...
    command = ["lib.exe", f"/OUT:{output_lib_path}"] + lib_files
    print(command)

    print(f"正在合并以下库文件到 '{output_lib_path}'...")
    for lib in lib_files:
        print(f"  - {os.path.basename(lib)}")

    # 调用 subprocess 来执行命令
    # 注意：为了让 lib.exe 可用，这个脚本必须在 Visual Studio 命令提示符下运行，
    # 或者你必须手动配置环境变量。
    try:
        # Popen 用于创建子进程，捕获其输出
        process = subprocess.Popen(command, 
                                     stdout=subprocess.PIPE, 
                                     stderr=subprocess.PIPE, 
                                     text=True, 
                                     shell=True)
        stdout, stderr = process.communicate()

        if process.returncode == 0:
            print("\n库合并成功！")
            print(f"新生成的库文件路径：{os.path.abspath(output_lib_path)}")
            # 打印 lib.exe 的输出
            if stdout:
                print("\n--- lib.exe 输出 ---")
                print(stdout)
        else:
            print(f"\n合并失败，lib.exe 退出码: {process.returncode}")
            if stderr:
                print("\n--- lib.exe 错误信息 ---")
                print(stderr)

    except FileNotFoundError:
        print("错误: 找不到 lib.exe。请确保你在 Visual Studio 命令提示符下运行此脚本，或已将它添加到系统 PATH。")
    except Exception as e:
        print(f"执行命令时发生异常 {e}")


def merge_libs_with_msvc_using_rsp(lib_files, output_lib_path):
    if not lib_files:
        print("没有找到需要合并的库文件。")
        return

    # 创建一个临时的响应文件
    rsp_file_name = "merge_libs_temp.rsp"
    try:
        with open(rsp_file_name, "w") as rsp_file:
            # 写入 /OUT 参数
            rsp_file.write(f"/OUT:{output_lib_path}\n")
            # 写入所有库文件名
            for lib in lib_files:
                rsp_file.write(f'"{lib}"\n') # 使用引号处理路径中的空格

        print(f"正在使用响应文件 '{rsp_file_name}' 合并库文件...")
        # 构造 lib.exe 命令，使用 @ 引用响应文件
        command = ["lib.exe", f"@{rsp_file_name}"]
        
        # 调用 subprocess 来执行命令
        process = subprocess.Popen(command, 
                                     stdout=subprocess.PIPE, 
                                     stderr=subprocess.PIPE, 
                                     text=True, 
                                     shell=True)
        stdout, stderr = process.communicate()

        if process.returncode == 0:
            print("\n库合并成功！")
            print(f"新生成的库文件路径：{os.path.abspath(output_lib_path)}")
        else:
            print(f"\n合并失败，lib.exe 退出码: {process.returncode}")
            if stderr:
                print("\n--- lib.exe 错误信息 ---")
                print(stderr)

    except Exception as e:
        print(f"执行命令时发生异常 {e}")
    finally:
        # 清理临时响应文件
        if os.path.exists(rsp_file_name):
            os.remove(rsp_file_name)
            print(f"\n已删除临时响应文件'{rsp_file_name}'。")


def merge_library_set(directory: str, prefixes: list[str], output_name: str):
    """
    一个通用的函数，用于在指定目录中合并一组库文件。

    Args:
        directory (str): 库文件所在的目录。
        prefixes (list[str]): 库文件名的前缀列表。
        output_name (str): 合并后新库的名称。
    """
    print(f"\n{'='*50}\n开始合并以 {prefixes} 开头的库文件...\n")
    
    # 构造输出文件的完整路径
    output_path = os.path.join(directory, output_name)
    
    # 查找需要合并的库文件
    libs_to_combine = find_lib_files(directory, prefixes)
    
    if libs_to_combine:
        # 使用响应文件的合并函数
        merge_libs_with_msvc_using_rsp(libs_to_combine, output_path)
    else:
        print(f"在目录'{directory}' 中没有找到以 '{prefixes}' 开头的 .lib 文件。")
    
    print(f"\n合并 {prefixes} 的操作完成。\n{'='*50}\n")


if __name__ == "__main__":
    # 使用 argparse 来解析命令行参数
    parser = argparse.ArgumentParser(description="合并指定目录下的静态库文件。")
    parser.add_argument('directory', help="要合并的库文件所在的目录。")
    args = parser.parse_args()

    # --- 配置区域 ---
    # 你的库文件所在的目录
    # 现在从命令行参数中获取
    library_directory = args.directory
    # --- 配置区域结束 ---

    # 检查目录是否存在
    if not os.path.exists(library_directory):
        print(f"指定的目录不存在: {library_directory}")
        sys.exit(1)

    # 第一次调用：合并LLVM开头的库
    merge_library_set(
        directory=library_directory,
        prefixes=["LLVM"],
        output_name="LLVM.lib"
    )

    # 第二次调用：合并clang开头的库
    merge_library_set(
        directory=library_directory,
        prefixes=["clang"],
        output_name="clang.lib"
    )