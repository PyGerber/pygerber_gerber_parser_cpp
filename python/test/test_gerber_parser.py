from __future__ import annotations

from typing import TYPE_CHECKING
from unittest.mock import MagicMock
import pytest

if TYPE_CHECKING:
    import pygerber_gerber_parser_cpp.gerber_parser as gerber_parser


@pytest.fixture()
def parser() -> gerber_parser.GerberParser:
    import pygerber_gerber_parser_cpp.gerber_parser as gerber_parser

    return gerber_parser.GerberParser()


def test_syntax_error(parser: gerber_parser.GerberParser) -> None:
    import pygerber_gerber_parser_cpp.gerber_parser as gerber_parser

    with pytest.raises(gerber_parser.SyntaxError):
        parser.parse("lol")


@pytest.mark.parametrize("g_code", [1, 2, 3, 4, 36, 37, 54, 55, 70, 71, 74, 75, 90, 91])
def test_parse_g_code(g_code, parser: gerber_parser.GerberParser) -> None:
    file = parser.parse(f"G{g_code}*G0{g_code}*G00{g_code}*G000{g_code}*")
    assert len(file.nodes) == 4

    for node in file.nodes:
        assert node.__class__.__qualname__ == f"G{g_code:0>2}"


@pytest.mark.parametrize("g_code", [1, 2, 3, 4, 36, 37, 54, 55, 70, 71, 74, 75, 90, 91])
def test_visit_g_code(g_code, parser: gerber_parser.GerberParser) -> None:
    file = parser.parse(f"G{g_code}*")
    assert len(file.nodes) == 1

    node = file.nodes[0]

    mock = MagicMock()
    node.visit(mock)
    getattr(mock, f"on_g{g_code:0>2}").assert_called_once_with(node)
