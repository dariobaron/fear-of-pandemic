#ifndef constants_h
#define constants_h


// time discretization
const double delta_time(3./24);						// 3hrs [days]

// starting time of the simulation
const double start_time(0);

// latency rate
const double mu = 1. / 2 * delta_time;				// 0.5 infectivity/day

// transmissibility rate
const double beta_tot = 1. / 2 * delta_time;		// 0.5 exposures/day

// recovery rate
const double ggamma = 1. / 6 * delta_time;			// 0.16 recovers/day

// fatality probability
const double f_D = 0.01;

// awareness for the local deads
const double delta_s_normalized = 5e-5 * delta_time;// with N*delta_s_normalized/dt D : beta -> beta/2

// awareness for the cumulative deads
const double delta_l_normalized = 1e-3;				// with N*delta_l_normalized D : beta -> beta/2

// awareness for infected neighbours
const double delta_nI = 2. / 8;						// with 2/8 of neighbours I : beta -> beta/2

// awareness for dead neighbours
const double delta_nD = 1. / 9 / 8; 				// with 1/8 of neighbours D : beta -> beta/10


#endif