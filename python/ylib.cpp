/*
 * @Author: Sky
 * @Date: 2021-09-13 16:29:50
 * @LastEditors: Sky
 * @LastEditTime: 2021-09-24 17:48:48
 * @Description: 
 */

// sudo apt install python3-dev
// pip3 install pybind11
#include "pybind11/pybind11.h"

#include "ylib.hpp"

namespace py = pybind11;

class yLogPyWrapper{

    public:
    static void I(const std::string &fmt, py::args args) noexcept;
    static void I(const std::string &tag, const std::string &fmt, py::args args) noexcept{}

    static void W(const std::string &fmt, py::args args) noexcept{}
    static void W(const std::string &tag, const std::string &fmt, py::args args) noexcept{}

    static void D(const std::string &fmt, py::args args) noexcept{}
    static void D(const std::string &tag, const std::string &fmt, py::args args) noexcept{}

    static void E(const std::string &fmt, py::args args) noexcept{}
    static void E(const std::string &tag, const std::string &fmt, py::args args) noexcept{}
};

void yLogPyWrapper::I(const std::string &fmt, py::args args) noexcept{

    ssize_t _args_num = args.size();
    for (int _i = 0; _i < _args_num; _i++){

    }
    yLib::yLog::I("test yLogPyWrapper::I");
}


PYBIND11_MODULE(yLib, m) {

    //define module
    py::module sub_core_m = m.def_submodule("core");
    py::module sub_ipc_m = m.def_submodule("ipc");
    py::module sub_network_m = m.def_submodule("network");
    py::module sub_utility_m = m.def_submodule("utility");

    py::class_<yLib::yClassInfo<yLib::yObject>>(sub_core_m, "yObjectClassInfo")
    .def_readwrite("class_name", &yLib::yClassInfo<yLib::yObject>::class_name);

    py::class_<yLib::yObject>(sub_core_m, "yObject")
    .def(py::init<>())
    .def_static("yLibGetClassInfo", static_cast<const yLib::yClassInfo<yLib::yObject>&(*)()>(&yLib::yObject::yLibGetClassInfo), "yLib::yObject::yLibGetClassInfo(void)");
    

    py::class_<yLib::yLog, std::unique_ptr<yLib::yLog, py::nodelete>>(sub_utility_m, "yLog")
    .def_static("I", static_cast<void (*)(const std::string &, py::args)noexcept>(&yLogPyWrapper::I), "yLib::yLog::I(const std::string &fmt , py::args args)")
    .def_static("I", static_cast<void (*)(const std::string &, const std::string & , py::args)>(&yLogPyWrapper::I), "yLib::yLog::I(const std::string &tag, const std::string &fmt , py::args args)");


    m.doc() = "yLib collects , collates and verifies some useful knowledges in my daily-work."; // optional module docstring
}