import string
import numpy as np
import lxml.etree as le

import torch
import torch.utils.data as Data
import torch.nn.utils.rnn as rnn

K = 5

# load word vectors from file
def load_wordVec():
    wordVec = {}
    with open('../data/glove.6B.100d.txt', 'r') as fp:
        for line in fp.readlines():
            line = line.split(' ')
            # convert the rest to float
            wordVec[line[0]] = [float(line[i]) for i in range(1, len(line))]
    return wordVec

# normalize the given string, convert it into word list
def string_to_list(s):
    # seperate each punctuation
    tmps = ''
    for i in range(len(s)):
        if s[i] in string.punctuation:
            tmps += ' ' + s[i] + ' '
        else:
            tmps += s[i]
    s = ' '
    for i in range(len(tmps)):
        if s[-1] != ' ' or tmps[i] != ' ':
            s += tmps[i]
    return s.strip().split(' ')

# use normalized word to construct a word vector
def list_to_wordVec(list, wordVec):
    v_s = []
    for word in list:
        if word in wordVec:
            v_s.append(wordVec[word])
        else:
        # word not found in the dictionary
            return 0, v_s
    return 1, v_s

# construct wordVec, load data from file
def data_load():
    wordVec = load_wordVec()

    data = []
    labels = []
    with open('../data/Laptop_Train_v2.xml', 'rb') as fp:
        xmlRoot = le.fromstring(fp.read())
        # convert sentence into word vectors
        for sentence in xmlRoot:
            text_norm = string_to_list(sentence.find('text').text.lower())
            ok, text_v = list_to_wordVec(text_norm, wordVec)
            if ok == 0:
                continue
            
            wordCnt = len(text_v)
            label_v = [0] * wordCnt
            ok = 1
            keywords = sentence.find('aspectTerms')
            if keywords != None:
                for keyword in keywords:
                    # get normalized list from the sentence
                    keyword_norm = string_to_list(keyword.attrib['term'].lower())
                    ok, keyword_v = list_to_wordVec(keyword_norm, wordVec)
                    if ok == 0:
                        break
                    # label keywords in the sentence
                    for i in range(wordCnt):
                        if text_norm[i] == keyword_norm[0]:
                            for j in range(len(keyword_v)):
                                label_v[i+j] = 1
            if ok == 1:
                data.append(text_v)
                labels.append(label_v)
    return data, labels                    

# split the dataset and construct loader for both training and validation
def data_split(dataset, batch_size=64):
    validSize = int(dataset.len/K)
    trainSize = dataset.len - validSize
    trainData, validData = torch.utils.data.random_split(dataset, [trainSize, validSize])
    trainLoader = torch.utils.data.DataLoader(dataset=trainData, batch_size=batch_size, collate_fn=data_transform)
    validLoader = torch.utils.data.DataLoader(dataset=validData, batch_size=batch_size, collate_fn=data_transform)
    return trainLoader, validLoader

# user-defined loading function, padding the dataset
def data_transform(dataset):
    dataset.sort(key=lambda x: len(x[0]), reverse=True)

    data = [x[0] for x in dataset]
    labels = [x[1] for x in dataset]
    data_len = [len(x) for x in data]

    data = rnn.pad_sequence(data, batch_first=True, padding_value=0)
    labels = rnn.pad_sequence(labels, batch_first=True, padding_value=0)

    return data, labels, data_len

# define dataset for semEval subtask
class semEvelDataset(Data.Dataset):
    def __init__(self):
        self.data, self.labels = data_load()
        self.len = len(self.data)

    def __getitem__(self, index):
        return torch.tensor(self.data[index]), torch.tensor(self.labels[index])
    
    def __len__(self):
        return self.len