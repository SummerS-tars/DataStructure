# Lab 12

## 1. 第一题：最短路径

### 1.1. 题目描述

- 一个 n 个节点的无向带权图，节点编号为 0 到 n - 1 。图中总共有 m 条边，用二维数组 edges 表示，其中 edges[i] = [ai, bi, wi] 表示节点 ai 和 bi 之间有一条边权为 wi 的边。
- 对于节点 0 为出发点，节点 n - 1 为结束点的所有最短路，你需要返回一个长度为 m 的 boolean 数组 answer 。如果 edges[i] 至少 在其中一条最短路上，那么 answer[i] 为 true ，否则 answer[i] 为 false 。

### 1.2. 示例

Example1：  
输入：n = 6, edges = [[0,1,4],[0,2,1],[1,3,2],[1,4,3],[1,5,1],[2,3,1],[3,5,3],[4,5,2]]  
输出：[true,true,true,false,true,true,true,false]  
解释：以下为节点 0 出发到达节点 5 的 所有 最短路：  
路径 0 -> 1 -> 5 ：边权和为 4 + 1 = 5 。  
路径 0 -> 2 -> 3 -> 5 ：边权和为 1 + 1 + 3 = 5 。  
路径 0 -> 2 -> 3 -> 1 -> 5 ：边权和为 1 + 1 + 2 + 1 = 5 。  

Example2：  
输入：n = 4, edges = [[2,0,1],[0,1,1],[0,3,4],[3,2,2]]  
输出：[true,false,false,true]  

Example3：  
输入：n = 3, edges = [[2,1,6]]  
输出：[false]  

## 2. 第二题：课程排序

### 2.1. 题目描述

- 给定 numCourses 门课程，记为0到numCourses-1。给你一个数组 prerequisites，其中 prerequisites[i]= [ai,bi]，表示在选修课程 ai 前 必须 先选修 bi 。
- 例如，想要学习课程0，你需要先完成课程1，我们用[0,1]来表示。
- 返回为了学完所有课程所安排的学习顺序。可能会有多个正确的顺序，你只要返回意一种 就可以了。如果不可能完成所有课程，返回一个空数组。

### 2.2. 示例

Example1：  
输入：numCourses = 2, prerequisites = [[1,0]]  
输出：[0,1]  

Example2:  
输入：numCourses = 4, prerequisites = [[1,0],[2,0],[3,1],[3,2]]  
输出：[0,1,2,3] 或 [0,2,1,3] 均可  

Example3:  
输入：numCourses = 3, prerequisites = [[1,0],[2,0],[0,2]]  
输出：[]  
