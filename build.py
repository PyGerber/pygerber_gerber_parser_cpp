"""Build script for epseon_backend package."""

from __future__ import annotations

import os
import subprocess
import sys
from pathlib import Path
import multiprocessing


THIS_DIR = Path(__file__).parent


class Builder:
    """Class responsible for building epseon_backend binaries."""

    def __init__(self) -> None:
        """Initialize builder object."""
        self.repo_path = Path(__file__).parent

    def build(self) -> None:
        """Build extension module."""
        build_directory = Path.cwd() / "build"
        os.environ["PATH"] += os.pathsep + os.path.dirname(sys.executable)
        self.cmake(
            "-S",
            ".",
            "-B",
            build_directory.as_posix(),
            "-DCMAKE_BUILD_TYPE=Release",
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
        subprocess.check_call(
            executable=sys.executable,
            args=[
                sys.executable,
                "-c",
                "import cmake;cmake.cmake()",
                *arg,
            ],
            env=os.environ,
            cwd=self.repo_path.as_posix(),
        )


Builder().build()
