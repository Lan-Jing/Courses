import matplotlib.pyplot as plt

color = ['r', 'g', 'b', 'y', 'c', 'm', 'k', 'w']

def plot_loss(loss, title, labels):
    plt.figure()
    for i in range(len(loss)):
        plt.plot(range(len(loss[i])), loss[i], c=color[i%len(color)], label="{}".format(labels[i]))
        plt.xlabel("Iteration")
        plt.ylabel("loss")
        plt.title(title)
        plt.legend()
    return 