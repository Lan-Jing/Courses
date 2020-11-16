# module for distance computation;
import numpy as np

def dist(arraya, arrayb, mode):
    if mode == 0:
        dis = np.sum(np.abs(np.subtract(arraya, arrayb)))
    elif mode == 1:
        dis = np.sqrt(np.sum(np.power(np.subtract(arraya, arrayb), 2)))
    else:
        dis = 1 - np.dot(arraya, arrayb) / np.sqrt(np.sum(np.power(arraya, 2)) * np.sum(np.power(arrayb, 2)))
    return dis

