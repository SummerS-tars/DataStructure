# Tree

## 1. Basic Concepts

- degree: 度，对结点而言的，他的子结点（子树）的个数  
- leaf: 度为0的结点，叶结点  
- child: 子结点  
- parent: 父结点  
- sibling: 同父结点的结点  
- ancestor: 祖先结点，根结点到某结点x上路径中所有结点（不包括x）
- descendant: 子孙结点，某结点x的子结点以及他们的子结点
- level: 层次，根节点定为1，父节点层次加1为子节点层次  
- depth: 最大层次
- height: 同depth  
- degree(of tree): 树的度，树中所有结点的最大度  
- ordered tree: 有序树，表现为结点的子树有顺序之分  
- unordered tree: 无序树，结点的子树无顺序之分  
- forest: 有限棵树的集合  

## 2. Storage Structure of Tree

### 2.1. Parent Pointer Representation(父指针表示法)

node domains:  

- data
- parent: pointer to parent node  

### 2.2. Child Linked List Representation(子女链表示法)

nodes are in an array  

node domains:  

- data
- firstChild: pointer to first child node  

every node can have a linked list of its children  

### 2.3. Child-Sibling Representation(孩子-兄弟表示法)

also called left-child right-sibling representation(左子女-右兄弟表示法)  

node domains:  

- data
- firstChild: pointer to first child node
- rightSibling: pointer to right sibling node

in this way, a tree can be projected to a binary tree  

### 2.4. Other Conceptions

all in all  
what we mentioned above are all  
linked list based representations of tree structure  

there are also array based representations of tree structure  
they have their own advantages and disadvantages  

## 3. Tree Traversal

we need some methods to visit every node once and only once  
here are some common methods:  

- DFS: depth first search  
    - pre-order traversal(先序遍历)  
    - in-order traversal(中序遍历)  
    - post-order traversal(后序遍历)
- BFS: breadth first search  

### 3.1. Pre-Order Traversal

Process:  

1. visit the root node
2. traverse the left subtree  
3. traverse the right subtree  

usually implemented by recursion  

#### 3.1.1. Implementation 1

recursive implementation  

```cpp
template <class T> void Tree<T>::
preOrder(TreeNode<T>* node) {
    if(node != nullptr) {
        visit(node);
        for(node = node->firstChild; node != nullptr; node = node->nextSibling) {
            preOrder(node);
        }
    }
}
```

work for child-sibling representation  
binary tree is a special case of child-sibling tree  

#### 3.1.2. Implementation 2

non-recursive implementation  

```cpp
template <class Type> void Tree <Type>::
PreOrder(TreeNode <Type>* node) {
    std::stack<TreeNode <Type>*> st(DefaultSize);
    do {
        while(node != nullptr) {
            visit(node);
            st.push(node);
            node = node->firstChild;
        }
        while(node == nullptr && !st.isEmpty()) {
            st.pop(&node);
            node = node->nextSibling;
        }
    } while(node != nullptr);
}
```

we can understand that recursive function calls are just the usage of stack structure  
and here we use manually implemented stack to simulate the recursive calls  

### 3.2. Post-Order Traversal

Process:  

1. traverse the left subtree
2. traverse the right subtree
3. visit the root node

#### 3.2.1. Implementation 1

```cpp
template <class T> void Tree<T>::
postOrder(TreeNode<T> *node) {
    if(node != nullptr) {
        TreeNode<T> *child;
        for(child = node->firstChild; child != nullptr; child = child->nextSibling) {
            postOrder(child);
        }
        visit(node);
    }
}
```

we can compare it with pre-order traversal implementation  
the only difference is the position of `visit(node)`  
here we visit the node when we are back  
while in pre-order traversal
we visit the node when we get deep into the tree  

### 3.3. Implementation 2

```cpp
template <class T> void Tree<T>::
postOrder(TreeNode<T> *node) {
    std::stack<TreeNode<T>*> st(DefaultSize);
    do{
        while(node != nullptr){
            st.push(node);
            node = node->firstChild;
        }
        while(node == nullptr && !st.isEmpty()){
            st.pop(&node);
            visit(node);
            node = node->nextSibling;
        }
    } while(node != nullptr);
}
```

## 4. Level-Order Traversal

BFS  

### 4.1. Implementation

```cpp
template <class T> void Tree<T>::
levelOrder(TreeNode<T>* node) {
    std::queue<TreeNode<T>*> q(DefaultSize);
    if(node != nullptr) {
        q.enqueue(node);
        while(!q.isEmpty()) {
            q.dequeue(&node);
            visit(node);
            for(TreeNode<T>* child = node->firstChild; child != nullptr; child = child->nextSibling) {
                q.enqueue(child);
            }
        }
    }
}
```

## 5. Binary Tree

properties:  

- degree: <=2  
- order: ordered  

special kinds of binary tree:  

- full binary tree(满二叉树)  
    all leaf nodes are at the same level  
    and except leaf nodes, all nodes have degree 2  
    *a special case of complete binary tree*  
- complete binary tree(完全二叉树)  
    only the deepest level can be incomplete  
    *(leaf nodes can only exist at the 1st and 2nd deepest levels)*  
- binary search tree(二叉搜索树)  
    for every node, left subtree nodes' values < node's value < right subtree nodes' values  

### 5.1. Properties of Binary Tree

mathematical properties:  

