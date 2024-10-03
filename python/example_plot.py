#!/usr/bin/env python

import matplotlib.pyplot as plt

from python import geom

if __name__ == "__main__":
    elements = [
        geom.Point([0, 0, 0]),
        geom.Point([1, 0, 0]),
        geom.Point([0, 1, 0]),
        geom.Point([0, 0, 1]),
    ]

    fig = geom.plot(elements)
    plt.show()
