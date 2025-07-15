import numpy as np
import pandas as pd
import cv2 as cv
from sklearn.decomposition import PCA
from scipy.stats import pearsonr, spearmanr

class StatisticalTools:
    def __init__(self):
        pass

    def get_image_info(self, image):
        """
        Extracts metadata and statistical information from an image.

        Parameters:
        - image (numpy.ndarray): Input image.

        Returns:
        - dict: Dictionary containing image metadata and statistics.
        """
        if not isinstance(image, np.ndarray):
            raise ValueError("Input must be a NumPy array representing an image.")

        # Extract image properties
        height, width = image.shape[:2]
        depth = 1 if len(image.shape) == 2 else image.shape[2]  # Number of channels
        dtype = image.dtype

        # Compute statistical properties
        min_val = np.min(image)
        max_val = np.max(image)
        mean_val = np.mean(image)
        std_val = np.std(image)
        nbytes  = image.nbytes

        return {
            "width": width,
            "height": height,
            "dtype": dtype,
            "depth": depth,
            "nbytes": nbytes,
            "min_value": min_val,
            "max_value": max_val,
            "mean": mean_val,
            "std_dev": std_val
        }

    def print_image_info(self, image):
        """
        Prints metadata and statistical information about a given image.

        Parameters:
        - image (numpy.ndarray): The input image as a NumPy array.

        Functionality:
        1. Calls `self.get_image_info(image)`, which returns a dictionary containing:
        - height: Image height in pixels.
        - width: Image width in pixels.
        - dtype: Data type of the image (e.g., uint8, float32).
        - depth: Number of color channels (1 for grayscale, 3 for RGB, etc.).
        - min_value: Minimum pixel intensity in the image.
        - max_value: Maximum pixel intensity in the image.
        - mean: Mean pixel intensity.
        - std_dev: Standard deviation of pixel intensities.

        2. Iterates through the dictionary and prints each property in the format:
        `key: value`

        Example Output (for a 100x100 grayscale image):
        ```
        height: 100
        width: 100
        dtype: uint8
        depth: 1
        min_value: 12
        max_value: 255
        mean: 127.56
        std_dev: 32.78
        ```
        """
        info = self.get_image_info(image)
        # Print results
        for key, value in info.items():
            print(f"{key}: {value}")