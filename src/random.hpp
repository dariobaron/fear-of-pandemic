#ifndef random_h
#define random_h

#include <vector>
#include <unordered_set>
#include <random>
#include "global.hpp"

using namespace std;


template<typename T>
vector<T> randomChoice(const vector<T> & v, int N_extractions, vector<double> weights){
	const int v_size = v.size();
	if (N_extractions >= v_size){
		cerr << "ERROR: too many extractions required, exceeding input vector" << endl;
		return vector<T>();
	}
	weights.resize(v_size);
	vector<T> extractions(N_extractions);
	double normalization(accumulate(weights.begin(), weights.end(), 0.));
	for (auto i = 0; i < N_extractions; ++i){
		double rnd(uniform_distribution() * normalization);
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
vector<T> randomChoice(vector<T> v, int N_extractions, bool replace=false){
	int v_size = v.size();
	if (!replace && (N_extractions>=v_size)){
		cerr << "ERROR: too many extractions required, exceeding input vector" << endl;
		return vector<T>();
	}
	vector<T> extractions(N_extractions);
	for (auto i = 0; i < N_extractions; ++i){
		int rnd_idx = uniform_distribution() * v_size;
		extractions[i] = v[rnd_idx];
		if (!replace){
			--v_size;
			swap(v[rnd_idx],v[v_size]);
		}
	}
	return extractions;
}


#endif