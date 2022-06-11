/*************
 * PROGRAM TO GENERATE A FULLY-CONNECTED NETWORK
 * 
 * The produced network is saved into the input folder to
 * be used by the main program.
 * 
**********/

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "global.hpp"
#include "network.hpp"

using namespace std;

mt19937_64 random_engine(12031997);

ofstream output;

int main(int argc, char const *argv[]){

for (auto i = 0; i < 10000; ++i){
	random_engine();
}

if (argc != 2){
	cerr << "USAGE: './simulation.x NUMBER_OF_NODES'" << endl << "Terminating\t..." << endl;
	return -1;
}

int N_nodes;
stringstream ss(argv[1]);
ss >> N_nodes;

string output_filename = "../input/bianconi-barabasi_" + string(argv[1]) + ".adjlist";

cout << "Creating network in '" + output_filename + "'..." << endl;

Network network(N_nodes);

uniform_real_distribution<> fitness_distribution(0, 1);
network.initEdgesBianconiBarabasi(fitness_distribution, 5);

network.writeAdjlist(output_filename);

return 0;
}