#include "../../include/solvers/jacobi_solver.h"
#include <cmath>

bool JacobiSolver::solve(std::vector<double> &x)
{
    const int n = A_.size();
    std::vector<double> x_new(n);

    // 检查对角线元素是否为0
    for (int i = 0; i < n; ++i)
    {
        if (std::abs(A_[i][i]) < 1e-10)
        {
            return false; // 对角线元素太接近0，无法求解
        }
    }

    // 迭代求解
    for (int iter = 0; iter < maxIterations_; ++iter)
    {
        // 计算新的x值
        for (int i = 0; i < n; ++i)
        {
            double sum = 0.0;
            for (int j = 0; j < n; ++j)
            {
                if (j != i)
                {
                    sum += A_[i][j] * x[j];
                }
            }
            x_new[i] = (b_[i] - sum) / A_[i][i];
        }

        // 检查收敛性
        double diff = 0.0;
        for (int i = 0; i < n; ++i)
        {
            diff = std::max(diff, std::abs(x_new[i] - x[i]));
        }

        // 更新x值
        x = x_new;

        if (diff < tolerance_)
        {
            return true; // 收敛
        }
    }

    return false; // 达到最大迭代次数仍未收敛
}