- number of nodes for i level:  
    $N \le 2^{i-1}$  
- total number of nodes for k-level tree
    $N \le 2^k - 1$  
- degree and leaf nodes: for any binary tree, $n_0$ number of leaf nodes, $n_2$ number of degree 2 nodes
    $n_0 = n_2 + 1$  
    besides, for complete binary tree  
    $n_1 = 0 \text{ or } 1$  
- depth of complete binary tree with $N$ nodes  
    $k = \lfloor log_2N \rfloor + 1$

### 5.2. Storage Structure of Binary Tree

linked list based representation:  
actually we have mentioned  
three domains  

- data
- leftChild: pointer to left child node
- rightChild: pointer to right child node  

but for complete binary tree  
a special way **array based representation** is more efficient  
*(actually all binary trees can be represented in this way, but complete binary tree is more suitable for less space waste)*  

in this representation:  

- root node is stored at index 1  
- for node at index $i$ :  
    - left child is at index $2i$  
    - right child is at index $2i + 1$
    - parent node is at index $\lfloor i/2 \rfloor$

### 5.3. Binary Tree Traversal

- Pre-order Traversal
- In-order Traversal(中序遍历)  
- Post-order Traversal(后序遍历)

all the traversal algorithms have the same time complexity: $O(n)$  

#### 5.3.1. Pre-order Traversal

```cpp
template <class T> void BinaryTree<T>::
preOrder(BinaryTreeNode<T> *node){
    if(node != nullptr){
        visit(node);
        preOrder(node->leftChild);
        preOrder(node->rightChild);
    }
}
```

#### 5.3.2. In-order Traversal

process:  

1. traverse the left subtree
2. visit the root node
3. traverse the right subtree

```cpp
template <class T> void BinaryTree<T>::
inOrder(BinaryTreeNode<T> *node){
    if(node != nullptr){
        inOrder(node->leftChild);
        visit(node);
        inOrder(node->rightChild);
    }
}
```

#### 5.3.3. Post-order Traversal

```cpp
template <class T> void BinaryTree<T>::
postOrder(BinaryTreeNode<T> *node){
    if(node != nullptr){
        postOrder(node->leftChild);
        postOrder(node->rightChild);
        visit(node);
    }
}
```

#### 5.3.4. Traversal Usage

- calculate the nodes' number  
- calculate the height of tree  
- copy the tree  
- delete the tree
- judge the equality of two trees  
- create binary tree  
- use two kinds of traversal results to reconstruct the binary tree  
    pre-order + in-order = only one  
    post-order + in-order = only one  
    pre-order + post-order = may be more than one  

    the first one in the pre-order result is the root node  
    and the previous nodes in the in-order result are in the left subtree  
    the later nodes in the in-order result are in the right subtree  
    we can use recursion to construct the tree  

#### 5.3.5. Usage Examples

1. print all ancestors of a given node data(the number of the nodes with the value is no more than 1)  

    ```cpp
    template <class T> bool BinaryTree<T>::
    printAncestors(BinaryTreeNode<T> *node, T target) {
        if(node == nullptr) {
            return false;
        }
        if(node->data == target) {
            return true;
        }
        if(printAncestors(node->leftChild, target) || printAncestors(node->rightChild, target)) {
            visit(node);
            return true;
        }
        return false;
    }
    ```

    ```cpp
    template <class T> void BinaryTree<T>::
    printAncestors(T target) {
        std::stack<BinaryTreeNode<T>*> st(DefaultSize);
        BinaryTreeNode<T>* node = root;
        bool found = false;
        do{
            //* 正常的后序遍历过程
            while(node != nullptr){
                st.push(node);
                if(node->data == target){
                    found = true;
                    st.pop();  //* 弹出目标节点，不将其作为祖先节点输出
                    break;
                }
                node = node->leftChild;
            }
            while(node == nullptr && !st.isEmpty()){
                st.pop(&node);
                node = node->rightChild;
            }

            //* 找到目标节点后，栈中存储的即为其祖先节点
            if(found){
                while(!st.isEmpty()){
                    st.pop(&node);
                    visit(node);
                }
            }
        } while(!st.isEmpty());
    }
    ```

## 6. Count of Binary Trees(二叉树的计数)

## Threaded Binary Tree(线索化二叉树)

this kind of binary tree can make full use of null pointers in binary tree nodes  
to denote the predecessor and successor nodes(前驱和后继结点) in certain traversal order  
*(that is to say, there are three types of threaded binary trees corresponded to different traversal orders: pre-order, in-order, and post-order)*  

## Heap

A special kind of complete binary tree  
whose every node's value is greater(or less) than its children's values  
called max-heap(min-heap) respectively  

### 7.1. Storage Structure of Heap

use array based representation of complete binary tree  
*this implementation can realize the $O(1)$ location of parent and child nodes*  
it is usually started with index 1  

### Manipulations of Heap

main operations:  
*other operations are mostly based on these two*  

- sift-up(上滤)
- sift-down(下滤)

important operations:  

- insert an element  
    append the element at the end of the array  
    then keep sift-up the element to its proper position  
- delete element  
    usually delete the root node element  
    replace the root node element with the last element in the array  
    remove the last element  
    then keep sift-down the new root node element to its proper position  
- build heap from an unordered array  
    starting from the last non-leaf node  
    keep sift-down each non-leaf node to its proper position