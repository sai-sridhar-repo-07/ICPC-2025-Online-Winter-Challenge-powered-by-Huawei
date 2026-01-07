#ifndef UTILS_H
#define UTILS_H

// Computes OXC port index for a given
// group, local spine index, and link number
inline int get_port(
    int group,
    int spine_local,
    int link,
    int ports_per_group,
    int K
) {
    return group * ports_per_group + spine_local * K + link;
}

#endif
