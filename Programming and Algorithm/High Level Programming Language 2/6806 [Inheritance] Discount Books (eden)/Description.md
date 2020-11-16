# [Inheritance]Discount Books (eden)

Hateful Ou is working for a bookstore, and now the boss tell him to make a plan about discount books.

Books will be discounted when the salse is more than minQty.

Hateful Ou is so stupid that he want you to help him finish.

 

BookItem is the Base class of a book, which records the name and the price :

(1) member function getName() is just return the name of the book;

(2) member function netPrice( cnt ) is return the total price of the book you buy : `cnt * price`.

 

BulkItem is the derived class from BookItem :

(1) discount_ means the discount;

(2) minQty_ means the minimun quantity : if cnt is more than minQty, it will be discounted;

(3) also rewrite the function netPrice( cnt ) : when `cnt > minQty`,  return `cnt * price * discount`, else return `cnt * price`.



Sample input:
```
a
10
10
b
2.5
10
c
5.5
5
0.8
8
```
 

Sample out:

```
a
100.00
b
25.00
c
35.20
```
 
