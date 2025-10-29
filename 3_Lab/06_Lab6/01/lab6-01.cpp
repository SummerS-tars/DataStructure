#include <iostream>
#include <vector>
#include <queue>
#include <assert.h>

using namespace std;

// ============================================================
// NestedInteger 类说明
// 
// 这个类用来表示"广义表"中的元素
// 每个元素可以是：
//   1. 一个整数（is_integer=true）
//   2. 一个列表（is_integer=false，包含其他NestedInteger元素）
//
// 示例：[1,[4,[6]]]
//   - 最外层是一个列表
//   - 包含两个元素：1（整数）和[4,[6]]（列表）
//   - [4,[6]]又包含两个元素：4（整数）和[6]（列表）
//   - [6]包含一个元素：6（整数）
// ============================================================

class NestedInteger
{
public:
    // 构造函数1：创建空列表
    // is_integer 初始化为 false，表示这是一个列表
    // list_value 为空向量，可以通过 add() 方法添加元素
    NestedInteger() : is_integer(false) {}
    
    // 构造函数2：创建整数元素
    // 参数 value：要存储的整数值
    // is_integer 初始化为 true，表示这是一个整数元素
    // integer_value 存储传入的整数值
    NestedInteger(int value) : is_integer(true), integer_value(value) {}

    // 接口1：isInteger()
    // 作用：判断当前元素的类型
    // 返回值：
    //   - true  表示当前元素是整数
    //   - false 表示当前元素是列表
    // 用途：决定如何处理当前元素
    //   if (element.isInteger()) {
    //       int val = element.getInteger();  // 安全地获取整数值
    //   } else {
    //       auto& list = element.getList();  // 安全地获取列表
    //   }
    bool isInteger() const
    {
        return is_integer;
    }

    // 接口2：getInteger()
    // 作用：获取当前元素的整数值
    // 前提条件：必须确保 isInteger() 返回 true
    //          如果元素是列表调用此方法会得到无意义的数值
    // 返回值：存储在 integer_value 中的整数
    // 用法示例：
    //   if (ni.isInteger()) {
    //       int num = ni.getInteger();  // 安全地获取整数
    //   }
    int getInteger() const
    {
        return integer_value;
    }

    // 接口3：getList()
    // 作用：获取当前元素包含的子元素列表
    // 前提条件：必须确保 isInteger() 返回 false
    //          如果元素是整数调用此方法会得到空列表
    // 返回值：引用到 list_value 向量的常量引用
    //        返回引用避免复制整个向量，提高效率
    //        返回常量引用保证不会修改原列表内容
    // 用法示例：
    //   if (!ni.isInteger()) {
    //       const auto& children = ni.getList();
    //       for (const auto& child : children) {
    //           // 处理每个子元素
    //       }
    //   }
    const std::vector<NestedInteger> &getList() const
    {
        return list_value;
    }

    // 接口4：add()
    // 作用：向列表中添加一个子元素
    // 参数 ni：要添加的 NestedInteger 元素（可以是整数或列表）
    // 机制：使用 vector 的 push_back() 将元素添加到列表末尾
    // 用法示例：
    //   NestedInteger list;           // 创建空列表
    //   list.add(NestedInteger(1));   // 添加整数 1
    //   list.add(NestedInteger(2));   // 添加整数 2
    //   // 现在 list 表示 [1,2]
    void add(const NestedInteger &ni)
    {
        list_value.push_back(ni);
    }

private:
    // 私有成员变量
    
    // is_integer：标志位，记录当前元素的类型
    //   true  → 当前元素是整数，integer_value 有效，list_value 为空
    //   false → 当前元素是列表，list_value 有效，integer_value 无意义
    bool is_integer;
    
    // integer_value：存储整数值
    //   仅在 is_integer=true 时有效
    //   如果 is_integer=false，此变量未使用
    int integer_value;
    
    // list_value：存储列表中的所有子元素
    //   仅在 is_integer=false 时有效
    //   每个元素本身也可以是整数或列表（递归结构）
    //   如果 is_integer=true，此向量为空
    std::vector<NestedInteger> list_value;
};

NestedInteger parseNestedInteger(const std::string &s, int &index)
{
    NestedInteger result;
    if (s[index] == '[')
    {
        ++index; // skip the '['
        while (s[index] != ']')
        {
            result.add(parseNestedInteger(s, index));
            if (s[index] == ',')
                ++index; // skip the ','
        }
        ++index; // skip the ']'
    }
    else
    {
        int start = index;
        while (index < s.size() && (isdigit(s[index]) || s[index] == '-'))
        {
            ++index;
        }
        int number = std::stoi(s.substr(start, index - start));
        result = NestedInteger(number);
    }
    return result;
}

std::vector<NestedInteger> buildNestedList(const std::string &input)
{
    std::vector<NestedInteger> nestedList;
    int index = 0;
    nestedList.push_back(parseNestedInteger(input, index));
    return nestedList;
}

