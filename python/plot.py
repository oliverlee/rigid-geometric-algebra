#!/usr/bin/env python
import fileinput
import json
import numpy as np
import pyvista as pv
from dataclasses import dataclass

@dataclass
class _Point:
    data: np.array

class Point(_Point):
    @property
    def data(self) -> np.array:
        return self._data

    @data.setter
    def data(self, value):
        self._data = np.array(value, dtype=np.float64)

def parse(raw):
    assert len(raw) == 1
    for k, v in raw.items():
        return {
            "point": Point,
        }[k](v)

def plot(data):
    points = np.stack([
        d.data for d in data
        if isinstance(d, Point)
    ])

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
    line = fileinput.input().readline()
    raw_values = json.loads(line)
    plot(map(parse, raw_values)))
