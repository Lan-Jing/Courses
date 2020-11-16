#%%
import numpy as np
import matplotlib.pyplot as plt

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

def plot_curve_from_file(title, labels):
    data = np.load("../data/{}.npy".format(title))
    plot_curve(data, title, labels, 0)

labels = ["Train", "Validation"]
kind = ["Accuracy", "Loss"]
title = "Train and Validation {} of {}".format(kind[1], "simple LSTM, with Dropout")

plot_curve_from_file(title, labels)
# %%
