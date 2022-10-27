/*************
 * PROGRAM TO GENERATE A BIANCONI-BARABASI NETWORK
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
#include "distribution.hpp"

using namespace std;

mt19937_64 random_engine(12031997);
Distribution<uniform_real_distribution<>,mt19937_64> uniform_distribution(uniform_real_distribution<>(0., 1.), &random_engine);

ofstream output;
ofstream logstream("log.txt");

int main(int argc, char const *argv[]){

for (auto i = 0; i < 10000; ++i){
	random_engine();
}

if (argc != 3){
	cerr << "USAGE: './create_network.x NUMBER_OF_NODES EDGES_PER_NODE'" << endl << "Terminating\t..." << endl;
	return -1;
}

int N_nodes, edges_per_node;
stringstream ss;
ss << argv[1] << " " << argv[2];
ss >> N_nodes >> edges_per_node;

string output_filename = "../input/bianconi-barabasi_" + string(argv[1]) + "_" + string(argv[2]) + ".adjlist";

cout << "Creating network in '" + output_filename + "'..." << endl;

Network network(N_nodes);

uniform_real_distribution<> fitness_distribution(0, 1);
network.initEdgesBianconiBarabasi(fitness_distribution, edges_per_node);

if (network.checkIdIntegrity()){
    cout << "OK : The node index corresponds to its ID" << endl;
}
else{
    cout << "ERR : There was an error in the network" << endl;
}

network.writeAdjlist(output_filename);

return 0;
}