#pragma once
#include <vector>

class Solver
{
public:
    virtual ~Solver() = default;

    // 设置方程组 Ax = b
    void setEquation(const std::vector<std::vector<double> > &A,
                     const std::vector<double> &b);

    // 求解方程
    virtual bool solve(std::vector<double> &x) = 0;

    // 设置迭代精度和最大迭代次数
    void setParameters(double tolerance, int maxIterations);

    // 检查矩阵是否可解
    bool checkSolvability() const;

protected:
    std::vector<std::vector<double> > A_;
    std::vector<double> b_;
    double tolerance_ = 1e-6;
    int maxIterations_ = 1000;

    // 检查矩阵维度
    bool checkDimensions() const;
    // 检查矩阵是否为零矩阵
    bool checkZeroMatrix() const;
    // 检查对角占优性
    bool checkDiagonalDominance() const;
};