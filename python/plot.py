#!/usr/bin/env python
import fileinput
import json
import sys
from collections.abc import Iterable
from dataclasses import dataclass

import numpy as np
import pyvista as pv


@dataclass
class _Point:
    data: np.ndarray


class Point(_Point):
    @property
    def data(self) -> np.ndarray:
        return self._data

    @data.setter
    def data(self, value: Iterable[float]) -> None:
        self._data = np.array(value, dtype=np.float64)


def parse(raw: dict[str, Iterable[float]]) -> Point:
    if len(raw) != 1:
        msg = "`raw` must contain a single item"
        raise ValueError(msg)

    k, v = next(iter(raw.items()))

    return {
        "point": Point,
    }[k](v)


def plot(data: Iterable[Point]) -> None:
    points = np.stack([d.data for d in data if isinstance(d, Point)])

    rgba = points - points.min(axis=0)
    rgba /= rgba.max(axis=0)
    pl = pv.Plotter(
        title="plot",
    )
    pl.add_axes_at_origin()
    pl.add_mesh(
        points,
        scalars=rgba,
        render_points_as_spheres=True,
        point_size=10,
        rgba=True,
    )
    pl.show()


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
    plot(map(parse, raw_values))
