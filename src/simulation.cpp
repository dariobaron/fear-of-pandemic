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
double mean_degree;
{
	Network network(input_filename);
	mean_degree = network.getMeanDegree();
}


double delta_time(3./24);									// 3hrs [days]
double start_time(0);
double mu = 1. / 2 * delta_time;							// 0.5 infectivity/day
double beta = 1. / 2 * delta_time / mean_degree;			// 0.5 exposures/day
double gamma = 1. / 6 * delta_time;							// 0.16 recovers/day
double f_D = 0.01;											// death probability


unsigned int N_runs(100);
unsigned int run(0);
//vector<vector<double>> t;
//vector<vector<unsigned int>> S;
//vector<vector<unsigned int>> E;
//vector<vector<unsigned int>> I;
//vector<vector<unsigned int>> R;
//vector<vector<unsigned int>> D;

while (run < N_runs){
	
	Network network(input_filename);
	
	Discretizer time_generator(delta_time, start_time);

	Epidemic epidemic(network.getNodes(), time_generator, mu, beta, gamma, f_D);
	
	epidemic.seedEpidemic(2);
	epidemic.evolve();

	auto t_single = epidemic.gett();
	auto S_single = epidemic.getS();
	auto E_single = epidemic.getE();
	auto I_single = epidemic.getI();
	auto R_single = epidemic.getR();
	auto D_single = epidemic.getD();

	if (S_single.back() > R_single.back()){		// N.B. to be improved! Not working for general epidemic parameters
		cerr << "Epidemic failed to spread on run " << run << ". Removed from analysis." << endl;
	}
	else {
//		t.push_back(t_single);
//		S.push_back(S_single);
//		E.push_back(E_single);
//		I.push_back(I_single);
//		R.push_back(R_single);
//		D.push_back(D_single);
		output.open("../output/no_awareness/"+to_string(run)+".txt");
		for (auto i = 0; i < t_single.size(); ++i){
			output << t_single[i] << " " << S_single[i] << " " << E_single[i] << " ";
			output << I_single[i] << " " << R_single[i] << " " << D_single[i] << endl;
		}
		output.close();
		output.clear();
		++run;
	}

	cout << "Processing : " << run * 100 / N_runs << "%\r" << flush; 
}
cout << "Completed" << endl;


//Results results(t, S, E, I, R, D);

//results.compute_means();

//result.print()


return 0;
}