import cv2 as cv
import os
import numpy as np
import urllib.request

class ImageLoader:
    def __init__(self):
        pass

    def load_image(self, filename, **kwargs):
        """
        Loads an image from a local file with optional arguments for OpenCV's cv.imread.
        
        Parameters:
        - filename (str): Absolute path to the image file.
        - **kwargs: Additional keyword arguments for cv.imread (e.g., flags=cv.IMREAD_GRAYSCALE).
        
        Returns:
        - image: Loaded image as a NumPy array.
        """
        # Extract 'flags' from kwargs or use cv.IMREAD_COLOR as the default
        flag = kwargs.get('flags', cv.IMREAD_COLOR)
        
        path = os.path.join(filename)
        image = cv.imread(path, flag)
        if image is None:
            raise FileNotFoundError(f"Image {filename} not found!")
        return image

    def load_image_from_url(self, url, **kwargs):
        """
        Loads an image from an Internet URL with optional arguments for OpenCV's cv.imdecode.
        
        Parameters:
        - url (str): URL of the image.
        - **kwargs: Additional keyword arguments for cv.imdecode (e.g., flags=cv.IMREAD_GRAYSCALE).
        
        Returns:
        - image: Loaded image as a NumPy array.
        """
        try:
            # Extract 'flags' from kwargs or use cv.IMREAD_COLOR as the default
            flag = kwargs.get('flags', cv.IMREAD_COLOR)
            
            # Fetch the image data from the URL
            response = urllib.request.urlopen(url)
            image_data = np.asarray(bytearray(response.read()), dtype=np.uint8)
            
            # Decode the image
            image = cv.imdecode(image_data, flag)
            if image is None:
                raise ValueError(f"Failed to decode image from URL: {url}")
            return image
        except Exception as e:
            raise RuntimeError(f"Error loading image from URL {url}: {e}")