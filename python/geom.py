from collections.abc import Iterable
from dataclasses import dataclass

import matplotlib.pyplot as plt
import numpy as np
from matplotlib.axes import Axes
from matplotlib.collections import PathCollection


@dataclass
class _Data:
    data: np.ndarray


class Point(_Data):
    @property
    def data(self) -> np.ndarray:
        return self._data

    @data.setter
    def data(self, value: Iterable[float]) -> None:
        self._data = np.array(value, dtype=np.float64)

    def add_to(self, ax: Axes) -> PathCollection:
        return ax.scatter(*self.data)


def plot(data: Iterable[Point]) -> plt.Figure:
    fig, ax = plt.subplots(subplot_kw={"projection": "3d"})
    ax.set_xlabel("x")
    ax.set_ylabel("y")
    ax.set_zlabel("z")

    for p in data:
        p.add_to(ax)

    return fig
