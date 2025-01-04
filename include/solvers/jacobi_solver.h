#pragma once
#include "../core/solver.h"

class JacobiSolver : public Solver
{
public:
    bool solve(std::vector<double> &x) override;
};