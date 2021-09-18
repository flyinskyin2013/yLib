'''
Author: Sky
Date: 2021-09-14 10:24:14
LastEditors: Sky
LastEditTime: 2021-09-14 10:50:11
Description: 
'''

from setuptools import setup

# Available at setup time due to pyproject.toml
from pybind11.setup_helpers import Pybind11Extension, build_ext
from pybind11 import get_cmake_dir

import sys

__version__ = "0.1.0"

# The main interface is through Pybind11Extension.
# * You can add cxx_std=11/14/17, and then build_ext can be removed.
# * You can set include_pybind11=false to add the include directory yourself,
#   say from a submodule.
#
# Note:
#   Sort input source files if you glob sources to ensure bit-for-bit
#   reproducible builds (https://github.com/pybind/python_example/pull/53)

ext_modules = [
    Pybind11Extension("yLib",
        ["ylib.cpp"],
        # Example: passing in the version to the compiled code
        # define_macros = [('VERSION_INFO', __version__)],
        extra_compile_args = ["-I /home/sky/yLib/build_gcc_x64/install/include/yLib"],
        cxx_std=11,
        include_dirs=["/home/sky/yLib/build_gcc_x64/install/include/yLib"]
        ),
]


setup(
    name="yLib",
    version=__version__,
    author="flyinskyin2013",
    author_email="iflyinskyin2013@gmail.com",
    url="https://github.com/flyinskyin2013/yLib",
    description="yLib for flyinskyin2013",
    long_description="yLib collects , collates and verifies some useful knowledges in my daily-work.",
    ext_modules=ext_modules,
    extras_require={"test": "pytest"},
    # Currently, build_ext only provides an optional "highest supported C++
    # level" feature, but in the future it may provide more features.
    cmdclass={"build_ext": build_ext},
    zip_safe=False,
)