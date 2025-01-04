#include "../../include/core/solver.h"
#include <iostream>
#include <cmath>

void Solver::setEquation(const std::vector<std::vector<double> > &A,
                         const std::vector<double> &b)
{
    A_ = A;
    b_ = b;
}

void Solver::setParameters(double tolerance, int maxIterations)
{
    tolerance_ = tolerance;
    maxIterations_ = maxIterations;
}

bool Solver::checkSolvability() const
{
    if (!checkDimensions())
    {
        std::cerr << "错误：矩阵维度不匹配" << std::endl;
        return false;
    }

    if (checkZeroMatrix())
    {
        std::cerr << "错误：系数矩阵为零矩阵" << std::endl;
        return false;
    }

    if (!checkDiagonalDominance())
    {
        std::cout << "警告：矩阵不是对角占优的，可能影响求解的收敛性" << std::endl;
    }

    return true;
}

bool Solver::checkDimensions() const
{
    if (A_.empty() || A_[0].empty() || b_.empty())
        return false;

    const size_t n = A_.size();
    if (b_.size() != n)
        return false;

    for (const auto &row : A_)
        if (row.size() != n)
            return false;

    return true;
}

bool Solver::checkZeroMatrix() const
{
    for (const auto &row : A_)
        for (double val : row)
            if (std::abs(val) > tolerance_)
                return false;
    return true;
}

bool Solver::checkDiagonalDominance() const
{
    const size_t n = A_.size();
    for (size_t i = 0; i < n; ++i)
    {
        double diagonal = std::abs(A_[i][i]);
        double sum = 0.0;
        for (size_t j = 0; j < n; ++j)
        {
            if (j != i)
                sum += std::abs(A_[i][j]);
        }

        std::cout << "行 " << i + 1 << " 的对角元素: " << diagonal
                  << ", 其他元素之和: " << sum << std::endl;

        if (diagonal < sum)
        {
            std::cout << "第 " << i + 1 << " 行不满足对角占优: |" << A_[i][i]
                      << "| < " << sum << std::endl;
            return false;
        }
    }
    return true;
}