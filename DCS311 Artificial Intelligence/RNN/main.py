import nn as rnn
import torch
import torch.nn as nn
import torch.nn.functional as F
import torch.optim as optim

import helper as hp
import data

learnRate = 0.0005
momentum = 0.9
epoch = 50

criterion = nn.CrossEntropyLoss()

num_device = 2
device = torch.device("cuda:{}".format(num_device) if torch.cuda.is_available() else "cpu")

lossTitle = "Train and Validation Loss of simple LSTM, with Dropout"
accTitle = "Train and Validation Accuracy of simple LSTM, with Dropout"
tofile = 1

def main():
    semEval = data.semEvelDataset()
    trainLoader, validLoader = data.data_split(semEval, batch_size=64)
    
    # define network and optimizer
    net = rnn.LSTM(100, 512, 2).float().to(device)
    optimizer = optim.Adam(net.parameters(), learnRate)

    # training
    accList, lossList = rnn.train(net, optimizer, criterion, trainLoader, validLoader, epoch, device)
    # plot training and validation loss
    hp.plot_curve(lossList, lossTitle, ["train", "validation"], tofile)
    hp.plot_curve(accList, accTitle, ["train", "validation"], tofile)
    
if __name__ == "__main__":
    main()