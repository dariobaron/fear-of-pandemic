#include <iostream>
#include <string>
#include <random>
#include "global.hpp"
#include "network.hpp"
#include "node.hpp"
#include "epidemic.hpp"
#include "discretizer.hpp"

using namespace std;

mt19937_64 random_engine(12031997);
uniform_real_distribution<> uniform_distribution(0., 1.);

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

if (!network.checkIdIntegrity()){
	cout << "ERROR : the node ID does not correspond to the index in the array" << endl;
	return -1;
}


double delta_time(3./24);								// 3hrs [days]
double start_time(0);
Discretizer time_generator(delta_time, start_time);

Epidemic epidemic(network.getNodes(), time_generator);
epidemic.seedEpidemic(10);

auto nodes = network.getNodes();

return 0;
}