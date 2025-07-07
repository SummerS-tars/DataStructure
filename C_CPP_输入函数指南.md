# C/C++ 输入函数指南

## 概述

本指南涵盖了C和C++中用于读取字符串和字符的最常用输入函数。

## C语言输入函数

### 1. `scanf()` - 格式化输入

```c
int scanf(const char *format, ...);
```

**用法：**

```c
#include <stdio.h>

char str[100];
char c;
int num;

// 读取字符串（遇到空白字符停止）
scanf("%s", str);

// 读取字符
scanf("%c", &c);

// 处理空格的读取
scanf(" %c", &c);  // 注意%c前的空格

// 读取多个项目
scanf("%d %s", &num, str);
```

**特点：**

- ✅ 格式化输入，有类型检查
- ✅ 可以一次读取多种类型
- ❌ 字符串读取时遇到空白字符就停止
- ❌ 有缓冲区溢出风险
- ❌ 会在缓冲区中留下换行符

### 2. `getchar()` - 单字符输入

```c
int getchar(void);
```

**用法：**

```c
#include <stdio.h>

char c;
c = getchar();  // 读取一个字符，包括换行符

// 读取直到换行符
while ((c = getchar()) != '\n') {
    // 处理字符
}
```

**特点：**

- ✅ 简单的单字符输入
- ✅ 文件结束时返回EOF
- ✅ 适合逐字符处理
- ❌ 会将换行符作为字符读取

### 3. `fgets()` - 安全的字符串输入

```c
char *fgets(char *str, int size, FILE *stream);
```

**用法：**

```c
#include <stdio.h>

char str[100];

// 从stdin读取（推荐）
fgets(str, sizeof(str), stdin);

// 如果存在换行符则移除
str[strcspn(str, "\n")] = '\0';

// 从文件读取
FILE *file = fopen("input.txt", "r");
fgets(str, sizeof(str), file);
```

**特点：**

- ✅ 有缓冲区溢出保护
- ✅ 读取整行，包括空格
- ✅ 包含换行符
- ✅ 安全且推荐使用
- ❌ 需要手动移除换行符

### 4. `gets()` - 不安全的字符串输入（已弃用）

```c
char *gets(char *str);  // 不要使用这个！
```

**为什么不要使用：**

- ❌ **严重安全风险** - 缓冲区溢出
- ❌ 没有边界检查
- ❌ 已从C11标准中移除

## C++输入函数

### 1. `cin` - 基本输入流

```cpp
#include <iostream>
using namespace std;

string str;
char c;
int num;

// 读取一个单词（遇到空白字符停止）
cin >> str;

// 读取一个字符
cin >> c;

// 读取多个项目
cin >> num >> str;

// 链式操作
cin >> num >> str >> c;
```

**特点：**

- ✅ 类型安全
- ✅ 运算符重载
- ✅ 链式操作
- ❌ 字符串读取时遇到空白字符就停止
- ❌ 会在缓冲区中留下换行符

### 2. `getline()` - 整行输入

```cpp
#include <iostream>
#include <string>
using namespace std;

string line;

// 读取整行
getline(cin, line);

// 使用自定义分隔符
getline(cin, line, ';');

// 从文件读取
ifstream file("input.txt");
getline(file, line);
```

**特点：**

- ✅ 读取整行，包括空格
- ✅ 自动内存管理
- ✅ 自定义分隔符
- ✅ 与string对象配合使用
- ❌ 与`cin >>`混合使用时可能出现问题

### 3. `cin.get()` - 字符输入

```cpp
#include <iostream>
using namespace std;

char c;

// 读取单个字符
c = cin.get();

// 读取到变量中
cin.get(c);

// 读取多个字符
char buffer[100];
cin.get(buffer, 100);
```

**特点：**

- ✅ 读取空白字符和换行符
- ✅ 多个重载版本
- ✅ 精确控制
- ❌ 比getline更低级

### 4. `cin.getline()` - C风格字符串输入

```cpp
#include <iostream>
using namespace std;

char str[100];

// 读取行到字符数组
cin.getline(str, 100);

// 使用自定义分隔符
cin.getline(str, 100, ';');
```

**特点：**

- ✅ 读取整行
- ✅ 缓冲区大小控制
- ✅ 自定义分隔符
- ❌ 固定缓冲区大小
- ❌ 没有自动内存管理

## 常见问题和解决方案

