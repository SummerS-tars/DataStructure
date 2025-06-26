# Chapter 1 : Introduction to Data Structures

Class Structure:

1. Basic Data Structure
2. Linear Table
3. String
4. Stack and Queue
5. Recursion and Generalized Table
6. Tree

---

- [1. Data](#1-data)
    - [1.1. Category of Data Structure](#11-category-of-data-structure)
        - [1.1.1. Linear Structure](#111-linear-structure)
        - [1.1.2. Non-Linear Structure](#112-non-linear-structure)
    - [1.2. Class Content](#12-class-content)
- [2. Data Type](#2-data-type)
- [3. Analyzing the Performance of Algorithms](#3-analyzing-the-performance-of-algorithms)
    - [3.1. Previous Estimation](#31-previous-estimation)
    - [3.2. Asymptotic Analysis](#32-asymptotic-analysis)
        - [3.2.1. Big O Notation](#321-big-o-notation)
        - [3.2.2. Ω and Θ Notation](#322-ω-and-θ-notation)

---

## 1. Data

**Data**: the carrier of info, can be processed by computer  

**Data element**: the smallest unit of data, can be composed of multiple data items

**Data item**: the relatively independent data unit in a data element

<font color="yellow">**Data structure** is composed of some set of data elements and the relationship between them</font>

which can be denoted as:

$$
S = \{D, R\}
$$

$D$ is some set of data elements, $R$ is some relationship between the data elements

### 1.1. Category of Data Structure

Two main types:

1. **Linear Structure**
2. **Non-Linear Structure**

#### 1.1.1. Linear Structure

also as **Linear Table**  
the elements in which are arranged in some order in a sequence  

except for the 1st and the last elements,  
every other element has one and only one previous element  
and one and only one next element  

can be divided into several more detailed types  
not mentioned here  

#### 1.1.2. Non-Linear Structure

the elements in this structure are no longer in a linear sequence  
that is to say,  
every element can be related to none or multiple other elements  

according to the relationship between the elements,  
the structure can be divided into:  

1. hierarchical structure  
    e.g. tree structure  
2. group structure  
    e.g. graph structure  
    network structure  

### 1.2. Class Content

<font size="5" color="yellow">program = data structure + algorithm</font>

the Center is **Data**  

3 hierarchies and 5 "elements" :

1. **abstraction**  
    1. logical structure  
        logical relationship between data elements
    2. basic calculation
2. **realization**
    1. storage structure  
        the denotation of the data elements and there relationship in the computer  
        for types, 1-2 commonly used in memory, 3-4 commonly used in disk  
        1. sequential storage
        2. linked storage
        3. indexed storage
        4. hashing storage
    2. algorithm
3. **evaluation**
    1. analysis of different data structures and algorithms

the core is <font size="5" color="yellow">decomposition</font> and <font size="5" color="yellow">abstraction</font>

we will call the "logical structure" as "data structure"  
and "storage image of some logical structure" as "storage structure"  

## 2. Data Type

**Data Type**: a set of values which share the same properties, and operations on the value set  

**Abstract Data Type(ADT)**: the denotation of the data logical properties, with no relation to the physical implementation details. The abstract form of data structure  

*OOP can be developed form the thought of ADT*  

## 3. Analyzing the Performance of Algorithms

1. performance standard  
    1. correctness
    2. usability
    3. readability
    4. **efficiency**
    5. robustness
    6. e.t.c.
2. post test  
    easy but not very accurate  
3. pre estimation  
    confirm the relation between problem scale $n$ and it's time consumption $T(n)$ and space consumption $S(n)$  
4. asymptotic analysis(渐进分析)  

### 3.1. Previous Estimation

**Space Complexity**: the amount of memory space required by the algorithm  
    include the static and dynamic parts  

**Time Complexity**: the amount of time required by the algorithm  
    can be divided into compile time and execution time  
    the latter can be measured by *program steps*  

### 3.2. Asymptotic Analysis

when the scale $n$ is large enough,  
we call the order of magnitude of the complexity as the **asymptotic complexity** of the algorithm  

**Big O Notation**: the upper bound of the algorithm complexity

**$\Omega$ Notation**: the lower bound of the algorithm complexity

**$\Theta$ Notation**: the tight bound of the algorithm complexity

#### 3.2.1. Big O Notation

How to use?

easy to say:  
extract the part which takes the most significant when the scale $n$ is large enough  

detail description:  

if $\exist c > 0$ and $n_0 > 0$, $c, n_0 \in \mathbb{N_+}$,  
which confirm that $\forall n \geq n_0$, $T(n) \leq c \cdot f(n)$,  
then we can say that $T(n) = O(f(n))$  

*for example, $O(n^2 + n) = O(n^2)$*  

some rules:

1. addition  
    take the max one  
    *when the procedures happens serially*  

    sequence:  

    $$
    c < log_2n<n<nlog_2n<n^2<n^3<2^n<3^n<n!
    $$

2. multiplication  
    multiple the part inside the $O()$  
    *when the procedures happens nested*  
3. simplify  
    for polynomial expression, only leave the highest power term(最高次项)  

thoughts:  

1. consider the key operations in the program  
2. key operations are mostly in the loop or recursion  
3. can directly consider the execution frequency of the key operations

loop:  

1. single
2. serial  
    use addition rule
3. nested  
    use multiplication rule

as a result:  

we can break the procedure into several sub procedures  
and use the Big O Notation to respectively analyze the complexity  
then use the rules to merge the results  

#### 3.2.2. Ω and Θ Notation

we can take $\Omega$ denotation as the reverse of Big O Notation  

and if the result of the Big O Notation and $\Omega$ Notation are the same,  
then we can use $\Theta$ Notation to denote the complexity  
