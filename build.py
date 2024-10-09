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

    DEPS: tuple[tuple[str, str], ...] = (
        ("googletest", "release-1.12.1"),
        ("spdlog", "v1.12.0"),
        ("pybind11", "v2.11.1"),
        ("fmt", "10.1.1"),
        ("vma_hpp", "v3.0.1-3"),
    )

    def __init__(self) -> None:
        """Initialize builder object."""
        self.repo_path = Path(__file__).parent

    def build(self) -> None:
        """Build extension module."""
        self.cmake("-S", ".", "-B", "build", "-DCMAKE_BUILD_TYPE=Release")
        self.cmake(
            "--build",
            "build",
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
