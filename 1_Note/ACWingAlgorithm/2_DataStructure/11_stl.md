# STL

本篇将介绍一些STL容器和算法  

1. vector
2. string
3. queue, priority_queue
4. stack
5. deuqe
6. set, map, multiset, multimap
7. unordered_set, unordered_map, unordered_multiset, unordered_multimap
8. bitset

## 1. vector

直译过来是向量  
实际为变长数组  
即长度可以动态变化的数组  

基本思想：  
倍增  

### 1.1. 函数

1. size
2. empty
3. clear
4. front 返回第一个元素
5. back 返回最后一个元素
6. push_back 在末尾插入一个元素
7. pop_back 删除最后一个元素
8. begin 返回指向第一个元素的迭代器
9. end 返回指向最后一个元素的下一个位置的迭代器

### 1.2. 运用

数组下标遍历  
略  

迭代器遍历  

```cpp
for(vector<int>::iterator i = a.begin(); i != a.end(); i ++){
    printf("%d ", *i);
}
```

*迭代器可以看作指针*  

范围遍历  

```cpp
for(auto x : a) cout << x << ' ';
cout << endl
```

### 1.3. 其他

vector之间是可以比较的  
按照字典序比较  

## 2. pair

pair也很常用  
用于存储一个二元组  
前后两个变量类型任意  

### 2.1. 运用

1. first 返回第一个元素
2. second 返回第二个元素

### 2.2. 其他

pair也是支持比较运算的  
也可以看成是字典序比较  

### 2.3. 实际运用

创建（初始化）：  

```cpp
pair<int, int> p1, p2;
p1 = make_pair(1, 2);
p2 = {3, 4};
```

访问：  

```cpp
cout << p1.first << ' ' << p1.second << endl;
```

比较：  

```cpp
if(p1 < p2) cout << "p1 < p2" << endl;
```

## 3. string

字符串  

### 3.1. 函数

1. size/length 返回字符串长度
2. empty
3. clear
4. substr 返回子串  
    参数为起始位置和长度  
    起始位置从0开始  
5. c_str 返回字符串的C风格字符串  
    返回值为const char*  
    可以用于与printf函数配合使用  
    （即此时可以当作是C中的char数组字符串来使用）  

### 3.2. 性质

可以直接加上一个字符串或字符  

## 4. queue

队列  

1. push 在队尾插入一个元素
2. front 返回队头元素
3. pop 弹出队头元素
4. size

注意没有clear函数  
想要清空队列，可以直接使用`q = queue<int>();`  
创建一个空的新队列，用于替代  

*priority_queue和stack也没有clear函数*  

## 5. priority_queue

优先队列  
是一个大根堆  

1. push
2. top
3. pop

如果要作为小根堆使用，可以插入和使用时全部取反  

还可以：  

```cpp
priority_queue<int, vector<int>, greater<int>> q;
```

表示创建一个优先队列，使用vector作为底层容器，使用greater作为比较函数  

## 6. stack

栈

1. push
2. top
3. pop

## 7. deque

双端队列  
队头队尾都可以插入和删除  
且支持随机访问  
可以看作加强版的vector  

### 7.1. 函数

1. size
2. empty
3. clear
4. front
5. back
6. push_back
7. pop_back
8. push_front
9. pop_front

### 7.2. 性质

还支持随机访问  

不过速度较慢  

## 8. set, map, multiset, multimap

基于平衡二叉树（红黑树）  
动态维护有序序列  

均支持操作：  

1. size
2. empty
3. clear
4. begin
5. end

### set, multiset

set是集合，multiset是多重集合  
set中每个元素都是唯一的，multiset中可以有重复元素  

set/multiset操作：  

1. insert 插入一个数
2. find 查找一个数
3. count 返回一个数的个数
4. erase  
    1. 输入为数，删除所有该数  
        $O(k + logn)$
    2. 输入为迭代器，删除该迭代器指向的元素
5. lower_bound/upper_bound 返回大于等于/大于一个数的最小位置的迭代器  
    set中最核心的两个函数  
    注意不要因名混淆作用  
    1. lower_bound(x) 返回大于等于某一个数的最小的数  
    2. upper_bound(x) 返回大于某一个数的最小的数  
    3. 返回值为迭代器  

### map, multimap

1. insert  
    注意插入的是一个pair  
2. erase  
    可以是一个pair，也可以是一个迭代器  
3. find
4. lower_bound/upper_bound

基本所有的操作时间复杂度都是$O(logn)$  

#### 操作方法

我们完全可以用类似于C中数组的方式来操作map  

```cpp
map<string, int> a;
a["abc"] = 1;
cout << a["abc"] << endl;
```

不过注意，时间复杂度是$O(logn)$  
*数组是$O(1)$*  

## 9. unordered_set, unordered_map, unordered_multiset, unordered_multimap

基于哈希表  

优点：增删改查的时间复杂度都是 $O(1)$  

缺点：不支持lower_bound/upper_bound操作  
因为其内部不是有序的  
也不支持迭代器的自增自减操作  
*即所有的与排序有关的函数都不能使用*  

## 10. bitset

压位  
