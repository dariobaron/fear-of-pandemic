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

Network network("../input/bianconi-barabasi_10000_10/0.adjlist");
cout << "imported network" << endl;

Network othernet(10000);

othernet.initEdgesBianconiBarabasi(uniform_distribution, 10);

cout << network.getMeanDegree() << " vs " << othernet.getMeanDegree() << endl;

return 0;
}