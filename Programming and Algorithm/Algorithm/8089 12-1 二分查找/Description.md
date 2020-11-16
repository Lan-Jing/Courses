# 12-1 二分查找

给定一个 n 个元素有序的（升序）整型数组 nums 和一个目标值 target  ，写一个函数搜索 nums 中的 target，如果目标值存在返回下标，否则返回 - 1。
<br>完成函数 int binary_search(vector<int> &num, int target);
<br> 输入有3行，第一行1个数字k，表示数组的数量，第二行k个数字为数组数据，第三行一个数字为搜索的关键字。
<br>样例1
<br>输入:
<br>5
<br>-1 0 3 5 9 12 
<br>9
<br>输出 : 
<br>4
<br> 解释 : 9 出现在 nums 中并且下标为 4
<br> 样例2
<br>5
<br>-1 0 3 5 9 12 
<br>2
<br>输出 : 
<br>-1
<br> 解释 : 2 不存在 nums 中因此返回 - 1