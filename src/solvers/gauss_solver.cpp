#include "../../include/solvers/gauss_solver.h"
#include <cmath>

bool GaussSolver::solve(std::vector<double> &x)
{
    const int n = A_.size();
    // 创建增广矩阵 [A|b]
    std::vector<std::vector<double> > Ab = A_;
    for (int i = 0; i < n; ++i)
    {
        Ab[i].push_back(b_[i]);
    }

    // 前向消元
    for (int k = 0; k < n; ++k)
    {
        // 选主元
        int maxRow = k;
        double maxVal = std::abs(Ab[k][k]);
        for (int i = k + 1; i < n; ++i)
        {
            if (std::abs(Ab[i][k]) > maxVal)
            {
                maxVal = std::abs(Ab[i][k]);
                maxRow = i;
            }
        }

        if (maxVal < tolerance_)
        {
            return false; // 矩阵奇异
        }

        // 交换行
        if (maxRow != k)
        {
            std::swap(Ab[k], Ab[maxRow]);
        }

        // 消元
        for (int i = k + 1; i < n; ++i)
        {
            double factor = Ab[i][k] / Ab[k][k];
            for (int j = k; j <= n; ++j)
            {
                Ab[i][j] -= factor * Ab[k][j];
            }
        }
    }

    // 回代
    x.resize(n);
    for (int i = n - 1; i >= 0; --i)
    {
        double sum = 0.0;
        for (int j = i + 1; j < n; ++j)
        {
            sum += Ab[i][j] * x[j];
        }
        x[i] = (Ab[i][n] - sum) / Ab[i][i];
    }

    return true;
}