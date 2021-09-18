/*
 * @Author: Sky
 * @Date: 2021-09-13 16:29:50
 * @LastEditors: Sky
 * @LastEditTime: 2021-09-18 11:06:49
 * @Description: 
 */

// sudo apt install python3-dev
// pip3 install pybind11
#include "pybind11/pybind11.h"


#include "ylib.hpp"

namespace py = pybind11;


class yLogPyWrapper{

    public:
    static void I(const std::string &fmt , py::args args) noexcept;
};

void yLogPyWrapper::I(const std::string &fmt , py::args args) noexcept{

    
    yLib::yLog::I(fmt, args[0]);
}


void test(int a, ...){}
PYBIND11_MODULE(yLib, m) {

    py::class_<yLib::yClassInfo<yLib::yObject>>(m, "yObjectClassInfo")
    .def_readwrite("class_name", &yLib::yClassInfo<yLib::yObject>::class_name);

    py::class_<yLib::yObject>(m, "yObject")
    .def(py::init<>())
    .def_static("yLibGetClassInfo", static_cast<const yLib::yClassInfo<yLib::yObject>&(*)()>(&yLib::yObject::yLibGetClassInfo), "yLib::yObject::yLibGetClassInfo(void)");
    
    // py::class_<yLib::yClassInfo<yLib::yLog>>(m, "yLogClassInfo")
    // .def_readwrite("class_name", &yLib::yClassInfo<yLib::yLog>::class_name);

    py::class_<yLib::yLog, std::unique_ptr<yLib::yLog, py::nodelete>>(m, "yLog");
    // .def_static("I", static_cast<void (*)(const std::string &, ...)noexcept>(&yLib::yLog::I), "yLib::yLog::I(const std::string &fmt , ...)");
    // .def_static("I", static_cast<void (*)(const std::string &, const std::string & , ...)>(&yLib::yLog::I), "yLib::yLog::I(const std::string &tag, const std::string &fmt , ...)")
    // .def_static("I", static_cast<void (*)(const char * , const char * )>(&yLib::yLog::I), "yLib::yLog::I(const char *fmt , const char * str)")
    
    // .def_static("D", static_cast<void (*)(const std::string &, ...)>(&yLib::yLog::D), "yLib::yLog::D(const std::string &fmt , ...)")
    // .def_static("D", static_cast<void (*)(const std::string &, const std::string & , ...)>(&yLib::yLog::D), "yLib::yLog::D(const std::string &tag, const std::string &fmt , ...)")
    // .def_static("D", static_cast<void (*)(const char * , const char * )>(&yLib::yLog::D), "yLib::yLog::D(const char *fmt , const char * str)")
    
    // .def_static("W", static_cast<void (*)(const std::string &, ...)>(&yLib::yLog::W), "yLib::yLog::W(const std::string &fmt , ...)")
    // .def_static("W", static_cast<void (*)(const std::string &, const std::string & , ...)>(&yLib::yLog::W), "yLib::yLog::W(const std::string &tag, const std::string &fmt , ...)")
    // .def_static("W", static_cast<void (*)(const char * , const char * )>(&yLib::yLog::W), "yLib::yLog::W(const char *fmt , const char * str)")
    
    // .def_static("E", static_cast<void (*)(const std::string &, ...)>(&yLib::yLog::E), "yLib::yLog::E(const std::string &fmt , ...)")
    // .def_static("E", static_cast<void (*)(const std::string &, const std::string & , ...)>(&yLib::yLog::E), "yLib::yLog::E(const std::string &tag, const std::string &fmt , ...)")
    // .def_static("E", static_cast<void (*)(const char * , const char * )>(&yLib::yLog::E), "yLib::yLog::E(const char *fmt , const char * str)");


    m.doc() = "yLib collects , collates and verifies some useful knowledges in my daily-work."; // optional module docstring

    // m.def("test", static_cast<void (*)(int, ...)>(&test), "");
    // m.def("test", &test, "A function which adds two numbers");
}