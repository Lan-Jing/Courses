import os
#%%

# read in data, return the matrix of feature and the label vector
# remember transform string type into float
def readin(fileName):
    matrix = []
    labels = []
    with open(file="..\\lab3_data\\{}".format(fileName), mode="r") as fp:
        for line in fp.readlines():
            words = line.strip('\n').split(',')
            for j in range(len(words)):
                words[j] = float(words[j])
            labels.append(words[-1])
            matrix.append(words[0:-1:1])
            matrix[-1].append(1)
    return matrix, labels

# split data into train set and validation set
def cross_validation_split(matrix, labels, validStart, validEnd):
    trainMatrix = []
    trainLabels = []
    validMatrix = []
    validLabels = []
    for i in range(len(matrix)):
        if validStart <= i and i < validEnd:
            validMatrix.append(matrix[i])
            validLabels.append(labels[i])
        else:
            trainMatrix.append(matrix[i])
            trainLabels.append(labels[i])
    return trainMatrix, trainLabels, validMatrix, validLabels
# %%
