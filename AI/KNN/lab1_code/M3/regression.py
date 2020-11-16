#%%
import numpy as np
import heapq as hp
import TFIDF as TF
import dist as ds
import math

modeDict = {
    "Manhattan" : 0,
    "Euclidean" : 1,
    "Similarity" : 2
}
# define the model parameter--distance mode
distMode = ""

# model specifies how distance is calculated
# K is the hyper-parameter that control the range of elements from which the decisions are made
# when there's no answer to compare, ifTest == 1, only prediction results would be presented
def KNN_regression(trainMatrix, trainLabel, testMatrix, testLabel, model, K, ifTest):
    emoCount = len(trainLabel[1])
    corr = np.ndarray((emoCount, 1))
    predictProb = []
    if ifTest:
        fp = open(file="..\\..\\lab1_data\\regression_dataset\\18340085_LanJing_KNN_regression.csv", mode="w")
        fp.write("TextID,anger,disgust,fear,joy,sad,surprise\n")
    for i in range(np.shape(testMatrix)[0]):
        heap = []
        predictProb.append([])
        for j in range(emoCount):
            predictProb[-1].append(0)
        # get K-smallest distance
        for j in range(np.shape(trainMatrix)[0]):
            heap.append(ds.dist(trainMatrix[j], testMatrix[i], model))
        upperLimit = hp.nsmallest(K, heap)[-1]
        # to get probability standardized
        dividend = 0
        # get the predicted probability
        for j in range(np.shape(trainMatrix)[0]):
            dis = ds.dist(trainMatrix[j], testMatrix[i], model)
            if dis <= upperLimit:
                dividend += 1 / dis
                for emo in range(emoCount):
                    predictProb[-1][emo] += trainLabel[j][emo] / dis
        if ifTest:
            fp.write("{},".format(i+1))
        for j in range(emoCount):
            predictProb[-1][j] /= dividend
            # print out prediction result
            if ifTest:
                fp.write("{}".format(round(predictProb[-1][j], 4)))
                if j == emoCount - 1:
                    fp.write("\n")
                else:
                    fp.write(",")
    # only output prediction for test
    if ifTest:
        fp.close()
        return 0
    # compute correlation coefficient for validation    
    for i in range(emoCount):
        corr[i] = ds.corr(np.array(predictProb)[:,i], np.array(testLabel)[:,i], model == 2)
    print("Correlation coefficient: {}".format(np.mean(corr)))
    return np.mean(corr)

def KNN_train(trainFileName, validationFileName, K):
    print("KNN regression on K={}".format(K))
    # receive data collection
    trainMatrix, trainLabel, testMatrix, testLabel = TF.TFIDF(trainFileName, validationFileName, 0)
    maxCorr = 0
    bestModel = ""
    # learn the best model
    for model in modeDict.items():
        print("Running distance model: {}".format(model[0]))
        corr = KNN_regression(trainMatrix, trainLabel, testMatrix, testLabel, model[1], K, 0)
        if corr > maxCorr:
            maxCorr = corr
            bestModel = model[0]
    return bestModel, corr

def KNN_test(trainFileName, testFileName, model, K):
    # receive data collection
    trainMatrix, trainLabel, testMatrix, testLabel = TF.TFIDF(trainFileName, testFileName, 1)
    # output the prediction into files
    corr = KNN_regression(trainMatrix, trainLabel, testMatrix, testLabel, model, K, 1)
    return corr
                