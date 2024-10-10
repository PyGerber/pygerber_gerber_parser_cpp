"""Build script for pygerber_gerber_parser_cpp package."""

from __future__ import annotations

import os
import platform
import subprocess
import sys
from pathlib import Path
import multiprocessing


THIS_DIR = Path(__file__).parent


class Builder:
    """Class responsible for building pygerber_gerber_parser_cpp binaries."""

    def __init__(self, build_type: str) -> None:
        """Initialize builder object."""
        self.repo_path = Path(__file__).parent
        self.build_type = build_type

    def build(self) -> None:
        """Build extension module."""
        build_directory = Path.cwd() / "build"
        os.environ["PATH"] += os.pathsep + os.path.dirname(sys.executable)
        self.cmake(
            "-S",
            ".",
            "-B",
            build_directory.as_posix(),
            f"-DCMAKE_BUILD_TYPE={self.build_type}",
            "-G",
            "Ninja",
        )
        self.cmake(
            "--build",
            build_directory.as_posix(),
            "--target",
            "PyGerberGerberParserCpp",
            "-j",
            str(multiprocessing.cpu_count()),
        )

    def cmake(self, *arg: str) -> None:
        """Run cmake command. If fails, raises CalledProcessError."""
        print("cmake", *arg)
        python_executable_directory = os.path.dirname(sys.executable)

        if platform.system() == "Windows":
            cmake_executable = python_executable_directory + os.path.sep + "cmake.exe"
        elif platform.system() == "Linux":
            cmake_executable = python_executable_directory + os.path.sep + "cmake"
        elif platform.system() == "Darwin":
            cmake_executable = python_executable_directory + os.path.sep + "cmake"
        else:
            msg = f"Unsupported platform: {platform.system()}"
            raise NotImplementedError(msg)

        subprocess.check_call(
            executable=cmake_executable,
            args=[
                cmake_executable,
                *arg,
            ],
            env=os.environ,
            cwd=self.repo_path.as_posix(),
        )


print(sys.argv)

if "--no-root" in sys.argv:
    print("Skipping building of root package.")
else:
    Builder(os.environ.get("PYGPP_BUILD", "DEBUG")).build()
