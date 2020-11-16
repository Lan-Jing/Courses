#%%
import numpy as np
import matplotlib.pyplot as plt
# import torchvision as tv

category = (
    'plane', 'car', 'bird', 'cat', 'deer',
    'dog', 'frog', 'horse', 'ship', 'truck'
)

def merge_dict(dict1, dict2):
    return dict1.update(dict2)

def imgshow(img, ifNormalized):
    if ifNormalized:
        img = img / 2 + 0.5
    npimg = img.numpy()
    plt.imshow(np.transpose(npimg, (1, 2, 0)))
    plt.show()

def print_sample(dataLoader, ifNormalized):
    dataIter = iter(dataLoader)
    images, labels = dataIter.next()
    imgshow(tv.utils.make_grid(images), ifNormalized)
    print('Labels to the above:', ' '.join('%5s' % category[labels[i]] for i in range(len(labels))))

color = ('r', 'g', 'b', 'y')

def plot_curve(data, title, labels, tofile):
    if tofile == 1:
        writeData = np.array(data)
        np.save("../data/{}.npy".format(title), writeData)
        print("Save {}.npy done".format(title))
    else:
        plt.figure()
        for i in range(len(data)):
            plt.plot(range(len(data[i])), data[i], c=color[i%len(color)], label="{}".format(labels[i]))
            plt.xlabel("Epoch")
            plt.title(title)
            plt.legend()
            print("final:{}".format(data[i][-1]))

def plot_curve_from_file(title, labels):
    data = np.load("../data/{}.npy".format(title))
    plot_curve(data, title, labels, 0)

labels = ["Train", "Validation"]
kind = ["Accuracy", "Loss"]
title = "Train and Validation {} of {}".format(kind[1], "VGG3_final")

plot_curve_from_file(title, labels)
# %%
