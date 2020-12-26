# the main module that run the KNN classification
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
def KNN_classification(trainMatrix, trainLabel, testMatrix, testLabel, model, K, ifTest):
    accuracy = 0
    if ifTest:
        fp = open(file="..\\..\\lab1_data\\classification_dataset\\18340085_LanJing_KNN_classification.csv", mode="w")
        fp.write("TextID,labels\n")
    for i in range(np.shape(testMatrix)[0]):
        heap = []
        emoDict = {}
        predictCount = 0
        predictLabel = ""
        # get K-smallest distance
        for j in range(np.shape(trainMatrix)[0]):
            heap.append(ds.dist(trainMatrix[j], testMatrix[i], model))
        upperLimit = hp.nsmallest(K, heap)[-1]
        lowerLimit = hp.nsmallest(K, heap)[0]
        # get the predicted labels
        for j in range(np.shape(trainMatrix)[0]):
            dis = ds.dist(trainMatrix[j], testMatrix[i], model)
            if dis <= upperLimit:
                if trainLabel[j] in emoDict:
                    emoDict[trainLabel[j]] += lowerLimit / dis
                else:
                    emoDict[trainLabel[j]] = lowerLimit / dis
                if emoDict[trainLabel[j]] > predictCount:
                    predictCount = emoDict[trainLabel[j]]
                    predictLabel = trainLabel[j]
        # give the prediction and count accuracy
        if ifTest:
            fp.write("{},{}\n".format(i+1, predictLabel))
        elif predictLabel == testLabel[i]:
            accuracy += 1
    if ifTest:
        fp.close()
        return 0
    print("Accepted Prediction: {}".format(accuracy), "\nAccuracy: {}".format(accuracy / np.shape(testMatrix)[0]))
    accuracy /= np.shape(testMatrix)[0]
    return accuracy

def KNN_train(trainFileName, validationFileName, K):
    print("KNN classification on K={}".format(K))
    # receive data collection
    trainMatrix, trainLabel, testMatrix, testLabel = TF.TFIDF(trainFileName, validationFileName, 0)
    maxAccuracy = 0
    bestModel = ""
    # learn the best model--method to measure proper distance
    for model in modeDict.items():
        print("Running distance model: {}".format(model[0]))
        accuracy = KNN_classification(trainMatrix, trainLabel, testMatrix, testLabel, model[1], K, 0)
        if accuracy > maxAccuracy:
            maxAccuracy = accuracy
            bestModel = model[0]
    return bestModel, maxAccuracy

def KNN_test(trainFileName, testFileName, model, K):
    # receive data collection
    trainMatrix, trainLabel, testMatrix, testLabel = TF.TFIDF(trainFileName, testFileName, 1)
    accuracy = KNN_classification(trainMatrix, trainLabel, testMatrix, testLabel, model, K, 1)
    return accuracy

# %%
