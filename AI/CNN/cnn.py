import numpy as np
import torch
import torchvision
import torch.nn as nn
import torch.nn.functional as F

class base(nn.Module):
    def __init__(self):
        super(base, self).__init__()
        self.conv1 = nn.Conv2d(3, 6, 5)
        self.pool = nn.MaxPool2d(2, 2)
        self.conv2 = nn.Conv2d(6, 16, 5)
        self.fc1 = nn.Linear(16 * 5 * 5, 120)
        self.fc2 = nn.Linear(120, 84)
        self.fc3 = nn.Linear(84, 10)

    def forward(self, x):
        x = self.pool(F.relu(self.conv1(x)))
        x = self.pool(F.relu(self.conv2(x)))
        x = x.view(-1, 16 * 5 * 5)
        x = F.relu(self.fc1(x))
        x = F.relu(self.fc2(x))
        x = self.fc3(x)
        return x

class VGG3(nn.Module):
    def __init__(self):
        super(VGG3, self).__init__()
        self.feature = nn.Sequential(
            nn.Conv2d(3, 32, kernel_size=3, padding=1),
            nn.ReLU(True),
            nn.MaxPool2d(kernel_size=2, stride=2),

            nn.Conv2d(32, 64, kernel_size=3, padding=1),
            nn.ReLU(True),
            nn.MaxPool2d(kernel_size=2, stride=2),

            nn.Conv2d(64, 128, kernel_size=3, padding=1),
            nn.ReLU(True),
            nn.MaxPool2d(kernel_size=2, stride=2)
        )
        self.classifier = nn.Sequential(
            nn.Linear(2048, 4096),
            nn.ReLU(True),
            nn.Dropout(),
            nn.Linear(4096, 10)
        )

    def forward(self, x):
        #print(x.shape)
        out = self.feature(x)
        #print(out.shape)
        out = out.view(out.size(0), -1)
        #print(out.shape)
        out = self.classifier(out)
        #print(out.shape)
        return out

class VGG3_batchNorm(nn.Module):
    def __init__(self):
        super(VGG3_batchNorm, self).__init__()
        self.feature = nn.Sequential(
            nn.Conv2d(3, 32, kernel_size=3, padding=1),
            nn.BatchNorm2d(32),
            nn.ReLU(True),
            nn.MaxPool2d(kernel_size=2, stride=2),

            nn.Conv2d(32, 64, kernel_size=3, padding=1),
            nn.BatchNorm2d(64),
            nn.ReLU(True),
            nn.MaxPool2d(kernel_size=2, stride=2),

            nn.Conv2d(64, 128, kernel_size=3, padding=1),
            nn.BatchNorm2d(128),
            nn.ReLU(True),
            nn.MaxPool2d(kernel_size=2, stride=2)
        )
        self.classifier = nn.Sequential(
            nn.Linear(2048, 1024),
            nn.ReLU(True),
            nn.Dropout(),
            nn.Linear(1024, 10)
        )

    def forward(self, x):
        out = self.feature(x)
        out = out.view(out.size(0), -1)
        out = self.classifier(out)
        return out

class VGG3_final(nn.Module):
    def __init__(self):
        super(VGG3_final, self).__init__()
        self.feature = nn.Sequential(
            nn.Conv2d(3, 32, kernel_size=3, padding=1),
            nn.BatchNorm2d(32),
            nn.ReLU(True),
            nn.MaxPool2d(kernel_size=2, stride=2),
            nn.Dropout(0.2),

            nn.Conv2d(32, 64, kernel_size=3, padding=1),
            nn.BatchNorm2d(64),
            nn.ReLU(True),
            nn.MaxPool2d(kernel_size=2, stride=2),
            nn.Dropout(0.3),

            nn.Conv2d(64, 128, kernel_size=3, padding=1),
            nn.BatchNorm2d(128),
            nn.ReLU(True),
            nn.MaxPool2d(kernel_size=2, stride=2),
            nn.Dropout(0.4)
        )
        self.classifier = nn.Sequential(
            nn.Linear(2048, 1024),
            nn.ReLU(True),
            nn.Dropout(0.5),
            nn.Linear(1024, 10)
        )

    def forward(self, x):
        out = self.feature(x)
        out = out.view(out.size(0), -1)
        out = self.classifier(out)
        return out

def train(net, optimizer, criterion, trainLoader, validLoader, epoch, device):
    print("Training {} rounds.\n".format(epoch))
    accList = [[], []]
    lossList = [[], []]
    for i in range(epoch):
        loss = 0
        for batch in trainLoader:
            data, labels = batch
            data, labels = data.to(device), labels.to(device)

            optimizer.zero_grad()

            outputs = net(data.float())
            batchLoss = criterion(outputs, labels)
            batchLoss.backward()
            optimizer.step()

            loss += batchLoss.item()
        lossList[0].append(loss / len(trainLoader))
        accList[0].append(test(net, trainLoader, device))
        # validation after each epoch
        loss = 0
        for batch in validLoader:
            data, labels = batch

            with torch.no_grad():
                outputs = net(data.float())
                batchLoss = criterion(outputs, labels)
                loss += batchLoss.item()
        lossList[1].append(loss / len(validLoader))
        accList[1].append(test(net, validLoader, device))
        print('epoch:{}, trainLoss:{}, validLoss:{}'.format(i+1, lossList[0][-1], lossList[1][-1]))
        print('epoch:{}, TrainAcc:{}, validAcc:{}'.format(i+1, accList[0][-1], accList[1][-1]))

    return accList, lossList

def test(net, testLoader, device):
    total = 0
    accuracy = 0
    with torch.no_grad():
        for batch in testLoader:
            data, labels = batch
            data, labels = data.to(device), labels.to(device)

            outputs = net(data.float())
            _, predicted = torch.max(outputs.data, 1)
            total += len(labels)
            accuracy += (predicted == labels).sum().item()

    return accuracy / total * 100