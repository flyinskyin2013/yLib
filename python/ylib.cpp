/*
 * @Author: Sky
 * @Date: 2021-09-13 16:29:50
 * @LastEditors: Sky
 * @LastEditTime: 2021-09-13 18:22:20
 * @Description: 
 */

// sudo apt install python3-dev
// pip3 install pybind11
#include <pybind11/pybind11.h>


#include "ylib.hpp"

namespace py = pybind11;

void test(){}

PYBIND11_MODULE(yLib, m) {

    py::class_<yLib::yObject>(m, "yObject")
    .def(py::init<>());
    
    // py::class_<yLib::yLog, std::unique_ptr<yLib::yLog, py::nodelete>>(m, "yLog")
    // .def("I", static_cast<void (yLib::yLog::*)(const std::string &, ...)>(&yLib::yLog::I), "yLib::yLog::I(const std::string &fmt , ...)")
    // .def("I", static_cast<void (yLib::yLog::*)(const std::string &, const std::string & , ...)>(&yLib::yLog::I), "yLib::yLog::I(const std::string &tag, const std::string &fmt , ...)")
    // .def("I", static_cast<void (yLib::yLog::*)(const char * , const char * )>(&yLib::yLog::I), "yLib::yLog::I(const char *fmt , const char * str)");


    m.doc() = "pybind11 example plugin"; // optional module docstring

    // m.def("test", &test, "A function which adds two numbers");
}