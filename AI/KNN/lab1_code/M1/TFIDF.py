#%%
import numpy as np
import prettytable as pt
import os
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

# create the dictionary for word frequency
wordFreqInSet = {}
wordFreqInText = []
textIndex = []
# read from data file
with open(file="..\\..\\lab1_data\\semeval.txt", mode="r") as fp:
    lineCount = 0
    for line in fp.readlines():
        # count lines
        lineCount += 1
        # split into words
        line = line.strip('\n').split('\t')
        words = line[-1].split(' ')
        textIndex.append(line[0])

        wordFreqInText.append({})
        for word in words:
            # insert text frequency
            if word in wordFreqInText[-1]:
                wordFreqInText[-1][word] += 1 / len(words)
            else:
                wordFreqInText[-1][word] = 1 / len(words)
                # insert global frequency
                if word in wordFreqInSet:
                    wordFreqInSet[word] += 1
                else:
                    wordFreqInSet[word] = 1
# compute TF-IDF values
computeIDF(wordFreqInSet, lineCount)
computeTF(wordFreqInText, wordFreqInSet)
# sort the word list
sortedWord = list(wordFreqInSet.keys())
sortedWord.sort()
# create the output table
tableHead = ["", ]
for word in sortedWord:
    tableHead.append(word)
table = pt.PrettyTable(tableHead)
# add rows into the table
for i in range(lineCount):
    row = ["Text {}".format(textIndex[i]), ]
    # compute word frequency in a single text
    for word in sortedWord:
        if word in wordFreqInText[i]:
            row.append(round(wordFreqInText[i][word], 2))
        else:
            row.append(0)
    # insert row into the table
    table.add_row(row)
# output the table
tableString = table.get_string()
with open(file="TF-IDF.txt", mode="w") as fp:
    fp.write(tableString)
        

# %%
