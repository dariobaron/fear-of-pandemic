#ifndef constants_h
#define constants_h


// number of runs for each set of (hyper/)parameters
constexpr unsigned N_runs = 100;

// initial infecteds
constexpr unsigned I0 = 1;

// time discretization
constexpr double delta_time = 3./24;					// 3hrs [days]

// starting time of the simulation
constexpr double start_time = 0;

// latency rate
constexpr double mu = 1. / 2 * delta_time;				// 0.5 infectivity/day

// transmissibility rate
constexpr double beta_tot = 1. / 2 * delta_time;		// 0.5 exposures/day

// recovery rate
constexpr double ggamma = 1. / 6 * delta_time;			// 0.16 recovers/day

// fatality probability
constexpr double f_D = 0.01;

// awareness for the local deads
constexpr double delta_s_normalized = 5e-5 * delta_time;// with N*delta_s_normalized/dt D : beta -> beta/2

// awareness for the cumulative deads
constexpr double delta_l_normalized = 1e-3;				// with N*delta_l_normalized D : beta -> beta/2

// awareness for infected neighbours
constexpr double delta_nI = 2. / 8;						// with 2/8 of neighbours I : beta -> beta/2

// awareness for dead neighbours
constexpr double delta_nD = 1. / 9 / 8; 				// with 1/8 of neighbours D : beta -> beta/10


#endif