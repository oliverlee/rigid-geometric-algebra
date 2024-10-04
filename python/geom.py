from collections.abc import Iterable
from dataclasses import dataclass

import matplotlib.pyplot as plt
import numpy as np
from matplotlib.axes import Axes
from matplotlib.collections import PathCollection, PolyCollection
from mpl_toolkits.mplot3d.art3d import Line3DCollection, Poly3DCollection


class GeomError(ArithmeticError):
    pass


def require(cond: bool, message: str) -> None:  # noqa: FBT001
    # https://github.com/astral-sh/ruff/issues/9497
    if not cond:
        raise GeomError(message)


@dataclass
class _Data:
    data: np.ndarray

    def __post_init__(self) -> None:
        self._invariant()

    def _invariant(self) -> None:
        pass

    @property
    def data(self) -> np.ndarray:
        return self._data

    @data.setter
    def data(self, value: Iterable[float]) -> None:
        self._data = np.array(value, dtype=np.float64)
        self._invariant()

    @property
    def view(self) -> np.ndarray:
        v = self.data.view()
        v.setflags(write=False)
        return v


class Point(_Data):
    def add_to(self, ax: Axes) -> PathCollection:
        return ax.scatter(*self.view)


class Line(_Data):
    def _invariant(self) -> None:
        require(
            self.view.size % 2 == 0, "`Line` must contain an even number of elements"
        )
        require(
            np.dot(self.direction, self.moment) == 0,
            "The `direction` and `moment` of a `Line` must be orthogonal",
        )

    @property
    def direction(self) -> np.ndarray:
        return self.view[: (self.view.size // 2)]

    @property
    def moment(self) -> np.ndarray:
        return self.view[(self.view.size // 2) :]

    def add_to(self, ax: Axes) -> PathCollection:
        norm = np.linalg.norm

        # project origin onto line
        u = np.cross(self.direction, self.moment)
        p = norm(self.moment) / norm(self.direction) * u / norm(u)

        # these lines should extend to infinity
        lines = Line3DCollection(
            [
                np.vstack(
                    [
                        p - 1000 * self.direction,
                        p - 10 * self.direction,
                        p - self.direction,
                        p,
                        p + self.direction,
                        p + 10 * self.direction,
                        p + 1000 * self.direction,
                    ]
                )
            ]
        )
        return ax.add_collection(lines)


class Plane(_Data):
    @property
    def normal(self) -> np.array:
        return self.view[:-1]

    @property
    def position(self) -> np.array:
        return self.view[-1:]

    def add_to(self, ax: Axes) -> PolyCollection:
        norm = np.linalg.norm

        # obtain vectors orthogonal to self.normal
        u = np.cross(self.normal, [1, 0, 0])
        if not u.any():
            _, u, v = np.unstack(np.eye(3))
        else:
            v = np.cross(self.normal, u)
            u /= norm(u)
            v /= norm(v)

        # project origin onto plane
        d = self.position / norm(self.normal)
        p = -self.normal / norm(self.normal) * d

        # these vertices should extend to infinity but that makes it hard to see
        scaling = 1
        planes = Poly3DCollection(
            [
                np.vstack(
                    [
                        p - scaling * u,
                        p - scaling * v,
                        p + scaling * u,
                        p + scaling * v,
                    ]
                )
            ],
            alpha=0.3,
        )
        return ax.add_collection(planes)


def plot(data: Iterable[_Data]) -> plt.Figure:
    fig, ax = plt.subplots(subplot_kw={"projection": "3d"})
    ax.set_xlabel("x")
    ax.set_ylabel("y")
    ax.set_zlabel("z")

    for p in data:
        p.add_to(ax)

    return fig
