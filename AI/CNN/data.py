import numpy as np
import pathlib as pl
import torch
import torchvision as tv
import os
import pickle
import helper as hp

# n-fold cross-validation
K = 5

# download CIFAR-10 files
def data_download(path):
    trainset = tv.datasets.CIFAR10(root=path, train=True, download=True)
    testset = tv.datasets.CIFAR10(root=path, train=False, download=True)
    return trainset, testset

# load data from CIFAR-10 files downloaded above, return a whole dataset
def data_load(path='../data', train=True):
    print("Path of current working directory: {}".format(pl.Path(__file__).parent.absolute()))
    if os.path.exists(path) == False:
        print("CIFAR-10 dataset not found, downloading.\n")
        _, _ = data_download(path)
    else:
        print("CIFAR-10 dataset found.\n")
    
    data = []
    labels = []
    if train == True:
        for i in range(1, 6):
            with open("{}/cifar-10-batches-py/data_batch_{}".format(path, i), mode='rb') as fp:
                tmpdict = pickle.load(fp, encoding='bytes')
                data.extend(tmpdict[b'data'])
                labels.extend(tmpdict[b'labels'])
    else:
        with open('{}/cifar-10-batches-py/test_batch'.format(path), mode='rb') as fp:
            tmpdict = pickle.load(fp, encoding='bytes')
            data = tmpdict[b'data']
            labels = tmpdict[b'labels']

    labels = np.array(labels)
    # the original data stored in (C, H, W) order
    data = np.reshape(np.array(data), (50000, 3, 32, 32))
    data = np.transpose(data, (0, 2, 3, 1))

    return data, labels

trans = {
    'default' : tv.transforms.ToTensor(),
    'Normalize' : tv.transforms.Compose([
        tv.transforms.ToTensor(),
        tv.transforms.Normalize((0.5, 0.5, 0.5), (0.5, 0.5, 0.5))
    ])
}

# data transform when loading, update trans to add more
def data_transform(data, transform_name='default'):
    newData = np.zeros((np.shape(data)[0], 3, 32, 32))
    for i in range(np.shape(data)[0]):
        newData[i] = trans[transform_name](data[i])
    return newData

# split data for train and validation, then generate each dataloader
def data_split(dataset, batch_size=4, shuffle=True):
    validSize = int(dataset.len/K)
    trainSize = dataset.len - validSize
    trainData, validData = torch.utils.data.random_split(dataset, [trainSize, validSize])
    trainLoader = torch.utils.data.DataLoader(dataset=trainData, batch_size=batch_size, shuffle=shuffle, num_workers=4)
    validLoader = torch.utils.data.DataLoader(dataset=validData, batch_size=batch_size, shuffle=False, num_workers=4)
    return trainLoader, validLoader

# User-defined CIFAR-10 dataset structure
class cifar10Dataset(torch.utils.data.Dataset):
    def __init__(self, path='../data', transform='default', train=True):
        self.data, self.labels = data_load(path=path, train=train)
        self.len = len(self.data)
        # preprocessing, convert data to tensors.
        self.data = data_transform(self.data, transform_name=transform)
        self.labels = torch.from_numpy(self.labels)

    def __getitem__(self, index):
        return self.data[index], self.labels[index]

    def __len__(self):
        return self.len
