#ifndef random_h
#define random_h

#include <stdexcept>
#include <string>
#include <vector>
#include <random>
#include <numeric>
#include "global.hpp"


template<typename T>
std::vector<T> randomChoice(const std::vector<T> & v, int N_extractions, std::vector<double> weights){
	const int v_size = v.size();
	if (N_extractions > v_size){
		throw std::logic_error("randomChoice() cannot extract more elements ("+std::to_string(N_extractions)+") than provided ("+std::to_string(v_size)+")");
	}
	else if (N_extractions == v_size){
		std::vector<T> extractions(v);
		std::shuffle(extractions.begin(), extractions.end(), random_engine);
		return extractions;
	}
	weights.resize(v_size);
	std::vector<T> extractions(N_extractions);
	double normalization(std::accumulate(weights.begin(), weights.end(), 0.));
	for (auto i = 0; i < N_extractions; ++i){
		double rnd = uniform_distribution() * normalization;
		double cumulative_probability = 0;
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
std::vector<T> randomChoice(std::vector<T> v, int N_extractions, bool replace=false){
	int v_size = v.size();
	if (!replace && (N_extractions>v_size)){
		throw std::logic_error("randomChoice() cannot extract more elements ("+std::to_string(N_extractions)+") than provided ("+std::to_string(v_size)+")");
	}
	if (N_extractions == v_size){
		std::vector<T> extractions(v);
		std::shuffle(extractions.begin(), extractions.end(), random_engine);
		return extractions;
	}
	std::vector<T> extractions(N_extractions);
	for (auto i = 0; i < N_extractions; ++i){
		int rnd_idx = uniform_distribution() * v_size;
		extractions[i] = v[rnd_idx];
		if (!replace){
			--v_size;
			std::swap(v[rnd_idx],v[v_size]);
		}
	}
	return extractions;
}


#endif
