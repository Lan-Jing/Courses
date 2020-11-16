#%%
import data_proc as dp
import helper as hp
import NN as nn
import plot as plt

# K-fold cross validation
K = 5
# activation function of the NN
model = "tanh"
# maximum number of iterations
maxIter = 200
# learning rate 
learnRate = 1e-3
# batch size, <= 0 to prevent this feature
batchSize = 5

def train_valid(fileName, mode):
    if mode:
        X, y = dp.preprocessing_read_in(fileName, 0)
    else:
        global maxIter, learnRate
        maxIter = 2000
        learnRate = 1e-4
        X, y = dp.read_in(fileName, 0)
    trainLoss = []
    validLoss = []
    labels = []
    for i in range(K):
        print("Training the {}th sample, with {} iterations.".format(i+1, maxIter))
        trainX, trainy, validX, validy = dp.valid_split(X, y, len(X)*i/K, len(X)*(i+1)/K)
        bestNN, bestPredict, loss = nn.train(model, trainX, trainy, validX, validy, maxIter, learnRate, batchSize, 0)
        
        trainLoss.append(loss[0])
        validLoss.append(loss[-1])
        labels.append("Sample {}".format(i+1))
        print(bestPredict)
    
    plt.plot_loss(trainLoss, "Loss of Training", labels)
    plt.plot_loss(validLoss, "Loss of Validation", labels)
    return 

def compare_model(fileName):
    X, y = dp.preprocessing_read_in(fileName, 0)
    trainLoss = []
    validLoss = []
    labels = []
    for model in hp.activation_list:
        print("Training model: {}".format(model))
        trainX, trainy, validX, validy = dp.valid_split(X, y, 0, len(X)/K)
        bestNN, bestPredict, loss = nn.train(model, trainX, trainy, validX, validy, maxIter, learnRate, batchSize, 0)
        
        trainLoss.append(loss[0])
        validLoss.append(loss[-1])
        labels.append(model)
    
    plt.plot_loss(trainLoss, "Loss of Training", labels)
    plt.plot_loss(validLoss, "Loss of Validation", labels)


def compare_readin(fileName):
    labels = ["Plain Readin", "With One-hot"]
    readinList = [dp.read_in, dp.preprocessing_read_in]
    trainLoss = []
    validLoss = []
    for f in readinList:
        X, y = f(fileName, 0)
        trainX, trainy, validX, validy = dp.valid_split(X, y, 0, len(X)/K) 
        bestNN, bestPredict, loss = nn.train(model, trainX, trainy, validX, validy, maxIter, learnRate, batchSize, 0)

        trainLoss.append(loss[0])
        validLoss.append(loss[-1])

    plt.plot_loss(trainLoss, "Loss of Training", labels)
    plt.plot_loss(validLoss, "Loss of Validation", labels)

def min_batch(fileName):
    testRange = [-1, 1, 3, 5, 7]
    trainLoss = []
    validLoss = []
    labels = [str(i) for i in testRange]
    X, y = dp.preprocessing_read_in(fileName, 0)
    for i in testRange:
        trainX, trainy, validX, validy = dp.valid_split(X, y, 0, len(X)/K)
        bestNN, bestPredict, loss = nn.train(model, trainX, trainy, validX, validy, maxIter, learnRate, i, 0)

        trainLoss.append(loss[0])
        validLoss.append(loss[-1])

    plt.plot_loss(trainLoss, "Loss of Training", labels)
    plt.plot_loss(validLoss, "Loss of Validation", labels)

def sample_train(trainFile, testFile):
    global maxIter, learnRate
    maxIter = 2000
    learnRate = 1e-3

    trainX, trainy = dp.preprocessing_read_in(trainFile, 0)
    validX, validy = dp.preprocessing_read_in(testFile, 1)
    bestNN, bestPredict, loss = nn.train(model, trainX, trainy, validX, validy, maxIter, learnRate, batchSize, 1)
    plt.plot_loss(loss[0:1], "Loss of Training", ["train set"])
    print("Prediction:\n", bestPredict)

def main():
    global maxIter, learnRate, model

    #train_valid("train.csv", 0)
    #min_batch("train.csv")

    maxIter = 500
    learnRate = 1e-2
    #compare_readin("train.csv")
    #compare_model("train.csv")

    sample_train("sample_train.csv", "sample_test.csv")

if __name__ == "__main__":
    main()

# %%
