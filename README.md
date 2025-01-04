# MatrixKill 🧮

一个高效的线性方程组求解器 📐，支持多种迭代法求解大规模稀疏线性方程组。

## ✨ 功能特点

### 📊 求解方法
- Jacobi 迭代法 ⚡️
- Gauss-Seidel 迭代法 🔄
- SOR (Successive Over-Relaxation) 迭代法 🚀

### 🔍 矩阵分析
- 对角占优性检查 ✅
- 矩阵维度验证 📏
- 零矩阵检测 🎯

### ⚙️ 系统配置
- 支持 INI 格式配置文件 📝
- 命令行参数覆盖配置 🎮

### 📈 结果输出
- 求解时间统计 ⏱️
- 迭代次数记录 🔢
- 残差计算 📉
- 解向量可视化 📊

## 🛠️ 编译要求

- CMake 3.10 或更高版本 🏗️
- 支持 C++14 的编译器 💻
- 建议使用：
  - GCC 5.0+ 🚀
  - Clang 3.4+ 🌟
  - MSVC 2017+ ⭐

## 🚀 快速开始

1. 克隆仓库 📥
```bash
git clone https://github.com/little_penguin66/MatrixKill.git
cd MatrixKill
```

2. 编译项目 🔨
```bash
mkdir build && cd build
cmake ..
make
```

3. 运行示例 🎯
```bash
./MatrixKill ../examples/config.ini
```

## 📖 使用说明

```bash
./MatrixKill <配置文件> [选项]
```

### 配置示例 📝
```ini
[Solver]
type = sor
tolerance = 1e-6
max_iterations = 1000

[Matrix]
size = 4
A = 4,-1,0,0; -1,4,-1,0; 0,-1,4,-1; 0,0,-1,4
b = 1,2,3,4
```

## 📄 许可证

本项目采用 MIT License 开源协议。

## 🤝 贡献

欢迎提交 Issue 和 Pull Request！