#include "../../include/solvers/sor_solver.h"
#include <cmath>
#include <iostream>

bool SORSolver::solve(std::vector<double> &x)
{
    const int n = A_.size();
    std::vector<double> x_new(x);

    // 检查对角线元素是否为0
    for (int i = 0; i < n; ++i)
    {
        if (std::abs(A_[i][i]) < 1e-10)
        {
            std::cout << "对角线元素太接近0，无法求解" << std::endl;
            return false;
        }
    }

    // 迭代求解
    for (int iter = 0; iter < maxIterations_; ++iter)
    {
        double maxDiff = 0.0;

        // 计算新的x值
        for (int i = 0; i < n; ++i)
        {
            double sum1 = 0.0; // 已更新的元素之和
            double sum2 = 0.0; // 未更新的元素之和

            for (int j = 0; j < i; ++j)
            {
                sum1 += A_[i][j] * x_new[j];
            }
            for (int j = i + 1; j < n; ++j)
            {
                sum2 += A_[i][j] * x[j];
            }

            // SOR迭代公式
            double x_old = x[i];
            x_new[i] = (1 - omega_) * x_old +
                       (omega_ / A_[i][i]) * (b_[i] - sum1 - sum2);

            maxDiff = std::max(maxDiff, std::abs(x_new[i] - x_old));
        }

        // 更新x值
        x = x_new;

        if (maxDiff < tolerance_)
        {
            std::cout << "迭代次数: " << iter + 1 << std::endl;
            return true; // 收敛
        }
    }

    std::cout << "达到最大迭代次数仍未收敛" << std::endl;
    return false;
}