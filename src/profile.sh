#!/bin/bash

g++ -std=c++17 -Wall -Wpedantic -Wextra -Wno-sign-compare -pg simulation.cpp -o simulation_gprof

./simulation_gprof ../input/barabasi-albert_100_4.adjlist soften fixed none

gprof simulation_gprof gmon.out >analysis.txt