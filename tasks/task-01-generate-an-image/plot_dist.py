import numpy as np
import matplotlib.pyplot as plt


def main():
    points = [150.4559, 134.05611, 129.41719,
              129.46083, 99.553482, 158.40138,
              122.17212, 125.3382, 124.5386,
              92.766968, 126.24654, 155.33759,
              150.50629, 120.82009, 152.41216]
    x = range(len(points))
    int_points = [round(p) for p in np.array(points)]
    bin_count = np.bincount(int_points)

    plt.plot(range(len(bin_count)), bin_count)
    plt.title(f"std={np.std(points):.2f}, mean={np.mean(points):.2f}")
    plt.show()


if __name__ == "__main__":
    main()
