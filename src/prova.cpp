#include <iostream>
#include <unordered_set>
#include "random.hpp"
#include "global.hpp"
#include "distribution.hpp"
#include "network.hpp"
#include "node.hpp"
#include "awareness.hpp"

using namespace std;

mt19937_64 random_engine(12031997);
UniformDistribution uniform_distribution(random_engine);

ofstream logstream("log.txt");


int main(){

Network network("../input/barabasi-albert_100_4.adjlist");
cout << "imported network" << endl;

NoAwareness awareness;
cout << "created awareness" << endl;

return 0;
}