#ifndef global_h
#define global_h

#include <random>
#include "distribution.hpp"
using namespace std;

extern mt19937_64 random_engine;

extern Distribution<uniform_real_distribution<>,mt19937_64> uniform_distribution;

#endif