#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <stdexcept>
#include <filesystem>
#include <cmath>
#include "global.hpp"
#include "network.hpp"
#include "node.hpp"
#include "epidemic.hpp"
#include "discretizer.hpp"
#include "distribution.hpp"
#include "results.hpp"
#include "awareness.hpp"
#include "constants.hpp"
#include "timer.hpp"

std::mt19937_64 random_engine(12031997);
UniformDistribution uniform_distribution(random_engine);

std::ofstream output;
std::ofstream logstream("log.txt");

int main(int argc, char const *argv[]){

Timer timer;

// random engine burn-in
for (auto i = 0; i < 10000; ++i){
	random_engine();
}


if (argc != 5){
	std::cerr << "USAGE: './simulation.x [1] [2] [3] [4] [5]'\n[1] : PATH_TO_SOURCE_NETWORK\n[2] : REACTION_TYPE[none/soften/cut]\n";
	std::cerr << "[3] : FEAR_DISTRIBUTION[none/fixed/uni/asyLow/asyHigh/bi/corr/anticorr]\n";
	std::cerr << "[4] : FEEDBACK_TYPE[none/short/long/shortlong/neighbours/all]";
	std::cerr << std::endl << "Terminating..." << std::endl;
	return -1;
}


std::string input_filename(argv[1]);
std::clog << "Processing network in '" + input_filename + "'" << std::endl;
std::clog << "with parameters REACTION_TYPE=" << argv[2] << " FEAR_DISTRIBUTION=" << argv[3] << " FEEDBACK_TYPE=" << argv[4] << std::endl;
Network network(input_filename);
double mean_degree = network.getMeanDegree();
// correcting the epidemiological constants
// for this specific network:
// - the trasmissibility rate for each contact
const double beta = beta_tot / mean_degree;		// 0.5 exposures/day/contact
// - awareness for the local deads for the entire population
const double delta_s = network.size() * delta_s_normalized;
// - awareness for the cumulative deads for the entire population
const double delta_l = network.size() * delta_l_normalized;


std::string reaction_type(argv[2]);
bool cut_connections;
if (reaction_type == "soften"){	cut_connections = false;	}
else if (reaction_type == "cut"){	cut_connections = true;	}
else if (reaction_type == "none"){}
else {	throw std::invalid_argument("Wrong REACTION_TYPE passed : "+reaction_type+" is not a defined reaction type");	}


std::string fear_distribution(argv[3]);
Distribution * fear_distr = nullptr;
if		(fear_distribution == "fixed")		{	fear_distr = new DeltaDistribution(random_engine, 0.5);	}
else if (fear_distribution == "uni")		{	fear_distr = new BetaDistribution(random_engine, 5, 5);	}
else if (fear_distribution == "asyLow")		{	fear_distr = new BiBetaDistribution(random_engine, 2, 8);	}
else if (fear_distribution == "asyHigh")	{	fear_distr = new BiBetaDistribution(random_engine, 8, 2);	}
else if (fear_distribution == "bi")			{	fear_distr = new BiBetaDistribution(random_engine, 2, 8); }
else if (fear_distribution == "anticorr" || fear_distribution == "corr"){
	std::vector<int> degrees(network.size());
	unsigned int i = 0;
	for (auto n : network.getNodes()){
		degrees[i] = n->degree();	++i;
	}
	if (fear_distribution == "anticorr"){	fear_distr = new AntiCorrDistribution(random_engine, degrees);	}
	else if (fear_distribution == "corr"){	fear_distr = new CorrDistribution(random_engine, degrees);	}
}
else if (fear_distribution == "none")		{}
else										{	throw std::invalid_argument("Wrong FEAR_DISTRIBUTION passed : "+fear_distribution+" is not a defined distribution for fear");	}


std::string feedback_type(argv[4]);
Awareness * aware_ptr = nullptr;
if		(feedback_type == "none")		{	aware_ptr = new NoAwareness();	}
else if (feedback_type == "short")		{	aware_ptr = new ShortAwareness(delta_s, cut_connections);	}
else if (feedback_type == "long")		{	aware_ptr = new LongAwareness(delta_l, cut_connections);	}
else if (feedback_type == "shortlong")	{	aware_ptr = new ShortLongAwareness(delta_s, delta_l, cut_connections);	}
else if (feedback_type == "neighbours")	{	aware_ptr = new NeighbourAwareness(delta_nI, delta_nD, cut_connections);	}
else if (feedback_type == "all")		{	aware_ptr = new AllAwareness(delta_s, delta_l, delta_nI, delta_nD, cut_connections);	}
else 									{	throw std::invalid_argument("Wrong FEEDBACK_TYPE passed : "+feedback_type+" is not a defined feedback type");	}


auto str_loc = input_filename.find("input");
std::string output_directory = input_filename.replace(str_loc, str_loc+5, "output");
output_directory = output_directory.substr(0, output_directory.find_last_of("."));
output_directory = output_directory + "/REACTION" + reaction_type
									+ "_FEAR" + fear_distribution
									+ "_FEEDBACK" + feedback_type + "/";
std::filesystem::create_directories(output_directory);

unsigned int N_runs = 100;
unsigned int run = 0;
unsigned int failed_runs = 0;
while (run < N_runs){
	
	Discretizer time_generator(delta_time, start_time);

	Epidemic epidemic(network.getNodes(), time_generator, *aware_ptr, *fear_distr, mu, beta, ggamma, f_D);
	
	epidemic.seedEpidemic(2);
	epidemic.evolve();


	if (epidemic.outbreakHappened()){

		Results results(epidemic);

		std::string output_filename =  std::to_string(run) + ".txt";
		output.open(output_directory + output_filename);
		
		results.writeData(output);
		
		output.close();
		output.clear();
		
		++run;
	}
	
	else {
		++failed_runs;
	}

	std::cout << "Processing : " << run * 100 / N_runs << "%\r" << std::flush; 
}

output.open(output_directory + "rate_early_extinction.txt");
output << (double)(failed_runs) / (run+failed_runs) << std::endl;
output.close();
output.clear();

std::clog << "\r----------Completed in " << timer.stop() << " sec ----------" << std::endl << std::endl;


//Results results(t, S, E, I, R, D);

//results.compute_means();

//result.print()


return 0;
}
