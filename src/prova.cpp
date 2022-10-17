#include <iostream>
#include <unordered_set>
#include "random.hpp"
#include "global.hpp"
#include "distribution.hpp"

using namespace std;

mt19937_64 random_engine(12031997);
Distribution<uniform_real_distribution<>,mt19937_64> uniform_distribution(uniform_real_distribution<>(0., 1.), &random_engine);

int main(){

vector<int> prova({1,2,3,4,5,6,7,8,9,10});

cout << prova.size() << endl;

for (auto i : prova){
	cout << i << " ";
}
cout << endl;

auto v = randomChoice(prova, 8);

for (auto i : v){
	cout << i << " ";
}
cout << endl;

v = randomChoice(prova, 8, true);

for (auto i : v){
	cout << i << " ";
}
cout << endl;

return 0;
}