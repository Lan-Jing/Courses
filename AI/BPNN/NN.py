import numpy as np
import helper as hp
import data_proc as dp

# deep copy from_nn to to_nn
def copy(to_nn, from_nn):
    to_nn.act = from_nn.act
    to_nn.act_grad = from_nn.act_grad
    to_nn.W1 = from_nn.W1.copy()
    to_nn.W2 = from_nn.W2.copy()
    return 

class BPNN():
    def __init__(self, iSize = 1, hSize = 1, model = "sigmoid"):
        self.act = hp.activation_list[model][0]
        self.act_grad = hp.activation_list[model][1]
        self.W1 = np.random.rand(hSize, iSize+1)
        self.W2 = np.random.rand(1, hSize+1)

    def forward(self, X):
        m = len(X)
        X = np.array(X, dtype=float)

        # stack bias element on X before each forwarding.
        A1 = np.vstack((X.T, np.ones((1, m), dtype=float)))
        Z2 = self.W1 @ A1
        A2 = self.act(Z2)
        # to output layer, no activation for output
        A2 = np.vstack((A2, np.ones((1, m), dtype=float)))
        Z3 = self.W2 @ A2
        A3 = Z3
        return A1, Z2, A2, Z3, A3
    
    # return prediction and loss
    def predict(self, X, y):
        A1, Z2, A2, Z3, A3 = self.forward(X)
        loss = hp.loss(y, A3.T)
        return A3, loss

    # first forward the input, then update the weight matrices.
    def backpropagation(self, X, y, learnRate):
        m = len(X)

        A1, Z2, A2, Z3, A3 = self.forward(X)
        delta3 = A3 - y.T
        delta2 = (self.W2.T) @ delta3 * self.act_grad(np.vstack((Z2, np.zeros((1, m), dtype=float))))
        W2_grad = delta3 @ (A2.T) / m
        W1_grad = delta2[:-1,:] @ A1.T / m

        self.W1 -= W1_grad * learnRate
        self.W2 -= W2_grad * learnRate
        return 

def train(model, trainX, trainy, validX, validy, maxIter, learnRate, batchSize, ifTest):
    trainy = np.reshape(trainy, (len(trainy), 1))
    validy = np.reshape(validy, (len(validy), 1))

    min_loss = 2**32-1
    loss = [[], []]
    bestPredict = []

    bestNN = BPNN()
    nowNN = BPNN(len(trainX[0]), 100, model)
    for i in range(maxIter+1):
        # split into batches
        X_list, y_list = dp.batch_split(trainX, trainy, batchSize)
        for j in range(len(X_list)):
            nowNN.backpropagation(X_list[j], y_list[j], learnRate)

        loss[0].append(nowNN.predict(trainX, trainy)[-1])
        # output loss function
        if ifTest == 0:
            loss[1].append(nowNN.predict(validX, validy)[-1])
            if i % (maxIter/10) == 0:
                print("train loss:", loss[0][-1], " validation loss:", loss[1][-1])
        elif i % (maxIter/10) == 0:
                print("trian loss:", loss[0][-1])
        # update the best, use train set when there's no validation result to reference.
        if loss[1-ifTest][-1] < min_loss:
            min_loss = loss[1-ifTest][-1]
            copy(bestNN, nowNN)
            bestPredict = nowNN.predict(validX, validy)[0]

    # print(trainy, '\n', bestNN.predict(trainX, trainy)[0])
    return bestNN, bestPredict, loss