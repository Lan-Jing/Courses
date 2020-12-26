import random
from collections import namedtuple
from checkerboard import *

statusDict1 = {
    # score on the offend side
    "011111" : 100000,
    "211111" : 100000,
    "011110" : 100000,
    "1011101" : 100000,
    "11011011" : 100000,
    "111010111" : 100000,
    "011101" : 10000,
    "011010" : 10000,
    "011100" : 10000,
    "211101" : 10000,
    "211110" : 10000,
    "011102" : 10000,
    "1010101" : 10000,
    "0101010" : 10000,
    "0101012" : 5000,
    "011001" : 5000,
    "210110" : 5000,
    "110011" : 5000,
    "2101012" : 2000,
    "211100" : 2000,
    "010010" : 2000,
    "211010" : 2000,
    "0100110" : 2000,
    "010" : 500,
    "012" : 500,
    "0112" : 500,
    # score on the defend side
    "122221" : 1000000,
    "122200" : 1000000,
    "022120" : 1000000,
    "022122" : 1000000,
    "022121" : 100000,
    "020212" : 10000,
    "021202" : 10000,
    "221202" : 10000,
    "220212" : 10000,
    "021200" : 1000,
}

statusDict2 = {
    "011110" : 500,
    "211110" : 500,
    "011010" : 100,
    "011100" : 100,
    "211100" : 50,
    "211010" : 50,
    "011012" : 50,
    "0110" : 5,
    "2110" : 5,
    "01010" : 5,
    "21010" : 5,
    "010" : 1,
    "210" : 1,
}

class AI:
    def __init__(self, line_points, chessman, maxDepth):
        self._line_points = line_points
        self._my = chessman
        self._opponent = BLACK_CHESSMAN if chessman == WHITE_CHESSMAN else WHITE_CHESSMAN
        self._checkerboard = [[0] * line_points for _ in range(line_points)]
        self._maxDepth = maxDepth

    def get_opponent_drop(self, point):
        if point.Y < 0 or point.X < 0:
            return 
        self._checkerboard[point.Y][point.X] = self._opponent.Value

    def evaluation_rand(self):
        return random.randint(10, 100)

    def evaluation(self, dropList):
        myScore = 0
        opponentScore = 0
        for point in dropList:
            x = point.X
            y = point.Y
            if self._checkerboard[y][x] == self._my.Value:
                myScore += self._get_point_score(point)
            else:
                opponentScore += self._get_point_score(point)
        return myScore / (opponentScore + 1)

    def _get_point_score(self, point):
        score = 0
        for i in range(4):
            score += self._get_direction_score(point, i, statusDict2, 5)
        return score
    
    def _get_direction_score(self, point, direction, statusDict, n):
        # for a given point and a direction, consider the status of n consecutive points on that direction.
        x = point.X - n * offset[direction][0]
        y = point.Y - n * offset[direction][1]
        status = []
        for i in range(-n, n+1):
            if x < 0 or y < 0 or x >= self._line_points or y >= self._line_points:
                status.append("*")
            elif self._checkerboard[y][x] == 0:
                status.append("0")
            else:
                status.append("1" if self._checkerboard[y][x] == self._checkerboard[point.Y][point.X] else "2")
            x += offset[direction][0]
            y += offset[direction][1]
        # then give a score from the status. The given point is status[(n-1)/2]
        score = 0
        status = "".join(status)
        for model in statusDict.keys():
            if status.count(model) > 0:
                score += statusDict[model]
        status = status[::-1]
        for model in statusDict.keys():
            if status.count(model) > 0:
                score += statusDict[model]
        # print(point, i, status, score)
        return score

    # if there's nothing arround the given point, then no need to consider it.
    def _check_surrounding(self, point):
        for x_offset in range(-1, 2):
            for y_offset in range(-1, 2):
                x = point.X + x_offset
                y = point.Y + y_offset
                if x < 0 or y < 0 or x >= self._line_points or y >= self._line_points:
                    continue
                if self._checkerboard[y][x] != 0:
                    return False
        return True

    def _check_win(self):
        for i in range(self._line_points):
            for j in range(self._line_points):
                if self._checkerboard[i][j] == 0:
                    continue
                for direction in range(4):
                    if self._get_count_on_direction(Point(j, i), direction) == 0:
                        continue
                    return self._checkerboard[i][j]
        return 0

    def _get_count_on_direction(self, point, direction):
        for way in (-1, 1):
            count = 1
            x = point.X
            y = point.Y
            for i in range(1, 5):
                x += offset[direction][0] if way == 1 else -offset[direction][0]
                y += offset[direction][1] if way == -1 else -offset[direction][1]
                if 0 <= x < self._line_points and 0 <= y < self._line_points and self._checkerboard[y][x] == self._checkerboard[point.Y][point.X]:
                    count += 1
                else:
                    break
            if count >= 5:
                return True
        return False

    def _minmax_dfs(self, step, alpha, beta, nodeList):
        dropPoint = None
        winner = self._check_win()
        if winner != 0:
            return 0 if winner == self._opponent.Value else 1e5, None
        if step > self._maxDepth:
            return self.evaluation(nodeList), None
        # the max node
        if step % 2 == 1:
            value = 0
            for i in range(self._line_points):
                for j in range(self._line_points):
                    if self._checkerboard[i][j] != 0 or self._check_surrounding(Point(j, i)) != 0:
                        continue

                    self._checkerboard[i][j] = self._my.Value
                    nodeList.append(Point(j, i))
                    tmpValue = self._minmax_dfs(step+1, alpha, beta, nodeList)[0]
                    self._checkerboard[i][j] = 0
                    nodeList.pop()
                    
                    if tmpValue >= value:
                        value = tmpValue
                        dropPoint = Point(j, i)
                    alpha = max(alpha, value)
                    if alpha >= beta:
                        return value, dropPoint
        # the min node
        else:
            value = 1e5
            for i in range(self._line_points):
                for j in range(self._line_points):
                    if self._checkerboard[i][j] != 0 or self._check_surrounding(Point(j, i)) != 0:
                        continue

                    self._checkerboard[i][j] = self._opponent.Value
                    nodeList.append(Point(j, i))
                    tmpValue = self._minmax_dfs(step+1, alpha, beta, nodeList)[0]
                    self._checkerboard[i][j] = 0
                    nodeList.pop()

                    if tmpValue <= value:
                        value = tmpValue
                        dropPoint = Point(j, i)
                    beta = min(beta, value)
                    if alpha >= beta:
                        return value, dropPoint

        return value, dropPoint
    
    def AI_drop(self):
        score, dropPoint = self._minmax_dfs(1, -1e10, 1e10,[])
        if dropPoint is None:
            dropPoint = Point((int)((self._line_points-1)/2), (int)((self._line_points-1)/2))
        
        self._checkerboard[dropPoint.Y][dropPoint.X] = self._my.Value
        return score, dropPoint