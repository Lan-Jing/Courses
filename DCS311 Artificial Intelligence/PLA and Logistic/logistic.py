import numpy as np
import helpers as hp

# train the logistic function
def train(matrix, labels, maxIter, learnRate, minGradient, Lambda):
    w = np.zeros((len(matrix[0]), 1), dtype=float)
    npMatrix = np.matrix(matrix, dtype=float)
    npLabels = np.array(labels, dtype=float, ndmin=2)
    npLabels = np.reshape(npLabels, (len(matrix), 1))

    for iter in range(maxIter):
        gradient = hp.logistic_gradient_reg(w, npMatrix, npLabels, Lambda)
        if hp.size(gradient) < minGradient:
            break
        w = hp.logistic_update(w, gradient, learnRate)
    return w

# test the logistic model
def test(w, matrix, labels, ifTest):
    accuracy = 0
    prediction = []
    npMatrix = np.matrix(matrix, dtype=float)

    for i in range(len(matrix)):
        predict = hp.logistic_predict(w, npMatrix[i])
        prediction.append(predict)
        if ifTest == 0 and predict == labels[i]:
            accuracy += 1
    accuracy /= len(matrix)
    return accuracy, prediction