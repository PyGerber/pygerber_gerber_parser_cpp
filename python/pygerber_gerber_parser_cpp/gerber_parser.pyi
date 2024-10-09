from __future__ import annotations
from typing import Any

class Node:
    def visit(self, visitor: Any) -> None:
        pass

class File:
    nodes: list[Node]

class GerberParser:
    def parse(self, source: str) -> File:
        pass

class SyntaxError(Exception):
    pass
