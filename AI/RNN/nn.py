import torch
import torch.nn as nn
import torch.nn.functional as F
import torch.nn.utils.rnn as rnn
import torch.optim as optim

class LSTM(nn.Module):
    def __init__(self, vec_dim, hidden_dim, out_dim):
        super(LSTM, self).__init__()
        self.lstm = nn.LSTM(vec_dim, hidden_dim, batch_first = True)
        self.fc1 = nn.Linear(hidden_dim, 128)
        self.fc2 = nn.Linear(128, out_dim)
        self.dropout = nn.Dropout(0.5)
        
    def forward(self, x):
        x, _ = self.lstm(x)
        x, _ = rnn.pad_packed_sequence(x, batch_first=True)

        x = self.dropout(x)
        x = self.fc1(F.relu(x))
        x = self.dropout(x)
        x = self.fc2(F.relu(x))
        
        return x
    
# pack padded data returned from loaders
def pack_data(data, labels, data_len, device):
    data = rnn.pack_padded_sequence(data, data_len, batch_first=True)
    labels = rnn.pack_padded_sequence(labels, data_len, batch_first=True)[0]
    return data.to(device), labels.to(device)
    
def train(net, optimizer, criterion, trainLoader, validLoader, epoch, device):
    accList = [[], []]
    lossList = [[], []]

    for i in range(epoch):
        loss = 0
        for data, labels, data_len in trainLoader:
            data, labels = pack_data(data, labels, data_len, device)
            
            optimizer.zero_grad()
            output = net(data)
            output = rnn.pack_padded_sequence(output, data_len, batch_first=True)[0]
            batchLoss = criterion(output, labels)
            batchLoss.backward()
            optimizer.step()
            loss += batchLoss.item()
        lossList[0].append(loss / len(trainLoader))
        accList[0].append(test(net, trainLoader, device))

        loss = 0
        for data, labels, data_len in validLoader:
            data, labels = pack_data(data, labels, data_len, device)
            
            with torch.no_grad():
                output = net(data)
                output = rnn.pack_padded_sequence(output, data_len, batch_first=True)[0]
                batchLoss = criterion(output, labels)
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
        for data, labels, data_len in testLoader:
            data, labels = pack_data(data, labels, data_len, device)
            
            output = net(data)
            output = rnn.pack_padded_sequence(output, data_len, batch_first=True)[0]
            _, predicted = torch.max(output, dim=-1)

            ptr = 0
            for l in data_len:
                now_predict = predicted[ptr:ptr+l]
                now_label = labels[ptr:ptr+l]

                accuracy += torch.equal(now_predict, now_label)
                total += 1
                ptr += l
    return accuracy / total * 100
            