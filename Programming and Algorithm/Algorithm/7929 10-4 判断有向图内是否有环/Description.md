# 10-4 判断有向图内是否有环

<br>给定一个有向图，判断其中是否存在环，有的话输出has circle，没有输出no circle
<br>请完成函数void Graphcircle(GraphList *g);
<br>输入包含3个部分
<br>第一个部分为第一行两个数k和m表示顶点数和边数
<br>第二部分为第二行有k个数为顶点的值（与编号相同）
<br>第三部分为后面m行，每行为vi vj w分别表示从节点i到节点j的边权值为5
<br>输出为一行，有环或无环
<br>输入
<br>8 9
<br>0 1 2 3 4 5 6 7 
<br>0 1 1
<br>1 5 1
<br>0 2 1
<br>2 3 1
<br>3 4 1
<br>4 5 1
<br>4 6 1
<br>6 7 1
<br>7 3 1
<br>输出
<br>has circle
<br>输入
<br>5 6
<br>0 1 2 3 4
<br>1 0 9
<br>1 2 3
<br>2 0 2
<br>2 3 5
<br>3 4 1
<br>0 4 6
<br>输出
<br>no circle