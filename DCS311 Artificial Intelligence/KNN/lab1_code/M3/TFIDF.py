# module for data collecting and preprocessing;
#%%
import os
import numpy as np
import math

# turn global word frequency into IDF index
def computeIDF(wordFreqInSet, count):
    # idf_i = log(|D|/(1+|D_i|))
    for item in wordFreqInSet.items():
        wordFreqInSet[item[0]] = math.log2(count / (1+item[1]))
    return
# turn loacl word frequency into TF-IDF statistics
def computeTF(wordFreqInText, wordFreqInSet):
    for line in wordFreqInText:
        for word in line.keys():
            line[word] *= wordFreqInSet[word]
    return

# this function gives the intersection of words from the train and test set
def getCommonWords(wordDict, trainFile, testFile, ifTest):
    tmpDict = trainDict = testDict = set()
    with open(file="..\\..\\lab1_data\\regression_dataset\\{}".format(trainFile), mode="r") as fp:
        lineCount = -1
        for line in fp.readlines():
            # count lines and skip line #1
            lineCount += 1
            if lineCount == 0:
                continue
            # split into words
            words = line.strip('\n').split(',')[0].split(' ')
            trainDict.update(words)
            tmpDict.update(words)
    with open(file="..\\..\\lab1_data\\regression_dataset\\{}".format(testFile), mode="r") as fp:
        lineCount = -1
        for line in fp.readlines():
            # count lines and skip line #1
            lineCount += 1
            if lineCount == 0:
                continue
            # split into words
            if ifTest:
                words = line.strip('\n').split(',')[1].split(' ')
            else:
                words = line.strip('\n').split(',')[0].split(' ')
            testDict.update(words)
            tmpDict.update(words)
    for word in tmpDict:
        if word in trainDict and word in testDict:
            wordDict.add(word)

# compute TF-IDF data a data file
# test files use a different format
def TFIDF_file(wordDict, fileName, ifTest):
    wordFreqInSet = {}
    wordFreqInText = []
    labels = []

    with open(file="..\\..\\lab1_data\\regression_dataset\\{}".format(fileName), mode="r") as fp:
        lineCount = -1
        for line in fp.readlines():
            lineCount += 1
            if lineCount == 0:
                continue
            line  = line.strip('\n').split(',')
            if ifTest:
                words = line[1].split(' ')
                # no reference labels for the test set
            else:
                words = line[0].split(' ')
                # get labels, only for training and validation sets
                labels.append([])
                for label in range(1, 7):
                    labels[-1].append(float(line[label]))

            validWordCount = 0
            wordFreqInText.append({})
            for word in words:
                # ignode words that two files don't have in common
                if word not in wordDict:
                    continue
                validWordCount += 1
                if word in wordFreqInText[-1]:
                    wordFreqInText[-1][word] += 1
                else:
                    wordFreqInText[-1][word] = 1
                    if word in wordFreqInSet:
                        wordFreqInSet[word] += 1
                    else:
                        wordFreqInSet[word] = 1          
            # compute valid frequency
            for word in words:
                if word not in wordDict:
                    continue
                wordFreqInText[-1][word] /= validWordCount
    # compute TF-IDF data
    computeIDF(wordFreqInSet, lineCount)
    computeTF(wordFreqInText, wordFreqInSet)
    # sort the word list
    sortedWord = list(wordDict)
    sortedWord.sort()
    # construct word matrix
    wordMat = np.ndarray((lineCount, len(sortedWord)))
    for i in range(lineCount):
        for j in range(len(sortedWord)):
            word = sortedWord[j]
            if word in wordFreqInText[i]:
                wordMat[i][j] = round(wordFreqInText[i][word], 3)
            else:
                wordMat[i][j] = 0
    return wordMat, labels

def TFIDF(trainFile, testFile, ifTest):
    # get intersection of the two files
    wordDict = set()
    getCommonWords(wordDict, trainFile, testFile, ifTest)
    
    trainMatrix, trainLabel = TFIDF_file(wordDict, trainFile, 0)
    testMatrix, testLabel = TFIDF_file(wordDict, testFile, ifTest)

    return trainMatrix, trainLabel, testMatrix, testLabel
    
        
# %%
