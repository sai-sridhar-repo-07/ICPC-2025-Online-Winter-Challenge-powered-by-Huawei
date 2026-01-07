#ifndef OXC_CONFIG_H
#define OXC_CONFIG_H

#include <vector>

// Stores physical OXC topology and port usage
struct OXCConfig {
    int M, R;

    // config[m][p] = connected port index or -1
    std::vector<std::vector<int>> config;

    // Number of flows currently using each port
    std::vector<std::vector<int>> port_use;

    OXCConfig(int m, int r)
        : M(m), R(r),
          config(m, std::vector<int>(r, -1)),
          port_use(m, std::vector<int>(r, 0)) {}
};

#endif
