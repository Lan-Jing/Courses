# main function
#%%
import readin_and_split as read
import decision_tree as dt
import plot as pl

# fold of cross validation
K = 10

dataMat, labels, attri = read.load_data("car_train.csv")
resultList = []
for mode in dt.modeDict.items():
    resultList.append([])
    for i in range(K):
        trainMat, trainLabel, validMat, validLabel = read.split_data(dataMat, labels, \
                                                                     len(dataMat) * i/K, len(dataMat) * (i+1)/K)
        DTtree = dt.dt_train(trainMat, trainLabel, attri, mode[1])
        accuracy, prediction = dt.dt_test(DTtree, validMat, validLabel, 0)
        resultList[-1].append(round(accuracy, 4))
        print("Model: {}, accuracy: {}".format(mode[0], accuracy))
pl.plot_result(resultList, list(dt.modeDict.items()), K)
# %%
