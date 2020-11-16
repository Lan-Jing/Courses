# [Inheritance]Employee

要求编写一个简单的人员信息管理程序，具体要求如下：

（1）程序涉及到五个类，分别是Employee，Technician，Salesman，Manager，Salesmanager。这五个类的关系为：Employee是顶层父类，Technician，Salesman，Manager是employee的子类，Salesmanager是Salesman，Manager的子类。

（2）Employee具有姓名，有设置姓名，获取姓名，获取收入等函数。

（3）Technician的收入计算方法为`工作小时数*时薪`，另外具有设置工作小时数，时薪的函数，具有工作小时数和时薪的属性。Salesman的收入计算方法为`销售额*提成比例`，另外具有设置销售额，提成比例的函数，具有销售额和提成比例的属性。Manager的收入计算方法为固定月薪，另外具有设置固定月薪的函数，具有固定月薪的属性。Salesmanager收入计算方法为`销售额*提成比例+固定月薪`。

（4）前面的要求不重要，因为所有函数的实现代码已给出。你的任务是写头文件。参考Employee.cpp。

**提示：收入是一个人的相关信息，当Salesmanager被问起收入时，不管这时候他/她是作为Salesman还是Manager，回答都是个人的总收入。**

> 通过编译，你就成功了一半！
