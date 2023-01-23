#ifndef global_h
#define global_h

#include <fstream>
#include <random>
#include "distribution.hpp"

extern std::mt19937_64 random_engine;

extern UniformDistribution uniform_distribution;

extern std::ofstream logstream;

#endif