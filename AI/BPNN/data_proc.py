import numpy as np

def create_one_hot(x, x_range):
    one_hot = np.zeros((1, x_range), dtype=int)
    one_hot[0][x-1] = 1
    return one_hot[0].tolist()
    
# read in data from the data file, turn it into X and y
def read_in(fileName, ifTest):
    X = []
    y = []
    skip = 1
    with open(file="..\\lab4_dataset\\{}".format(fileName), mode="r") as fp:
        # skip label descriptions
        for line in fp.readlines():
            if skip == 1:
                skip = 0
                continue
            
            words = line.strip('\n').split(',')
            for j in range(len(words)):
                if j == 0:
                    continue
                elif j == 1:
                    # extract day of the date only
                    words[j] = float(words[j].split('/')[-1])
                else:
                    # deal with test sample.
                    if j == len(words)-1 and ifTest:
                        words[j] = -1
                    else:
                        words[j] = float(words[j])
            y.append(words[-1])
            X.append(words[1:-1:1])
    return X, y

# adapt one-hot preprocessing when readin data
def preprocessing_read_in(fileName, ifTest):
    X = []
    y = []
    skip = 1
    with open(file="..\\lab4_dataset\\{}".format(fileName), mode="r") as fp:
        for line in fp.readlines():
            if skip == 1:
                skip = 0
                continue

            words = line.strip('\n').split(',')
            x = []
            # season
            x.extend(create_one_hot(int(words[2]), 4))
            # month
            x.extend(create_one_hot(int(words[4]), 12))
            # hour
            x.extend(create_one_hot(int(words[5])+1, 24))
            # if holiday
            x.extend(create_one_hot(int(words[6])+1, 2))
            # weekday index
            x.extend(create_one_hot(int(words[7])+1, 7))
            # if workingday
            x.extend(create_one_hot(int(words[8])+1, 2))
            # the rest of the float data
            for i in range(9, len(words)-1):
                x.append(float(words[i]))

            X.append(x)
            if ifTest:
                y.append(-1)
            else:
                y.append(float(words[-1]))
    return X, y

# split data into train set and validation set
def valid_split(X, y, begin, end):
    trainX = []
    trainy = []
    validX = []
    validy = []
    for i in range(len(X)):
        if begin <= i and i < end:
            validX.append(X[i])
            validy.append(y[i])
        else:
            trainX.append(X[i])
            trainy.append(y[i])
    return trainX, trainy, validX, validy

def batch_split(X, y, batchSize):
    X_list = []
    y_list = []
    if batchSize <= 0:
        X_list.append(X)
        y_list.append(y)
    else:
        for begin in range(0, len(X), 2**batchSize):
            end = min(len(X), begin + 2**batchSize)
            X_list.append(X[begin:end])
            y_list.append(y[begin:end])
    return X_list, y_list