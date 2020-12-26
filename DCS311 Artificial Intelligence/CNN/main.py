#%%
import data
import helper as hp
import cnn
import torch
import torch.nn as nn
import torch.nn.functional as F
import torch.optim as optim

transform = 'default'

learnRate = 0.001
momentum = 0.9
epoch = 10

criterion = nn.CrossEntropyLoss()

num_device = 2
device = torch.device("cuda:{}".format(num_device) if torch.cuda.is_available() else "cpu")
tofile = 1

def main():
    cifar = data.cifar10Dataset(path='../data', transform=transform, train=True)
    trainLoader, validLoader = data.data_split(cifar, batch_size=4, shuffle=True)

    # output sample graphs
    hp.print_sample(trainLoader, transform != 'default')

    # define network and optimizer
    net = cnn.VGG3_final().float().to(device)
    optimizer = optim.SGD(net.parameters(), learnRate, momentum)

    # training
    accList, lossList = cnn.train(net, optimizer, criterion, trainLoader, validLoader, epoch, device)
    # plot training and validation loss
    hp.plot_curve(lossList, "Train and Validation Loss", ["train", "validation"], tofile)
    hp.plot_curve(accList, "Train and Validation Accuracy", ["train", "validation"], tofile)
    # testing
    cnn.test(net, validLoader, device)

if __name__ == "__main__":
    main()
# %%
