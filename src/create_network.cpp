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
#include <filesystem>
#include "constants.hpp"
#include "global.hpp"
#include "network.hpp"
#include "distribution.hpp"


std::mt19937_64 random_engine(12031997);
UniformDistribution uniform_distribution(random_engine);

std::ofstream output;
std::ofstream logstream("log.txt");

int main(int argc, char const *argv[]){

for (auto i = 0; i < 10000; ++i){
	random_engine();
}

if (argc != 3){
	std::cerr << "USAGE: './create_network.x NUMBER_OF_NODES EDGES_PER_NODE'" << std::endl << "Terminating..." << std::endl;
	return -1;
}

int N_nodes, edges_per_node;
std::stringstream ss;
ss << argv[1] << " " << argv[2];
ss >> N_nodes >> edges_per_node;

std::string output_directory = "../input/bianconi-barabasi_" + std::string(argv[1]) + "_" + std::string(argv[2]) + "/";
std::filesystem::create_directories(output_directory);

for (auto run = 0; run < N_runs; ++run){
    std::string output_filename =  output_directory + std::to_string(run) + ".adjlist";

    std::cout << "Creating network in '" + output_filename + "'..." << std::endl;

    Network network(N_nodes);

    UniformDistribution fitness_distribution(random_engine, 0, 1);
    network.initEdgesBianconiBarabasi(fitness_distribution, edges_per_node);

    network.writeAdjlist(output_filename);
}

return 0;
}