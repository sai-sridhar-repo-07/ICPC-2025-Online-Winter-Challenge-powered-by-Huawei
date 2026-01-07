#include "solver.h"
#include "utils.h"
#include <algorithm>
#include <climits>

using namespace std;

void solve_query(
    int N, int S, int L, int M, int K, int P,
    OXCConfig &oxc,
    const vector<array<int,4>> &flows,
    vector<array<int,5>> &routes
) {
    int spines_per_plane = S / P;
    int oxcs_per_plane = M / P;
    int ports_per_group = spines_per_plane * K;

    // Per-query load tracking
    vector<vector<vector<int>>> leaf_spine_load(
        N, vector<vector<int>>(L, vector<int>(S, 0))
    );

    vector<vector<vector<vector<int>>>> spine_oxc_load(
        N, vector<vector<vector<int>>>(
            S, vector<vector<int>>(M, vector<int>(K, 0))
        )
    );

    for (auto &f : flows) {
        int gA = f[0], leafA = f[1];
        int gB = f[2], leafB = f[3];

        long long best_score = LLONG_MAX;

        int best_sA=0, best_sB=0, best_kA=0, best_kB=0;
        int best_m=0, best_portA=0, best_portB=0;
        bool best_already=false;
        int best_nlA=0, best_nlB=0, best_nsA=0, best_nsB=0;

        // Iterate plane-wise (mandatory constraint)
        for (int p = 0; p < P; p++) {
            int spine_start = p * spines_per_plane;
            int spine_end   = spine_start + spines_per_plane;
            int oxc_start   = p * oxcs_per_plane;
            int oxc_end     = oxc_start + oxcs_per_plane;

            for (int sA = spine_start; sA < spine_end; sA++) {
                for (int sB = spine_start; sB < spine_end; sB++) {
                    for (int m = oxc_start; m < oxc_end; m++) {
                        for (int kA = 0; kA < K; kA++) {
                            for (int kB = 0; kB < K; kB++) {

                                int portA = get_port(
                                    gA, sA % spines_per_plane, kA,
                                    ports_per_group, K
                                );
                                int portB = get_port(
                                    gB, sB % spines_per_plane, kB,
                                    ports_per_group, K
                                );

                                // Do not break active connections
                                if (oxc.config[m][portA] != -1 &&
                                    oxc.config[m][portA] != portB &&
                                    oxc.port_use[m][portA] > 0) continue;

                                if (oxc.config[m][portB] != -1 &&
                                    oxc.config[m][portB] != portA &&
                                    oxc.port_use[m][portB] > 0) continue;

                                bool already = (oxc.config[m][portA] == portB);

                                int nlA = leaf_spine_load[gA][leafA][sA] + 1;
                                int nlB = leaf_spine_load[gB][leafB][sB] + 1;
                                int nsA = spine_oxc_load[gA][sA][m][kA] + 1;
                                int nsB = spine_oxc_load[gB][sB][m][kB] + 1;

                                int max_load = max({nlA, nlB, nsA, nsB});
                                long long score = (long long)max_load * 1000
                                                + (already ? 0 : 1);

                                if (score < best_score) {
                                    best_score = score;
                                    best_sA = sA; best_sB = sB;
                                    best_kA = kA; best_kB = kB;
                                    best_m  = m;
                                    best_portA = portA;
                                    best_portB = portB;
                                    best_already = already;
                                    best_nlA = nlA; best_nlB = nlB;
                                    best_nsA = nsA; best_nsB = nsB;
                                }
                            }
                        }
                    }
                }
            }
        }

        // Apply best assignment safely
        if (!best_already) {
            if (oxc.config[best_m][best_portA] != -1 &&
                oxc.port_use[best_m][best_portA] == 0) {
                int old = oxc.config[best_m][best_portA];
                oxc.config[best_m][old] = -1;
                oxc.config[best_m][best_portA] = -1;
            }
            if (oxc.config[best_m][best_portB] != -1 &&
                oxc.port_use[best_m][best_portB] == 0) {
                int old = oxc.config[best_m][best_portB];
                oxc.config[best_m][old] = -1;
                oxc.config[best_m][best_portB] = -1;
            }
            oxc.config[best_m][best_portA] = best_portB;
            oxc.config[best_m][best_portB] = best_portA;
        }

        oxc.port_use[best_m][best_portA]++;
        oxc.port_use[best_m][best_portB]++;

        leaf_spine_load[gA][leafA][best_sA] = best_nlA;
        leaf_spine_load[gB][leafB][best_sB] = best_nlB;
        spine_oxc_load[gA][best_sA][best_m][best_kA] = best_nsA;
        spine_oxc_load[gB][best_sB][best_m][best_kB] = best_nsB;

        routes.push_back({
            best_sA, best_kA, best_m, best_sB, best_kB
        });
    }
}
