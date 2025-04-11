import os
import sys
from copy import deepcopy
from platform import system, uname
from psutil import cpu_count
from subprocess import check_call
from pybind11.setup_helpers import Pybind11Extension, build_ext, WIN, MACOS
from setuptools import find_packages, setup

# Loop through command line arguments
debug_build = False
coverage_build = False
build_fenn_libs = True
filtered_args = []
for arg in sys.argv:
    if arg == "--debug":
        debug_build = True
    elif arg == "--coverage":
        coverage_build = True
        continue
    elif arg in ["clean", "egg_info", "sdist"]:
        build_fenn_libs = False

    filtered_args.append(arg)

# Add filtered (those that setuptools will understand) back to sys.argv
sys.argv = filtered_args

# Get GeNN path from environment variable
genn_path = os.environ.get("GENN_PATH")
if genn_path is None or not os.path.exists(genn_path):
    raise RuntimeError("GeNN is currently required to install FeNN")

# Are we on Linux?
# **NOTE** Pybind11Extension provides WIN and MAC
LINUX = system() == "Linux"

# Determine correct suffix for FeNN libraries
if WIN:
    lib_suffix = "_Debug_DLL" if debug_build else "_Release_DLL"
else:
    if debug_build:
        lib_suffix = "_dynamic_debug"
    else:
        ib_suffix = "_dynamic"

fenn_path = os.path.dirname(os.path.abspath(__file__))

pyfenn_path = os.path.join(fenn_path, "pyfenn")
pyfenn_src = os.path.join(pyfenn_path, "src")
fenn_include = os.path.join(fenn_path, "include")

genn_include = os.path.join(genn_path, "include", "genn", "genn")
genn_third_party_include = os.path.join(genn_path, "include", "genn", "third_party")
#genn_share = os.path.join(genn_path, "share", "genn")
#pygenn_share = os.path.join(pygenn_path, "share")

fenn_libraries = ["assembler", "backend", "common", "compiler", "ise"]
# Always package LibGeNN
if WIN:
    package_data = [f"genn{lib_suffix}.dll",
                    f"libffi{lib_suffix}.dll"]
    package_data += [f"{l}{lib_suffix}.dll" for l in fenn_libraries]
else:
    package_data = [f"libgenn{lib_suffix}.so"]
    package_data += [f"lib{l}{lib_suffix}.so" for l in fenn_libraries]

# Define standard kwargs for building all extensions
fenn_extension_kwargs = {
    "include_dirs": [fenn_include, genn_third_party_include, genn_include],
    "library_dirs": [pyfenn_path],
    "libraries": [f"genn{lib_suffix}"] + [f"{l}{lib_suffix}" for l in fenn_libraries],
    "cxx_std": 17,
    "extra_compile_args": [],
    "extra_link_args": [],
    "define_macros": [("LINKING_ASSEMBLER_DLL", 1), ("LINKING_BACKEND_DLL", 1),
                      ("LINKING_COMMON_DLL", 1), ("LINKING_COMPILER_DLL", 1), 
                      ("LINKING_ISE_DLL", 1), ("LINKING_COMPILER_DLL", 1),
                      ("LINKING_GENN_DLL", 1)]}

# If this is Windows
if WIN:
    # Turn off warnings about dll-interface being required for stuff to be
    # used by clients and prevent windows.h exporting TOO many awful macros
    fenn_extension_kwargs["extra_compile_args"].extend(["/wd4251", "/wd4275", "-DWIN32_LEAN_AND_MEAN", "-DNOMINMAX"])

    # Add include directory for FFI as it's built from source
    fenn_extension_kwargs["include_dirs"].append(os.path.join(genn_third_party_include, "libffi"))

    # Add FFI library with correct suffix
    # **TODO** just call this ffi
    fenn_extension_kwargs["libraries"].append("libffi" + lib_suffix)

    # Add GeNN and FFI libraries to dependencies
    fenn_extension_kwargs["depends"] = [os.path.join(pyfenn_path, "genn" + lib_suffix + ".dll"),
                                        os.path.join(pyfenn_path, "libffi" + lib_suffix + ".dll")]
# Otherwise
else:
    # Add GeNN library to dependencies
    fenn_extension_kwargs["depends"] = [os.path.join(pyfenn_path, "libgenn" + lib_suffix + ".so"),
                                        os.path.join(pyfenn_path, "docStrings.h")]

    # If this is Linux, we want to add extension directory i.e. $ORIGIN to runtime
    # directories so libGeNN and backends can be found wherever package is installed
    if LINUX:
        fenn_extension_kwargs["runtime_library_dirs"] = ["$ORIGIN"]
        fenn_extension_kwargs["libraries"].append("ffi")

ext_modules = [
    Pybind11Extension("_fenn",
                      [os.path.join(pyfenn_src, "fenn.cc")],
                      **fenn_extension_kwargs)]

# If we should build required FeNN libraries
if build_fenn_libs:
    # If compiler is MSVC
    if WIN:
        # **NOTE** ensure pygenn_path has trailing slash to make MSVC happy
        out_dir = os.path.join(pyfenn_path, "")
        check_call(["msbuild", "riscv_ise.sln", f"/t:backend",
                    f"/p:Configuration={lib_suffix[1:]}",
                    "/m", "/verbosity:quiet",
                    f"/p:OutDir={out_dir}"],
                    cwd=fenn_path)
    else:
        # Define make arguments
        make_arguments = ["make", "backend", "DYNAMIC=1",
                          f"LIBRARY_DIRECTORY={pyfenn_path}",
                          f"--jobs={cpu_count(logical=False)}"]
        if debug_build:
            make_arguments.append("DEBUG=1")

        if coverage_build:
            make_arguments.append("COVERAGE=1")

        # Build
        check_call(make_arguments, cwd=fenn_path)
# Read version from txt file
with open(os.path.join(fenn_path, "version.txt")) as version_file:
    version = version_file.read().strip()

setup(
    name="pyfenn",
    version=version,
    packages = find_packages(),
    package_data={"pyfenn": package_data},

    url="https://github.com/neworderofjamie/riscv_ise/",
    ext_package="pyfenn",
    ext_modules=ext_modules,
    zip_safe=False,
    python_requires=">=3.6",
    install_requires=["numpy>=1.17", "psutil",
                      "importlib-metadata>=1.0;python_version<'3.8'",
                      "setuptools"])
