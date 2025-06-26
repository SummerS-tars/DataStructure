# Chapter 2 : Linear Table

Main Content:  

1. Linear List
2. Array
3. Sequential List

---

- [1. Linear List(线性表)](#1-linear-list线性表)
    - [Operations of Linear List](#operations-of-linear-list)
    - [Array](#array)
        - [Special Matrices Storage](#special-matrices-storage)
    - [Sequential List](#sequential-list)

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

### Operations of Linear List

- access

- update

- search

- insert

- delete

### Array

the list whose elements are in the same data type  
and the elements can be directly stored and accessed by index

**static array**  

**dynamic array**  

advantages:  
can randomly access or modify the value of the element  

#### Special Matrices Storage

here, the special matrices means those matrices whose non-zero or zero elements are distributed in some special patterns  

compressed storage is mainly used to store the special matrices which is in high degree  

### Sequential List

definition:  
put the elements in a linear list one by one in a continuous storage space  
can be denoted as one-dimensional array  

features:  
the logical sequence and the physical sequence of the elements are the same  
so they can be accessed randomly  

compared with the Linear List, the sequential list has one more constraint:  
the elements must share the same data type  
