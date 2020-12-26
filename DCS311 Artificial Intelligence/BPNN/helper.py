import numpy as np
import math as mt

def sigmoid(x):
    return 1 / (1 + np.exp(-x))

def sigmoid_grad(x):
    return sigmoid(x) * (1 - sigmoid(x))

def tanh(x):
    return (np.exp(x) - np.exp(-x)) / (np.exp(x) + np.exp(-x))

def tanh_grad(x):
    return 1 - tanh(x)**2

def relu(x):
    return np.maximum(x, 0)

def relu_grad(x):
    return x > 0

def loss(y, y_reg):
    return np.sum(((y - y_reg)**2) / (2 * len(y)))

def size(x):
    return np.sqrt(x.T @ x)

# dictionary of the available activation functions.
activation_list = {
    "sigmoid" : (sigmoid, sigmoid_grad),
    "tanh" : (tanh, tanh_grad),
    "relu" : (relu, relu_grad)
}