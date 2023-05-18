#include <iostream>
#include <unordered_set>
#include <vector>
#include <iomanip>
#include "random.hpp"
#include "global.hpp"
#include "distribution.hpp"
#include "network.hpp"
#include "node.hpp"
#include "awareness.hpp"
#include "utilities.hpp"

using namespace std;

mt19937_64 random_engine(12031997);
UniformDistribution uniform_distribution(random_engine);

ofstream logstream("log.txt");


int main(){

std::vector<char> order = {'b','a','e','f','d','c'};

std::vector<double> fears = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6};

auto indices = argsort(order);

std::vector<double> ordered(6);
for (auto i = 0; i < 6; ++i){
	ordered[indices[i]] = fears[i];
}

for (auto i : indices){
	cout << i << " ";
}
cout << endl << endl;

for (auto i : ordered){
	cout << i << " ";
}
cout << endl << endl;

/*
BetaDistribution beta(random_engine, 2, 1, 0., 3./4);

Network network("../input/bianconi-barabasi_100_5/0.adjlist");
cout << setprecision(3) << endl << "imported network" << endl;

std::vector<unsigned> degrees;
vector<double> fears;
for (auto n : network.getNodes()){
	degrees.push_back(n->degree());
	fears.push_back(beta());
	cout << degrees.size()-1 << ";" << degrees.back() << "\t";
}
cout << endl << endl;

auto indices = argsort(degrees,false);
for (auto i : indices){
	cout << i << " ";
}
cout << endl << endl;

sort(fears.begin(), fears.end());
for (auto f : fears){
	cout << f << "\t";
}
cout << endl << endl;

vector<double> ordered_fears(degrees.size());
for (auto i = 0; i < degrees.size(); ++i){
	ordered_fears[indices[i]] = fears[i];
}

for (auto i = 0; i < degrees.size(); ++i){
	cout << ordered_fears[i] << ";" << degrees[i] << "\t";
}
cout << endl << endl;

*/
return 0;
}