### 1. 缓冲区问题

**问题：** 混合使用`cin >>`和`getline()`

```cpp
int n;
string line;
cin >> n;        // 在缓冲区中留下换行符
getline(cin, line);  // 读取空行
```

**解决方案：**

```cpp
int n;
string line;
cin >> n;
cin.ignore();   // 清除缓冲区
getline(cin, line);
```

### 2. 输入验证

**C版本：**

```c
char str[100];
if (fgets(str, sizeof(str), stdin) != NULL) {
    // 移除换行符
    str[strcspn(str, "\n")] = '\0';
    // 处理str
}
```

**C++版本：**

```cpp
string line;
if (getline(cin, line)) {
    // 处理line
}
```

### 3. 读取直到EOF

**C版本：**

```c
char c;
while ((c = getchar()) != EOF) {
    // 处理字符
}
```

**C++版本：**

```cpp
string line;
while (getline(cin, line)) {
    // 处理line
}
```

## 最佳实践

### 对于C

1. **使用`fgets()`而不是`scanf()`读取字符串**
2. **始终检查缓冲区边界**
3. **显式处理换行符**
4. **检查返回值进行错误处理**

### 对于C++

1. **使用`getline()`读取整行**
2. **使用`cin >>`读取单词/数字**
3. **使用`cin.ignore()`清除缓冲区**
4. **优先使用`string`而不是`char[]`**

## 快速参考表

| 函数 | 语言 | 安全性 | 空白字符 | 换行符 | 最适合 |
|------|------|--------|----------|---------|--------|
| `scanf()` | C | ❌ | 停止 | 留下 | 格式化输入 |
| `fgets()` | C | ✅ | 包含 | 包含 | 安全字符串输入 |
| `getchar()` | C | ✅ | 读取 | 读取 | 单字符 |
| `cin >>` | C++ | ✅ | 停止 | 留下 | 单词/数字 |
| `getline()` | C++ | ✅ | 包含 | 消耗 | 整行 |
| `cin.get()` | C++ | ✅ | 读取 | 读取 | 字符控制 |

## 示例程序

### C示例

```c
#include <stdio.h>
#include <string.h>

int main() {
    char name[50];
    int age;
    
    printf("请输入您的姓名：");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = '\0';  // 移除换行符
    
    printf("请输入您的年龄：");
    scanf("%d", &age);
    
    printf("您好%s，您今年%d岁。\n", name, age);
    return 0;
}
```

### C++示例

```cpp
#include <iostream>
#include <string>
using namespace std;

int main() {
    string name;
    int age;
    
    cout << "请输入您的姓名：";
    getline(cin, name);
    
    cout << "请输入您的年龄：";
    cin >> age;
    
    cout << "您好" << name << "，您今年" << age << "岁。" << endl;
    return 0;
}
```

## 内存管理注意事项

- **C语言**：手动内存管理，固定缓冲区大小
- **C++**：使用`string`自动内存管理
- **在C中始终验证输入边界**
- **在C++中尽可能使用`string`而不是`char[]`**

## 结论

根据需要选择合适的函数：

- **简单格式化输入**：`scanf()`（C）或`cin >>`（C++）
- **安全字符串输入**：`fgets()`（C）或`getline()`（C++）
- **逐字符处理**：`getchar()`（C）或`cin.get()`（C++）
- **混合输入**：结合使用函数，注意清理缓冲区

## 常见陷阱

### 1. 缓冲区溢出

```c
// 危险
char str[10];
scanf("%s", str);  // 用户输入超过10个字符会溢出

// 安全
char str[10];
fgets(str, sizeof(str), stdin);
```

### 2. 换行符处理

```c
// 问题
scanf("%d", &num);
getchar();  // 读取换行符

// 更好的方法
scanf("%d\n", &num);  // 在格式字符串中包含换行符
```

### 3. 混合输入类型

```cpp
// 问题
int n;
string line;
cin >> n;
getline(cin, line);  // 读取空行

// 解决方案
int n;
string line;
cin >> n;
cin.ignore(1000, '\n');  // 忽略到换行符
getline(cin, line);
```

## 调试技巧

1. **打印输入内容**：验证实际读取的内容
2. **检查缓冲区状态**：使用`cin.peek()`查看下一个字符
3. **使用调试器**：逐步检查输入过程
4. **测试边界情况**：空输入、很长的输入等
