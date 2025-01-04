#pragma once
#include "../core/solver.h"

class GaussSolver : public Solver
{
public:
    bool solve(std::vector<double> &x) override;
};