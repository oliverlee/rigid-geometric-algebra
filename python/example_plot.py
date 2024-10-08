#!/usr/bin/env python

import matplotlib.pyplot as plt

from python import geom

if __name__ == "__main__":
    elements = [
        geom.Point([1, 0, 0, 0]),
        geom.Point([1, 1, 0, 0]),
        geom.Point([1, 0, 1, 0]),
        geom.Point([1, 0, 0, 1]),
        geom.Line([0, 1, 0, 0, 0, 1]),
        geom.Line([0, 1, 0, 0, 0, 0.5]),
        geom.Plane([1, 1, 1, -1]),
        geom.Plane([0, 1, 0, -0.5]),
    ]

    fig = geom.plot(elements)
    plt.show()
