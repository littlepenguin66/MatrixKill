import argparse
import sys
import random

def generate_matrix(size, diagonal_value=None):
    """生成对角占优矩阵
    
    Args:
        size: 矩阵大小
        diagonal_value: 对角线元素值，如果不指定则自动计算一个合适的值
    """
    # 如果没有指定对角线元素值，则设为8(保证对角占优)
    if diagonal_value is None:
        diagonal_value = 8  # 由于每行最多有8个非零元素，所以对角线元素为8就能保证对角占优

    matrix = []
    for i in range(size):
        row = []
        for j in range(size):
            if i == j:
                # 对角线元素在[diagonal_value-0.5, diagonal_value+0.5]范围内随机
                row.append(round(random.uniform(diagonal_value-0.5, diagonal_value+0.5), 2))
            else:
                # 计算与对角线的距离
                dist = abs(i - j)
                if dist <= 5:  # 在对角线周围5条带内生成非零元素
                    # 元素值随距离衰减
                    value = round(random.uniform(-1.5/dist, -0.5/dist), 2)
                    row.append(value)
                else:
                    # 在带外以0.2的概率生成小的非零元素
                    if random.random() < 0.2:
                        value = round(random.uniform(-0.2, -0.1), 2)
                        row.append(value)
                    else:
                        row.append(0)
        matrix.append(row)
    
    # 确保矩阵对称
    for i in range(size):
        for j in range(i+1, size):
            matrix[j][i] = matrix[i][j]

    # 确保对角占优
    for i in range(size):
        row_sum = sum(abs(x) for j, x in enumerate(matrix[i]) if j != i)
        if abs(matrix[i][i]) <= row_sum:
            matrix[i][i] = row_sum + random.uniform(0.5, 1.0)

    return matrix

def print_matrix(matrix, b=None):
    """以可视化格式打印矩阵和向量"""
    print("\n生成的矩阵A:")
    for row in matrix:
        print(" ".join(f"{x:3}" for x in row))
    
    if b is not None:
        print("\n向量b:")
        print(" ".join(f"{x:3}" for x in b))

def generate_config_format(size, matrix, b):
    """生成配置文件格式的字符串"""
    config = "[Solver]\n"
    config += "type = sor\n"
    config += "tolerance = 1e-6\n"
    config += "max_iterations = 1000\n\n"
    
    config += "[Matrix]\n"
    config += f"size = {size}\n\n"
    config += "# 系数矩阵 A (按行排列)\n"
    config += "# 这是一个对角占优的矩阵\n"
    
    A_str = ";".join(",".join(str(x) for x in row) for row in matrix)
    config += f"A = {A_str}\n\n"
    config += "# 常数向量 b\n"
    b_str = ",".join(str(x) for x in b)
    config += f"b = {b_str}"
    return config

def generate_result_format(size, matrix, b):
    """生成结果文件格式的字符串"""
    result = "矩阵规模: {}\n\n".format(size)
    
    # 添加矩阵可视化
    result += "系数矩阵 A:\n"
    max_width = max(len(f"{x:.2f}") for row in matrix for x in row)
    format_str = f"{{:>{max_width+1}.2f}}"
    
    for row in matrix:
        result += " ".join(format_str.format(x) for x in row) + "\n"
    
    # 添加向量b
    result += "\n常数向量 b:\n"
    result += " ".join(f"{x:>3}" for x in b) + "\n"
    
    # 添加矩阵特征信息
    result += "\n矩阵特征:\n"
    for i in range(size):
        diagonal = abs(matrix[i][i])
        row_sum = sum(abs(x) for j, x in enumerate(matrix[i]) if j != i)
        ratio = diagonal / row_sum if row_sum != 0 else float('inf')
        result += f"第{i+1:>3}行: 对角元素={diagonal:>6.2f}, 其他元素和={row_sum:>6.2f}, 比值={ratio:>6.2f}\n"
    
    return result

def main():
    parser = argparse.ArgumentParser(
        description='生成对角占优矩阵的配置文件',
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog='''
示例:
  %(prog)s -s 3                    # 生成3x3矩阵并保存到config.ini
  %(prog)s -s 4 -d 8 -o test.ini   # 生成4x4矩阵，对角线元素为8，保存到test.ini
  %(prog)s -s 3 --no-visual        # 只输出配置文件格式
'''
    )
    
    parser.add_argument('-s', '--size', type=int, required=True,
                        help='矩阵大小 (必需)')
    parser.add_argument('-d', '--diagonal', type=float,
                        help='对角线元素的值 (默认为2*size)')
    parser.add_argument('-o', '--output', type=str, default='config.ini',
                        help='输出文件路径 (默认为config.ini)')
    parser.add_argument('-r', '--result', type=str,
                        help='结果输出文件路径 (可选)')
    parser.add_argument('--no-visual', action='store_true',
                        help='不显示可视化矩阵')
    parser.add_argument('-v', '--version', action='version',
                        version='%(prog)s 1.0')

    args = parser.parse_args()

    if args.size <= 0:
        parser.error("矩阵大小必须大于0")

    # 生成矩阵和向量
    matrix = generate_matrix(args.size, args.diagonal)
    b = list(range(1, args.size + 1))

    # 保存配置文件
    try:
        with open(args.output, 'w') as f:
            f.write(generate_config_format(args.size, matrix, b))
        print(f"配置已保存到: {args.output}")
    except IOError as e:
        print(f"错误: 无法写入配置文件 {args.output}", file=sys.stderr)
        print(f"原因: {e}", file=sys.stderr)
        sys.exit(1)

    # 保存结果文件
    if args.result:
        try:
            with open(args.result, 'w') as f:
                f.write(generate_result_format(args.size, matrix, b))
            print(f"结果已保存到: {args.result}")
        except IOError as e:
            print(f"错误: 无法写入结果文件 {args.result}", file=sys.stderr)
            print(f"原因: {e}", file=sys.stderr)
            sys.exit(1)
    
    # 显示可视化输出
    if not args.no_visual:
        print("\n" + generate_result_format(args.size, matrix, b))

if __name__ == "__main__":
    main()