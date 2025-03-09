import os

from setuptools import Extension, find_packages, setup
from setuptools.command.install import install

ext_modules = [
    Extension(
        "quick_cliques.quick_cliques_cpp",
        sources=[
            "src/python_bindings.cpp",
            "src/CliqueTools.cpp",
            "src/MemoryManager.cpp",
            "src/Algorithm.cpp",
            "src/TomitaAlgorithm.cpp",
            "src/AdjacencyListAlgorithm.cpp",
            "src/HybridAlgorithm.cpp",
            "src/DegeneracyAlgorithm.cpp",
            "src/DegeneracyTools.cpp",
            "src/Tools.cpp",
        ],
        include_dirs=[
            "src/",
            os.path.join(
                os.path.dirname(os.path.abspath(__file__)), "pybind11/include"
            ),
        ],
        language="c++",
        extra_compile_args=["-O2", "-Winline", "-std=c++11"],
        define_macros=[
            ("PRINT_CLIQUES_ONE_BY_ONE", "1"),
            ("ALLOW_ALLOC_ZERO_BYTES", "1"),
            ("RETURN_CLIQUES_ONE_BY_ONE", "1"),
        ],
    ),
]


class InstallQuickCliques(install):
    """Custom install command to build the C++ Quick Cliques binaries."""

    def run(self):
        install.run(self)


packages = find_packages()
if "quick_cliques" in packages and "quick_cliques.bin" not in packages:
    packages.append("quick_cliques.bin")


setup(
    name="quick-cliques",
    version="0.1.0",
    author="Raphael Engel",
    author_email="hello AT alpharee DOT de",
    description="A Python module for finding maximal cliques in graphs",
    long_description=open(
        os.path.join(os.path.dirname(os.path.abspath(__file__)), "README.md")
    ).read(),
    long_description_content_type="text/markdown",
    url="https://github.com/dr-lego/quick-cliques-py",
    packages=packages,
    include_package_data=True,
    ext_modules=ext_modules,
    cmdclass={
        "install": InstallQuickCliques,
    },
    classifiers=[
        "Programming Language :: Python :: 3",
        "License :: OSI Approved :: GNU General Public License v3 (GPLv3)",
        "Operating System :: POSIX :: Linux",
    ],
    python_requires=">=3.6",
    install_requires=[
        "numpy",
        "pybind11>=2.6.0",
    ],
)
