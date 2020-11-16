# [Classes and Objects]IntegerSet

## Introduction
Create class IntegerSet for which each object can hold integers in the range 0 through 100. A set is represented
internally as an array of ones and zeros. Array element a[ i ] is 1 if integer i is in the set. Array element a[ j ]
is 0 if integer j is not in the set. The default constructor initializes a set to the so-called “empty-set,” i.e., a set
whose array representation contains all zeros.
Provide member functions for the common set operations. For example, a unionOfSets member function
(already provided) creates a third set that is the set-theoretic union of two existing sets (i.e., an element of the
third array’s is set to 1 if that element is 1 in either or both of the existing sets, and an element of the third set’s
array is set to 0 if that element is 0 in each of the existing sets).
Provide an intersectionOfSets member function which creates a third set which is the set-theoretic intersection
of two existing sets (i.e., an element of the third set’s array is set to 0 if that element is 0 in either or both
of the existing sets, and an element of the third set’s array is set to 1 if that element is 1 in each of the existing sets).
An insertElement member function (already provided) inserts a new integer k into a set (by setting a[ k ]
to 1). If k is invalid, print "Invalid insert attempted!\n".

Provide a deleteElement member function that deletes integer m (by setting a[ m ] to 0). If m is invalid, print

"Invalid delete attempted\n".
A printSet member function (already provided) prints a set as a list of numbers separated by spaces. Print
only those elements which are present in the set (i.e., their position in the array has a value of 1). Print "---" 

without quotation marks for an empty set.

Provide an isEqualTo member function that determines whether two sets are equal.
Provide an additional constructor that receives an array of integers and the size of that array and uses the
array to initialize a set object.

 

## Sample Output
```
Enter set A:
Enter an element (-1 to end): 45
Enter an element (-1 to end): 76
Enter an element (-1 to end): 34
Enter an element (-1 to end): 6
Enter an element (-1 to end): -1
Entry complete
Enter set B:
Enter an element (-1 to end): 34
Enter an element (-1 to end): 8
Enter an element (-1 to end): 93
Enter an element (-1 to end): 45
Enter an element (-1 to end): -1
Entry complete
Union of A and B is:
{ 6 8 34 45 76 93 }
Intersection of A and B is:
{ 34 45 }
Set A is not equal to set B
Inserting 77 into set A...
Set A is now:
{ 6 34 45 76 77 }
Deleting 77 from set A...
Set A is now:
{ 6 34 45 76 }
Invalid insert attempted!
Invalid insert attempted!
Set e is:
{ 1 2 9 25 45 67 99 100 }
```


*From 劳思*