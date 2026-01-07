#include <bits/stdc++.h>
#include "solver.h"
#include "oxc_config.h"

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, S, L;
    int M, K, P;
    cin >> N >> S >> L;
    cin >> M >> K >> P;

    int spines_per_plane = S / P;
    int ports_per_group = spines_per_plane * K;
    int R = N * ports_per_group;

    OXCConfig oxc(M, R);

    for (int q = 0; q < 5; q++) {
        int Q;
        cin >> Q;

        vector<array<int,4>> flows(Q);
        for (int i = 0; i < Q; i++) {
            cin >> flows[i][0] >> flows[i][1]
                >> flows[i][2] >> flows[i][3];
        }

        vector<array<int,5>> routes;
        solve_query(N, S, L, M, K, P, oxc, flows, routes);

        // Output OXC configuration
        for (int m = 0; m < M; m++) {
            for (int i = 0; i < R; i++) {
                cout << oxc.config[m][i]
                     << (i + 1 == R ? '\n' : ' ');
            }
        }

        // Output routes
        for (auto &r : routes) {
            cout << r[0] << ' ' << r[1] << ' ' << r[2] << ' '
                 << r[3] << ' ' << r[4] << '\n';
        }
    }
}
