# Huawei OXC Ports Arrangement Solver

This repository contains a high-performance heuristic solution for the
**Huawei Optical Circuit Switch (OXC) Ports Arrangement Problem**
(Codeforces Round 2177).

## 🧠 Problem Overview

The task is to route inter-group traffic in a multi-plane network using
optical circuit switches while:

- Minimizing maximum flow conflict
- Minimizing OXC physical reconfiguration cost
- Respecting strict plane and port constraints

## 🚀 Solution Strategy

The solver uses a greedy, load-aware heuristic:

- Plane-respecting routing
- Max-link-load minimization (primary objective)
- Persistent OXC topology across queries
- Safe rewiring using port usage tracking
- Zero illegal topology transitions

This approach achieved **Rank ~400 globally** in the contest.

## 📁 Project Structure

