# 【Array】 打印魔方阵

## Description
所谓魔方阵，古代又称“纵横图”，是指组成元素为自然数1、2…n<sup>2</sup>的平方的n×n的方阵，其中每个元素值都不相等，且每行、每列以及主、副对角线上各n个元素之和都相等。例如三阶魔方阵为：  
```
8 1 6  
3 5 7  
4 9 2 
```
## Input
输入一个小于等于15的整数（**注意：本题只考虑奇数阶情况**）。
# Output
输出对应阶数的魔方阵。
## Example input
```
9
```

## Example output
```
  47  58  69  80  1  12  23  34  45
  57  68  79  9  11  22  33  44  46
  67  78  8  10  21  32  43  54  56
  77  7  18  20  31  42  53  55  66
  6  17  19  30  41  52  63  65  76
  16  27  29  40  51  62  64  75  5
  26  28  39  50  61  72  74  4  15
  36  38  49  60  71  73  3  14  25
  37  48  59  70  81  2  13  24  35
```
## Hint
魔方阵的排列规律如下：  
1. 将1放在第一行中间一列；
2. 从2开始直到n×n止各数依次按下列规则存放；每一个数存放的行比前一个数的行数减1，列数加1（例如上面的三阶魔方阵，5在4的上一行后一列）；
3. 如果上一个数的行数为1，则下一个数的行数为n(指最下一行);例如1在第一行，则2应放在最下一行，列数同样加1；
4. 当上一个数的列数为n时，下一个数的列数应为1，行数减去1。例如2在第3行最后一列，则3应放在第二行第一列；
5. 如果按上面规则确定的位置上已有数，或上一个数是第一行第n列时，则把下一个数放在上一个数的下面。例如按上面的规定，4应该放在第1行第2列，但该位置已经被占据，所以4就放在3的下面；
	