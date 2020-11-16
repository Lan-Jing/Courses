# [Class and Objects]Sequence

# Description
```
Design a class named Sequence to support several operators.
The class contains:
0、A no-arg constructor that creates a default Sequence.
1、A function named size(),return the number of the element in the sequence.
2、A function named empty(), return whether the sequence has no element.
3、A function named find(int value) that return whether the value is in the sequence.
4、A function named at(int k), return the reference of the k-th element in the sequence. 0 <= k <= size()-1.
5、A function named front(), return the first element in the sequence.
6、A function named back(), return the last element in the sequence.
7、A function named insert(int value), insert an int value to the back of the sequence.
8、A function named insert(int k, int value), insert an int value to the sequence before the k-th element.
9、A function named clear(), clear all the element in the sequence, then the sequence became empty.
10、A function named reverse(), reverse all element in the sequence.
11、A function named reverse(int fir, int las), reverse all elements from the fir-th element to the (las-1)-th element.
12、A function named replace(int value1, int value2), replace the element with value1 to value2.
13、A function named swap(Sequence &seq2), swap all elements from seq2 to seq, also seq to seq2.
 
Function explain, we define the initial state of the sequence seq1 with four integers {1,2,3,3}, another sequence seq2 is {3,4,5}
called size():      4
called empty():     false
called find(5):     false (because 5 is not in the sequence)
called at(3):       return the reference of third element
called front():     1 
called back():      3
called insert(6):   the sequence became {1,2,3,3,6} 
called insert(2,4): the sequence became {1,2,4,3,3} 
called clear():     the sequence became {}
called reverse:     the sequence became {3,3,2,1}
called reverse(1,3): the sequence became {1,3,2,3}
called replace(3,5): the sequence became {1,2,5,5}
called swap(seq2)  : seq1 became {3,4,5} and seq2 became {1,2,3,3}
 
you can assume that the total of the operators is not greater thant 1000.
 
public:
Sequence();
int size();
bool empty();
bool find(int value);
int &at(int pos);
int front();
int back();
void insert(int value);
void insert(int pos, int value);
void clear();
void reverse();
void reverse(int fir, int las);
void replace(int value1, int value2);
void swap(Sequence &seq2);
};
 
```