# Available at setup time due to pyproject.toml
from pybind11.setup_helpers import Pybind11Extension, build_ext  # type: ignore
from setuptools import setup  # type: ignore

__version__ = "1.0.0"

ext_modules = [
    Pybind11Extension(
        "shanten_number",
        sources=["src/calsht.cpp", "src/python.cpp"],
        cxx_std=17,
        define_macros=[("VERSION_INFO", __version__)],
        extra_compile_args=["-O3"],
    ),
]

setup(
    name="shanten_number",
    version=__version__,
    author="tomohxx",
    description="Shanten Number",
    long_description="",
    ext_modules=ext_modules,
    cmdclass={"build_ext": build_ext},
    zip_safe=False,
    python_requires=">=3.7",
)
