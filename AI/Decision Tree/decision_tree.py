# main body of the decision tree: 
# establish the tree, decision making
import math as mt

modeDict = {
    "ID3" : 0,
    "C4.5" : 1,
    "CART" : 2
}

# definition to nodes on decision trees
class dt_node:
    def __init__(self):
        # define the attribute used in this node
        self.currentAttribute = -1
        # point to the next level
        self.children = {}
        # decision for leave nodes
        self.decision = -1

# entropy function
def entropy(x):
    return -x*mt.log2(x)
# Gini index 
def gini(x):
    gini = 1
    for i in range(len(x)):
        gini -= x[i]**2
    return gini

# pruning threshold
minSize = 150
# check if the end of recursion is reached
# returns: end signal, majorLabel and globalEntropy
def dt_checkLeave(root, availableAttributes, dataMat, labels):
    # try pruning the decision tree
    if len(dataMat) <= minSize:
        return 1, 0, 0
    # only one kind of labels
    for i in range(2):
        if i not in labels:
            root.decision = 1-i
            return 1, 0, 0
    # dataMat is empty, use the default decision from parent
    if len(dataMat) == 0:
        return 1, 0, 0
    # count label frequency
    labelCount = {}
    maxCount = 0
    majorLabel = ""
    sameFlag = 1
    for i in range(len(dataMat)):
        # update the major label
        labelCount[labels[i]] = labelCount.setdefault(labels[i], 0) + 1
        if labelCount[labels[i]] > maxCount:
            maxCount = labelCount[labels[i]]
            majorLabel = labels[i]
        # check if there are available attributes
        if len(availableAttributes) == 0:
            continue
        # check differences on those attributes
        for j in availableAttributes:
            if dataMat[i][j] != dataMat[0][j]:
                sameFlag = 0
    if len(availableAttributes) == 0 or sameFlag == 1:
        root.decision = majorLabel
        return 1, 0, 0
    # compute the global entropy
    globalEntropy = 0
    for count in labelCount.values():
        globalEntropy += entropy(count/len(dataMat))
    return 0, majorLabel, globalEntropy

def ID3_C45_buildTree(root, availableAttributes, dataMat, labels, mode):
    # check whether the current node is leave
    ifEnd, majorLabel, globalEntropy = dt_checkLeave(root, availableAttributes, dataMat, labels)
    if ifEnd == 1:
        return 
    # choose the most helpful attribute
    bestAttribute = -1
    bestImprov = 0
    bestAttriSet = []
    for candidate in availableAttributes:
        # compute entropy of the attribute itself
        attributeEntropy = 0
        attriCount = {}
        for i in range(len(dataMat)):
            attriCount[dataMat[i][candidate]] = attriCount.setdefault(dataMat[i][candidate], 0) + 1
        for count in attriCount.values():
            attributeEntropy += entropy(count/len(dataMat))
        # compute conditional entropy for all values
        conditionEntropy = 0
        conditionCount = {}
        for attri in attriCount.keys():
            conditionCount.clear()
            # count frequency of labels for a specific attribute
            for i in range(len(dataMat)):
                if dataMat[i][candidate] != attri:
                    continue
                conditionCount[labels[i]] = conditionCount.setdefault(labels[i], 0) + 1
            # sum up the conditional entropy
            for label in conditionCount.keys():
                conditionEntropy += attriCount[attri]/len(dataMat) * entropy(conditionCount[label]/attriCount[attri])
        # compute improvement and update the best
        improv = globalEntropy - conditionEntropy
        # divide with entropy from attributes for C4.5
        # print("global:{}, conditional:{}, attribute:{}".format(round(globalEntropy, 4), round(conditionEntropy, 4), round(attributeEntropy, 4)))
        if mode == 1:
            improv /= attributeEntropy
        if improv > bestImprov:
            bestImprov = improv
            bestAttribute = candidate
            bestAttriSet = list(attriCount.keys())
    # determine the feature of current node
    root.currentAttribute = bestAttribute
    # create children 
    for attri in bestAttriSet:
        # fetch the corresponding subset for specific attributes
        subMat = []
        subLabel = []
        for i in range(len(dataMat)):
            if dataMat[i][bestAttribute] != attri:
                continue
            subMat.append(dataMat[i])
            subLabel.append(labels[i])
        # initiate the child node
        newNode = dt_node()
        newNode.decision = majorLabel
        root.children[attri] = newNode
        # recursion
        availableAttributes.remove(bestAttribute)
        ID3_C45_buildTree(newNode, availableAttributes, subMat, subLabel, mode)
        availableAttributes.add(bestAttribute)
    return 

