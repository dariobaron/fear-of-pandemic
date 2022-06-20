#include <iostream>
#include <random>
#include <algorithm>
#include <vector>
#include "global.hpp"

using namespace std;

mt19937_64 random_engine(12031997);

int main(){

cout << endl;

uniform_real_distribution<> distribution(0.0,1.0);

int N(6);
vector<int> v(N);
cout << "Vettore v:" << endl;
for (auto i = 0; i < N; ++i){
	v[i] = i;
	cout << v[i] << " ";
}
cout << endl << endl;


vector<double> weights(10, 10);
cout << "Vettore weights:" << endl;
for (auto i = 0; i < N; ++i){
	cout << weights[i] << " ";
}
cout << endl << endl;
vector<double> probability(10, 0);

transform(v.begin(), v.begin()+3, weights.begin(), probability.begin(), multiplies<double>());

cout << "Vettore probability:" << endl;
for (auto i = 0; i < N; ++i){
	cout << probability[i] << " ";
}
cout << endl << endl;


return 0;
}