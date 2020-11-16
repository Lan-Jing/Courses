# plot accuracy 
import matplotlib.pyplot as plt

plotColor = ["r", "g", "b"]

def plot_result(accuracy, model, K):
    for i in range(len(model)):
        plt.plot(range(K), accuracy[i], 'o', c=plotColor[model[i][1]], label=model[i][0])
        plt.plot(range(K), accuracy[i], c=plotColor[model[i][1]])
        # add number labels near those dots
        for x, y in zip(range(K), accuracy[i]):
            plt.text(x, y, y, ha='center', va='bottom', fontsize=10)

    plt.xlabel("Index of Validation Set (K= {})".format(K))
    plt.ylabel("Accuracy")
    plt.title("Result of Decision Tree Classification")
    plt.legend()
    return 