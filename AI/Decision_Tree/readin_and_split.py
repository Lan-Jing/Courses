# this module load data from the data set and split it into training set and validation set;
# use "fold" to control size of these data sets;
#%%
import decision_tree as dt
import numpy as np
import os

# read data from the data set, split it into a matrix
def load_data(fileName):
    dataMat = []
    labels = []
    attri = {}
    with open(file="..\\lab2_dataset\\{}".format(fileName), mode="r") as fp:
        firstLine = 0
        for line in fp.readlines():
            words = line.strip('\n').split(',')
            if firstLine == 0:
                # create a dictionary for attributes
                for i in range(len(words)-1):
                    attri[words[i]] = i
                firstLine = 1
                continue
            # change labels into numbers
            if words[-1] == '0':
                labels.append(0)
            else:
                labels.append(1)
            # update the dictionary and append attributes
            dataMat.append([])
            for i in range(len(words)-1):
                dataMat[-1].append(words[i])
    return dataMat, labels, attri

# split the matrix into num_part parts, then take one of them as validation set;
def split_data(dataMat, labels, validStart, validEnd):
    trainMat = []
    trainLabel = []
    validMat = []
    validLabel = []

    for i in range(len(dataMat)):
        if validStart <= i and i < validEnd:
            validMat.append(dataMat[i])
            validLabel.append(labels[i])
        else:
            trainMat.append(dataMat[i])
            trainLabel.append(labels[i])
    return trainMat, trainLabel, validMat, validLabel
'''
# test for input module
dataMat, labels, attri = load_data("car_train.csv")
trainMat, trainLabel, validMat, validLabel = split_data(dataMat, labels, 0, len(dataMat)/5)

for attribute in attri.attributes.keys():
    print(attribute, attri.attributes[attribute])
    print(attri.valueDict[attri.attributes[attribute]])
'''
# %%
