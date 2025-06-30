# Chapter 2 : Linear Table

Main Content:  

1. Linear List
2. Array
3. Sequential List

---

- [1. Linear List(线性表)](#1-linear-list线性表)
    - [1.1. Operations of Linear List](#11-operations-of-linear-list)
    - [1.2. Array](#12-array)
        - [1.2.1. Special Matrices Storage](#121-special-matrices-storage)
    - [1.3. Sequential List](#13-sequential-list)
- [2. Linked List](#2-linked-list)
    - [2.1. Singly Linked List(单链表)](#21-singly-linked-list单链表)
        - [Iterator of Linked List](#iterator-of-linked-list)
    - [2.2. Loop Linked List](#22-loop-linked-list)
    - [2.3. Double Linked List](#23-double-linked-list)
    - [2.4. Static Linked List](#24-static-linked-list)

---

## 1. Linear List(线性表)

a sequential ordered list with $n$ elements($n \geq 0$)  

can be denoted as:  

$$
L = (k_0, k_1, k_2, \cdots, k_{n-1})
$$

in which, the $k_0$ is **head** and the $k_{n-1}$ is **tail**  

**direct previous element(直接前驱)**  

**direct next element(直接后继)**  

### 1.1. Operations of Linear List

- access

- update

- search

- insert

- delete

### 1.2. Array

the list whose elements are in the same data type  
and the elements can be directly stored and accessed by index

**static array**  

**dynamic array**  

advantages:  
can randomly access or modify the value of the element  

#### 1.2.1. Special Matrices Storage

here, the special matrices means those matrices whose non-zero or zero elements are distributed in some special patterns  

compressed storage is mainly used to store the special matrices which is in high degree  

### 1.3. Sequential List

definition:  
put the elements in a linear list one by one in a continuous storage space  
can be denoted as one-dimensional array  

features:  
the logical sequence and the physical sequence of the elements are the same  
so they can be accessed randomly  

compared with the Linear List, the sequential list has one more constraint:  
the elements must share the same data type  

## 2. Linked List

Sequential List *(we usually call Array in C)* is good as any of its elements can be accessed randomly in $O(1)$ time  

but when we need to insert or delete an element from it, we need $O(n)$ time  

and static allocation for sequential list is hard to determine the size,  
which may cause waste of space or overflow  

for dynamic allocation,  
we can extend the space but the time consumption is still $O(n)$  

so the linked list can resolve these problems  
whose elements can be stored in any place  
every one keep the next element's address info(denoted as pointer)  

linked list is proper for the situation  
where frequent insertion or deletion and flexible storage need are required  

### 2.1. Singly Linked List(单链表)

every element is called a **node**  
which is composed of two parts:  

1. data domain  
2. pointer(link) domain  

one important property of linked list is that:  
the physical sequence of the elements can differ from the logical sequence  
which enables the convenient extension of the list  

#### Iterator of Linked List

Iterator, usually translated as **迭代器**, we can also call it **游标类**  
which is designed to traverse(遍历) the elements of the list  

definition principles:  

- should be friend class of the List and NodeList
- keep the reference of the existing List object  
- should record the current position of the traversal  
- should offer some checking and retrieving operations  

### 2.2. Loop Linked List

### 2.3. Double Linked List

### 2.4. Static Linked List
