import numpy as np
import math as mt

# compute the PLA prediction
def PLA_predict(theta, x):
    f = x @ theta
    return f > 0

# update PLA parameters using a wrong prediction
def PLA_update(theta, x, y, learnRate):
    return theta + (2*y-1) * x.T * learnRate

def sigmoid(x):
    return 1 / (1 + np.exp(-x))

def size(x):
    return np.sqrt(x.T @ x)

# compute the logistic function
def logistic_predict(theta, x):
    f = x @ theta
    return sigmoid(f) > 0.5

# gradient of the current w
def logistic_gradient(theta, X, y):
    return (1/len(X)) * ((sigmoid(X @ theta) - y).T @ X).T

# gradient after introducing regulation
def logistic_gradient_reg(theta, X, y, Lambda):
    newTheta = theta.copy()
    newTheta[0][0] = 0
    return (1/len(X)) * (((sigmoid(X @ theta) - y).T @ X).T + Lambda * newTheta)

# update parameter w
def logistic_update(theta, gradient, learnRate):
    return theta - gradient * learnRate

# cost function
def logistic_cost(theta, X, y):
    posSum = 0
    negSum = 0
    for i in range(len(X)):
        if y[i] > 0:
            posSum += mt.log(sigmoid(X[i] @ theta))
        else:
            negSum += mt.log(1 - sigmoid(X[i] @ theta))
    return (-1/len(X))*(posSum + negSum)