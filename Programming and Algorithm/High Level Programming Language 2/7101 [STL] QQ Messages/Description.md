# [Array / STL] QQ Messages

## Description

Do you use QQ, or other IM application?

You may notice that when you get a message from a certain user/group, the message box will pop to the top of all the message boxes.

Concretely, it behaviors like:

+ If the message box is already in the current list, pop it to the top
+ If the message box is not in the current list, insert it to the top

Now suppose that you have a empty list and the inputs are messages you get. You should output how your list looks like.

## Detail

**Input:** The **first line** is a number(1 <= N <= 50), representing how much messages you will get. N lines following are the users' ids and the messages you get. Each line is a pair, shows like `$id $message`. $id is an integer (100000000 <= $id < 1000000000), $message is a string with **no blank character**.

For example,

```
7
914670787 SeemsUGetAWrongDoor
418057982 MyNameIsVAN
633425541 NewAssignment
914670787 TakeItBoy
418057982 ImAnArtist
418057982 APerformanceArtist
633425541 FinishItInOneHour
```

**Output:** How your list looks like. The format of each line is `"$id:\t$message"`.

For example,

```
633425541:	FinishItInOneHour
418057982:	APerformanceArtist
914670787:	TakeItBoy

```

## Other things

由于大家可能还没有学习STL，因此本题的参考答案提供了两种版本，一种是使用STL的，一种是不使用STL容器的。

如果同学有兴趣尝试STL，可能要查查这些东西

+ `list`
+ `pair`
+ `string`
+ `operator` 重载
+ `iterator`


> 这道题其实是网易游戏研发岗的笔试题的改编，原题比这题简单，但是其实这题也不难，只是要写的多一点而已

