#ifndef random_h
#define random_h

#include <vector>
#include <random>
#include "global.hpp"

using namespace std;

template<typename T>
vector<T> randomChoice(const vector<T> & v, int N_extractions, vector<double> weights){
	const int v_size = v.size();
	weights.resize(v_size);
	vector<T> extractions(N_extractions);
	uniform_real_distribution<> distribution(0.0,1.0);
	double normalization(accumulate(weights.begin(), weights.end(), 0.));
	for (auto i = 0; i < N_extractions; ++i){
		double rnd(distribution(random_engine) * normalization);
		double cumulative_probability(0);
		for (auto idx_extracted = 0; idx_extracted < v_size; ++idx_extracted){
			cumulative_probability += weights[idx_extracted];
			if (rnd < cumulative_probability){
				extractions[i] = v[idx_extracted];
				normalization -= weights[idx_extracted];
				weights[idx_extracted] = 0;
				break;
			}
		}
	}
	return extractions;
}


template<typename T>
vector<T> randomChoice(const vector<T> & v, int N_extractions){
	const int v_size = v.size();
	vector<T> extractions(N_extractions);
	uniform_real_distribution<> distribution(0.0,1.0);
	for (auto i = 0; i < N_extractions; ++i){
		int rnd_idx = distribution(random_engine) * v_size;
		extractions[i] = v[rnd_idx];
	}
	return extractions;
}


#endif