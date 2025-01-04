# MatrixKill

MatrixKill 是一个高效的线性方程组求解器，支持多种迭代法求解大规模稀疏线性方程组。

## 功能特点

- 支持多种求解方法：
  - Jacobi 迭代法
  - Gauss-Seidel 迭代法
  - SOR (Successive Over-Relaxation) 迭代法
- 自动检测矩阵特性：
  - 对角占优性检查
  - 矩阵维度验证
  - 零矩阵检测
- 灵活的配置系统：
  - 支持 INI 格式的配置文件
  - 命令行参数覆盖配置
- 详细的结果输出：
  - 求解时间统计
  - 迭代次数记录
  - 残差计算
  - 解向量可视化

## 编译要求

- CMake 3.10 或更高版本
- 支持 C++14 的编译器
- 建议使用：
  - GCC 5.0+
  - Clang 3.4+
  - MSVC 2017+

## 快速开始

1. 克隆仓库：

```bash
git clone https://github.com/little_penguin66/MatrixKill.git
cd MatrixKill
```

2. 编译项目：

```bash
mkdir build
cd build
cmake ..
make
```

3. 运行程序：

```bash
./MatrixKill config.ini
```

## 使用方法

### 基本用法

```bash
./MatrixKill <配置文件路径> [选项]
```

### 命令行选项

- `-h, --help` 显示帮助信息
- `-o, --output <文件路径>` 指定输出文件路径 (默认: result.txt)
- `-s, --solver <求解器>` 指定求解器类型 (jacobi/gauss/sor)
- `-t, --tolerance <精度>` 设置求解精度
- `-m, --max-iter <次数>` 设置最大迭代次数
- `-w, --omega <系数>` 设置 SOR 松弛因子 (默认: 1.5)
- `-q, --quiet` 安静模式
- `-v, --verbose` 详细模式

### 配置文件格式

```ini
[Solver]
type = sor          # 求解器类型：jacobi, gauss, sor
tolerance = 1e-6    # 收敛精度
max_iterations = 1000

[Matrix]
size = 4           # 矩阵维度
# 系数矩阵 A (按行排列，分号分隔行，逗号分隔列)
A = 7.61,-0.53,-0.29,-0.44;-0.53,7.82,-0.54,-0.68;-0.29,-0.54,8.45,-1.43;-0.44,-0.68,-1.43,7.68
# 常数向量 b
b = 1,2,3,4
```

### 生成测试矩阵

项目提供了一个 Python 脚本用于生成测试矩阵：

```bash
python generateMatrix.py -s 100 -o config.ini
```

参数说明：

- `-s, --size`：矩阵大小
- `-d, --diagonal`：对角线元素值
- `-o, --output`：输出文件路径
- `-r, --result`：结果分析文件路径
- `--no-visual`：不显示可视化输出

## 输出示例

```
求解方法: sor
矩阵规模: 4
收敛精度: 1e-6
最大迭代次数: 1000
实际迭代次数: 15
计算时间: 0.123ms

系数矩阵 A:
    7.610000   -0.530000   -0.290000   -0.440000
   -0.530000    7.820000   -0.540000   -0.680000
   -0.290000   -0.540000    8.450000   -1.430000
   -0.440000   -0.680000   -1.430000    7.680000

解向量 x:
    0.154321    0.298765    0.432099    0.567901

残差向量 (Ax-b):
    0.000000    0.000000    0.000000    0.000000
```

## 许可证

本项目采用 MIT 许可证。详见 [LICENSE](LICENSE) 文件。

## 贡献

欢迎提交 Issue 和 Pull Request！
