#include <iostream>
#include <unordered_set>
#include "random.hpp"
#include "global.hpp"
#include "distribution.hpp"
#include "save.hpp"

using namespace std;

mt19937_64 random_engine(12031997);
Distribution<uniform_real_distribution<>,mt19937_64> uniform_distribution(uniform_real_distribution<>(0., 1.), &random_engine);

ofstream logstream("log.txt");

int main(){

vector<double> prova0({0.125,0.25,0.375,0.5,0.625,0.75});
vector<int> prova1({1,2,3,4,5,6});
vector<int> prova2({1,2,3,4,5,6});
vector<int> prova3({1,2,3,4,5,6});
vector<int> prova4({1,2,3,4,5,6});
vector<int> prova5({1,2,3,4,5,6});



return 0;
}