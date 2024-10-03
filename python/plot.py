#!/usr/bin/env python
import fileinput
import json
import sys
from collections.abc import Iterable

import matplotlib.pyplot as plt

from python import geom


def parse(raw: dict[str, Iterable[float]]) -> geom.Point:
    if len(raw) != 1:
        msg = "`raw` must contain a single item"
        raise ValueError(msg)

    k, v = next(iter(raw.items()))

    return {
        "point": geom.Point,
    }[k](v)


if __name__ == "__main__":
    try:
        line = fileinput.input().readline()
    except FileNotFoundError:
        print(
            """\
Plot points encoded as JSON.

bazel run //python:plot -- [files...]

Plots the first line of all files listed in sys.argv[1:], defaulting
to sys.stdin if the list is empty. If a filename is '-',  it is also
replaced with sys.stdin.
        """,
            file=sys.stderr,
        )
        sys.exit(1)

    raw_values = json.loads(line)
    fig = geom.plot(map(parse, raw_values))
    plt.show()
