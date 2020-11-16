#%%
import read_and_split as rs
import pla
import logistic

# K-fold cross validation
K = 5
# maximum iterations
maxIter = 5000
# learning rate of PLA
learnRate_PLA = 0.2
# learning rate of gradient descend
learnRate_reg = 0.1
# minimum gradient of the iteration
minGradient = 1e-5
# regulation parameter Lambda
Lambda = 0

# PLA training and validation

matrix, labels = rs.readin("train.csv")
for i in range(K):
    trainMatrix, trainLabels, validMatrix, validLabels = rs.cross_validation_split(matrix, labels, i/K*len(matrix), (i+1)/K*len(matrix))
    w = pla.train(trainMatrix, trainLabels, maxIter, learnRate_PLA)
    accuracy, prediction = pla.test(w, validMatrix, validLabels, 0)
    print("Accuracy of PLA: {}".format(accuracy))

# Logistic training and validation
for i in range(K):
    trainMatrix, trainLabels, validMatrix, validLabels = rs.cross_validation_split(matrix, labels, i/K*len(matrix), (i+1)/K*len(matrix))
    w = logistic.train(trainMatrix, trainLabels, maxIter, learnRate_reg, minGradient, Lambda)
    accuracy, prediction = logistic.test(w, validMatrix, validLabels, 0)
    print("Accuracy of Logistic: {}".format(accuracy))
# %%
