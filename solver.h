#ifndef SOLVER_H
#define SOLVER_H

#include <vector>
#include <array>
#include "oxc_config.h"

// Solves one query and updates OXC configuration
void solve_query(
    int N, int S, int L, int M, int K, int P,
    OXCConfig &oxc,
    const std::vector<std::array<int,4>> &flows,
    std::vector<std::array<int,5>> &routes
);

#endif
