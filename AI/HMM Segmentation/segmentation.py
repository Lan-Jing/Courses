#%%
import numpy as np

class HMM_segmentation():
    def __init__(self):
        self.statusSet = ("B", "M", "E", "S")
        self.observedSet = set()
        self.TransProbMatrix = {}
        self.EmitProbMatrix = {}
        self.initStatus = {}

        for s_i in self.statusSet:
            self.initStatus[s_i] = 0
            self.TransProbMatrix[s_i] = {}
            self.EmitProbMatrix[s_i] = {}
            for s_j in self.statusSet:
                self.TransProbMatrix[s_i][s_j] = 0

    def label(self, len):
        out = ""
        if len == 1:
            out = "S"
        elif len >= 3:
            out = "B" + "M"*(len-2) + "E"
        else:
            out = "B" + "E"        
        return out

    def train(self, filePath):
        with open(filePath, encoding='utf-8') as fp:
            for line in fp.readlines():
                wordList = line.strip().split()
                lineString = ""
                lineState = ""
                # generate concatenated string and labels for each character
                for word in wordList:
                    lineString += word
                    lineState += self.label(len(word))
                # collect the transition and emission matrix.
                for i in range(len(lineString)):
                    char = lineString[i]
                    state = lineState[i]
                    self.observedSet.add(char)
                    self.EmitProbMatrix[state][char] = self.EmitProbMatrix[state].get(char, 0) + 1
                    if i == 0:
                        self.initStatus[state] += 1
                    else:
                        prevState = lineState[i-1]
                        self.TransProbMatrix[prevState][state] += 1
        # unify all matricies
        initCount = sum(self.initStatus.values())
        for s_i in self.statusSet:
            self.initStatus[s_i] /= initCount

            count = sum(self.TransProbMatrix[s_i].values())
            for s_j in self.statusSet:
                self.TransProbMatrix[s_i][s_j] /= count

            count = sum(self.EmitProbMatrix[s_i].values())
            for char in self.EmitProbMatrix[s_i].keys():
                self.EmitProbMatrix[s_i][char] /= count

    def dp_viterbi(self, textLine):
        statePath = {}
        bestPros = []
        T = len(textLine)
        for t in range(T):
            bestPros.append({})
            char = textLine[t]
            # check if this char is ever seen in the training set, then compute its emission prob.
            seen = 0
            for state in self.statusSet:
                seen = 1 if char in self.EmitProbMatrix[state].keys() else seen
            if t == 0:
                for state in self.statusSet:
                    emit = self.EmitProbMatrix[state].get(char, 0) if seen else 1
                    bestPros[t][state] = self.initStatus[state]*emit
                    statePath[state] = state
            else:
                newStatePath = {}
                for state in self.statusSet:
                    emit = self.EmitProbMatrix[state].get(char, 0) if seen else 1
                    (prob, prevState) = max([(bestPros[t-1][prevState]*self.TransProbMatrix[prevState][state]*emit, prevState) \
                                            for prevState in self.statusSet])
                    bestPros[t][state] = prob
                    newStatePath[state] = statePath[prevState] + state
                statePath = newStatePath

        # only "E" and "S" are possible choice for end of the line
        if bestPros[T-1]["E"] > bestPros[T-1]["S"]:
            return bestPros[T-1]["E"], statePath["E"]
        else:
            return bestPros[T-1]["S"], statePath["S"]

    def test(self, testFilePath, resFilePath):
        seg_res = []
        seg_hmm = []
        with open(resFilePath, encoding="utf-8") as fp:
            for line in fp.readlines():
                seg_res.append(line.strip().split())
        with open(testFilePath, encoding="utf-8") as fp:
            for line in fp.readlines():
                seg_hmm.append([])
                _, path = self.dp_viterbi(line.strip())
                for i in range(len(path)):
                    if path[i] == "B" or path[i] == "S":
                        seg_hmm[-1].append(line[i])
                    else:
                        seg_hmm[-1][-1] += line[i]
        
        nTest = 0
        nRes = 0
        nMatched = 0
        for i in range(len(seg_res)):
            # print(seg_res[i], seg_hmm[i])
            nTest += len(seg_hmm[i])
            nRes += len(seg_res[i])
            for seg in seg_hmm[i]:
                nMatched += 1 if seg in seg_res[i] else 0

        precision = nMatched / nTest
        recall = nMatched / nRes
        print("F1 of HMM segmentation: {}".format(2*precision*recall/(precision+recall)))

def main():
    hmm = HMM_segmentation()
    hmm.train("../data/msr_training.utf8")
    hmm.test("../data/msr_test.utf8", "../data/msr_test_gold.utf8")

if __name__ == "__main__":
    main()
# %%
