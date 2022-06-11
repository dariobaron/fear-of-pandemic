#include <iostream>
#include <string>
#include <random>
#include "global.hpp"
#include "network.hpp"
#include "node.hpp"

using namespace std;

mt19937_64 random_engine(12031997);

int main(int argc, char const *argv[]){

for (auto i = 0; i < 10000; ++i){
	random_engine();
}

if (argc != 2){
	cerr << "USAGE: './simulation.x PATH_TO_SOURCE_NETWORK'" << endl << "Terminating\t..." << endl;
	return -1;
}

string input_filename(argv[1]);

cout << "Processing network in '" + input_filename + "'..." << endl;

Network network(input_filename);


return 0;
}