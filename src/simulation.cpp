#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include "global.hpp"
#include "network.hpp"
#include "node.hpp"
#include "epidemic.hpp"
#include "discretizer.hpp"
#include "distribution.hpp"

using namespace std;

mt19937_64 random_engine(1297);
Distribution<uniform_real_distribution<>,mt19937_64> uniform_distribution(uniform_real_distribution<>(0., 1.), &random_engine);

ofstream output;
ofstream logstream("log.txt");

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


double delta_time(3./24);									// 3hrs [days]
double start_time(0);
double mu = 1. / 2 * delta_time;							// 0.5 infectivity/day
double beta = 1. / 2 * delta_time / network.getMeanDegree();// 0.5 exposures/day
double gamma = 1. / 6 * delta_time;							// 0.16 recovers/day
double f_D = 0.01;											// death probability


Network network(input_filename);
Discretizer time_generator(delta_time, start_time);
Epidemic epidemic(network.getNodes(), time_generator, mu, beta, gamma, f_D);
epidemic.seedEpidemic(2);

output.open("../output/prova.txt");

epidemic.evolve();

auto t = epidemic.gett();
auto S = epidemic.getS();
auto E = epidemic.getE();
auto I = epidemic.getI();
auto R = epidemic.getR();
auto D = epidemic.getD();
for (auto i = 0; i < t.size(); ++i){
	output << t[i] << " " << S[i] << " " << E[i] << " " << I[i];
	output << " " << R[i] << " " << D[i] << endl;
}

output.close();
output.clear();

return 0;
}