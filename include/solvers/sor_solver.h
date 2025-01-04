#pragma once
#include "../core/solver.h"

class SORSolver : public Solver
{
public:
    SORSolver(double omega = 1.5) : omega_(omega) {} // 默认松弛因子为1.5
    bool solve(std::vector<double> &x) override;

private:
    double omega_; // 松弛因子
};