// !
// 辅助函数：计算嵌套列表的最大深度
// 使用迭代的方式（BFS）遍历整个列表树
int getMaxDepth(vector<NestedInteger> &nestedList)
{
    // 队列存储：<NestedInteger指针, 当前深度>
    // 使用指针以避免复制对象
    queue<pair<const NestedInteger *, int>> q;
    
    // 初始化：将所有顶层元素加入队列，深度为1
    for (const auto &item : nestedList)
    {
        q.push(make_pair(&item, 1));
    }
    
    int maxDepth = 0;
    
    // BFS遍历整个嵌套结构
    while (!q.empty())
    {
        const NestedInteger *current = q.front().first;
        int depth = q.front().second;
        q.pop();
        
        // 更新最大深度
        maxDepth = max(maxDepth, depth);
        
        // 如果当前元素是列表，将其所有子元素加入队列
        if (!current->isInteger())
        {
            const auto &list = current->getList();
            for (const auto &item : list)
            {
                q.push(make_pair(&item, depth + 1));
            }
        }
    }
    
    return maxDepth;
}

// !
// 主函数：计算深度加权和（自下而上）
// 输入：嵌套整数列表
// 输出：所有整数由其深度权重加权后的总和
// 例如 [1,[4,[6]]]：6*1 + 4*2 + 1*3 = 17
int depthSumInverse(vector<NestedInteger> &nestedList)
{
    // 第一步：计算整个嵌套结构的最大深度
    int maxDepth = getMaxDepth(nestedList);
    
    // 第二步：使用BFS进行加权求和
    // 队列存储：<NestedInteger指针, 当前深度>
    queue<pair<const NestedInteger *, int>> q;
    
    // 初始化：将所有顶层元素加入队列，深度为1
    for (const auto &item : nestedList)
    {
        q.push(make_pair(&item, 1));
    }
    
    int result = 0;
    
    // BFS遍历整个嵌套结构
    while (!q.empty())
    {
        const NestedInteger *current = q.front().first;
        int depth = q.front().second;
        q.pop();
        
        // 如果当前元素是整数
        if (current->isInteger())
        {
            // 计算深度权重：最大深度 - 当前深度 + 1
            // 这样最深层的元素权重为1，最浅层的权重最大
            int weight = maxDepth - depth + 1;
            result += current->getInteger() * weight;
        }
        else
        {
            // 如果当前元素是列表，将其所有子元素加入队列
            const auto &list = current->getList();
            for (const auto &item : list)
            {
                q.push(make_pair(&item, depth + 1));
            }
        }
    }
    
    return result;
}

int main()
{
    // ========== 测试用例1 ==========
    // 输入: [1,[4,[6]]]
    // 解析过程：
    //   1 在深度1（权重 = 3-1+1 = 3）
    //   4 在深度2（权重 = 3-2+1 = 2）
    //   6 在深度3（权重 = 3-3+1 = 1）
    // 预期输出: 1*3 + 4*2 + 6*1 = 3 + 8 + 6 = 17
    std::string input1 = "[1,[4,[6]]]";
    std::vector<NestedInteger> nestedList1 = buildNestedList(input1);
    int result1 = depthSumInverse(nestedList1);
    std::cout << "测试用例1: " << input1 << std::endl;
    std::cout << "预期输出: 17, 实际输出: " << result1 << std::endl;
    std::cout << (result1 == 17 ? "✓ 通过" : "✗ 失败") << std::endl << std::endl;

    // ========== 测试用例2 ==========
    // 输入: [[1,1],2,[1,1]]
    // 解析过程：
    //   最大深度为2
    //   [1,1] 在深度1，其内部的1在深度2（权重 = 1）
    //   2 在深度1（权重 = 2）
    //   [1,1] 在深度1，其内部的1在深度2（权重 = 1）
    // 预期输出: 1*1 + 1*1 + 2*2 + 1*1 + 1*1 = 1 + 1 + 4 + 1 + 1 = 8
    std::string input2 = "[[1,1],2,[1,1]]";
    std::vector<NestedInteger> nestedList2 = buildNestedList(input2);
    int result2 = depthSumInverse(nestedList2);
    std::cout << "测试用例2: " << input2 << std::endl;
    std::cout << "预期输出: 8, 实际输出: " << result2 << std::endl;
    std::cout << (result2 == 8 ? "✓ 通过" : "✗ 失败") << std::endl << std::endl;

    // ========== 测试用例3 ==========
    // 输入: [1,2,3]
    // 解析过程：
    //   最大深度为1
    //   1、2、3 都在深度1（权重 = 1-1+1 = 1）
    // 预期输出: 1*1 + 2*1 + 3*1 = 6
    std::string input3 = "[1,2,3]";
    std::vector<NestedInteger> nestedList3 = buildNestedList(input3);
    int result3 = depthSumInverse(nestedList3);
    std::cout << "测试用例3: " << input3 << std::endl;
    std::cout << "预期输出: 6, 实际输出: " << result3 << std::endl;
    std::cout << (result3 == 6 ? "✓ 通过" : "✗ 失败") << std::endl << std::endl;

    return 0;
}