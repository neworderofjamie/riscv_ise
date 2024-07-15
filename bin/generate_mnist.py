import numpy as np
import mnist

mnist.datasets_url = "https://storage.googleapis.com/cvdf-datasets/mnist/"
test_labels = mnist.test_labels()
test_images = mnist.test_images()

test_times = 20.0 * np.log(test_images / (test_images - 51))
test_bad_pixels = (test_images <= 51)
test_times[test_bad_pixels]=32767

test_times_int = np.round(test_times).astype(np.int16)
test_times_int.tofile("mnist_times.bin")

test_labels_int = np.round(test_labels).astype(np.int16)
test_labels_int.tofile("mnist_labels.bin")