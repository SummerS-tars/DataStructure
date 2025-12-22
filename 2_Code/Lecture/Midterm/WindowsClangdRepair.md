# Windows平台使用Clangd异常修复

依然还是没办法找到头文件的问题  
通过为clangd添加arguments解决  

```json
"clangd.arguments": [
    "-std=c++23",
    "-I\"E:/_ComputerLearning/_Environment/w64devkit/lib/gcc/x86_64-w64-mingw32/14.2.0/include\"",
]
```
