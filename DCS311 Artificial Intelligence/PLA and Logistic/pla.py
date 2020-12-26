import numpy as np
import helpers as hp

# train the pla machine
def train(matrix, labels, maxIter, learnRate):
    nextTarget = 0
    w = np.zeros((len(matrix[0]), 1), dtype=float)
    npMatrix = np.matrix(matrix, dtype=float)

    for iter in range(maxIter):
        nextTarget = -1
        for i in range(len(matrix)):
            predict = hp.PLA_predict(w, npMatrix[i])
            if predict != labels[i]:
                nextTarget = i
                break
        if nextTarget == -1:
            break
        else:
            w = hp.PLA_update(w, npMatrix[nextTarget], labels[nextTarget], learnRate)
    return w

def test(w, matrix, labels, ifTest):
    accuracy = 0
    prediction = []
    npMatrix = np.matrix(matrix, dtype=float)

    for i in range(len(matrix)):
        predict = hp.PLA_predict(w, npMatrix[i])
        prediction.append(predict)
        if ifTest == 0 and predict == labels[i]:
            accuracy += 1
    accuracy /= len(matrix)
    return accuracy, prediction