def CART_buildTree(root, availableAttributes, availableValues, dataMat, labels):
    # check whether the current node is leave
    ifEnd, majorLabel, globalEntropy = dt_checkLeave(root, availableAttributes, dataMat, labels)
    if ifEnd == 1:
        return 
    # choose the best (attribute, value) pair 
    minGini = 0x3f3f3f3f
    bestAttribute = -1
    bestValue = ""
    for attribute in availableAttributes:
        for value in availableValues[attribute]:
            # count frequency for: attribute == value and != value, seperately
            GiniCount = [{}, {}]
            valueCount = 0
            for i in range(len(dataMat)):
                if dataMat[i][attribute] == value:
                    valueCount += 1
                    GiniCount[0][labels[i]] = GiniCount[0].setdefault(labels[i], 0) + 1
                else:
                    GiniCount[1][labels[i]] = GiniCount[1].setdefault(labels[i], 0) + 1
            # get frequency
            for i in GiniCount[0].keys():
                GiniCount[0][i] /= valueCount
            for i in GiniCount[1].keys():
                GiniCount[1][i] /= len(dataMat) - valueCount
            # compute Gini index and update the best
            Gini = valueCount/len(dataMat) * gini(list(GiniCount[0].values())) + \
                   (1-valueCount/len(dataMat)) * gini(list(GiniCount[1].values()))
            if Gini < minGini:
                minGini = Gini
                bestAttribute = attribute
                bestValue = value
    # determine the current attribute
    root.currentAttribute = bestAttribute
    # collect sub data set and label set
    subMat = [[], []]
    subLabel = [[], []]
    for i in range(len(dataMat)):
        pos = dataMat[i][bestAttribute] != bestValue
        subMat[pos].append(dataMat[i])
        subLabel[pos].append(labels[i])
    # build left and right children
    for i in range(2):
        newNode = dt_node()
        newNode.decision = majorLabel
        if i == 1:
            # map all other values to tht right child
            # so that test for CART can proceed like ID3 and C4.5
            for otherValues in availableValues[bestAttribute]:
                if otherValues != bestValue:
                    root.children[otherValues] = newNode
        else:
            root.children[bestValue] = newNode
        # recursion
        availableValues[bestAttribute].remove(bestValue)
        if len(availableValues[bestAttribute]) == 0:
            availableAttributes.remove(bestAttribute)
        CART_buildTree(newNode, availableAttributes, availableValues, subMat[i], subLabel[i])
        if len(availableValues[bestAttribute]) == 0:
            availableAttributes.add(bestAttribute)
        availableValues[bestAttribute].add(bestValue)
    return 

# ID3 and C4.5 only differ on the metric,
# implement CART sepearetely
def dt_train(dataMat, labels, attribute, mode):
    root = dt_node()
    # {0, 1, 2, 3, 4, 5}
    availableAttributes = set(attribute.values())
    if mode < 2:
        # recursively build ID3/C4.5 trees
        ID3_C45_buildTree(root, availableAttributes, dataMat, labels, mode)
    else:
        # create set of (attribute, value) pairs
        availableValues = []
        for i in range(len(availableAttributes)):
            availableValues.append(set())
            for j in range(len(dataMat)):
                availableValues[i].add(dataMat[j][i])
        # build CART
        CART_buildTree(root, availableAttributes, availableValues, dataMat, labels)
    return root

def dt_recurTest(root, data):
    # leave node: no child or no child matched
    if len(root.children) == 0 or data[root.currentAttribute] not in root.children:
        return root.decision
    # enter the next level
    nextRoot = root.children[data[root.currentAttribute]]
    return dt_recurTest(nextRoot, data)

# answers only for validation, where ifTest == 0
# return validation accuracy and predction
def dt_test(root, dataMat, labels, ifTest):
    accuracy = 0
    prediction = []
    for i in range(len(dataMat)):
        prediction.append(dt_recurTest(root, dataMat[i]))
        if not ifTest:
            accuracy += prediction[-1] == labels[i]
    return accuracy / len(dataMat), prediction