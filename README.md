# 程序设计作业 6 折线标注

```
Polylines:\
│  Point.cpp
│  Point.h
│  Polyline.cpp
│  Polyline.h
│  util.cpp
│  util.h
│  fun.cpp
│  fun.h
│  main.cpp
│  README.md
```

文件介绍
---------------------------
1. `Point.cpp`, `Point.h` 点类及相关函数实现
2. `Polyline.cpp`, `Polyline.h` 折线类及相关函数实现
3. `util.cpp`, `util.h` 官方函数库, 及通用的小工具
4. `fun.cpp`, `fun.h` 本次项目主要函数
5. `main.cpp` 入口

特点介绍
----------------------------
1. 通过对当前目录下所有文件检索, 匹配所有`*.txt`数据文件(不包含`^Result_[\s\S]*.txt$`, 这些被识别为结果文件)
2. 每个文件运算完成后, 向`Result\`目录下存储同名结果文件
3. 每个文件计算完成后可以选择展示gui绘图, 因为初次上手窗口应用有些生疏, 有改进空间
4. 完成了对点的标注, 实现了标注不重叠, 算法有待完善.