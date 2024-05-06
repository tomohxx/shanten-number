#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "calsht.hpp"
#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)
namespace py = pybind11;

PYBIND11_MODULE(shanten_number, m)
{
  py::class_<Calsht>(m, "Calsht")
      .def(py::init<>())
      .def("initialize", &Calsht::initialize)
      .def("__call__", &Calsht::operator());

#ifdef VERSION_INFO
  m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
  m.attr("__version__") = "dev";
#endif
}
