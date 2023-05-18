#ifndef distribution_h
#define distribution_h

#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include "utilities.hpp"


class Distribution{

protected:
	std::mt19937_64 & random_engine_;

public:
	Distribution(std::mt19937_64 & random_engine) : random_engine_(random_engine) {};
	
	~Distribution(){};
	
	virtual double operator()() = 0;

	std::vector<double> generate(unsigned N){
		std::vector<double> samples(N);
		for (auto & s : samples){	s = (*this)();	}
		return samples;
	};

};



class UniformDistribution : public Distribution{

protected:
	std::uniform_real_distribution<> distribution_;

public:
	UniformDistribution(std::mt19937_64 & random_engine, double a=0., double b=1.)
		: Distribution(random_engine), distribution_(a, b) {};
	
	double operator()(){
		return distribution_(random_engine_);
	};

};



class DeltaDistribution : public Distribution{

protected:
	double value_;

public:
	DeltaDistribution(std::mt19937_64 & random_engine, double value)
		: Distribution(random_engine), value_(value) {};
	
	double operator()(){
		return value_;
	};

};



class BetaDistribution : public Distribution{

protected:
	std::gamma_distribution<> g1_;
	std::gamma_distribution<> g2_;
	double loc_;					// The start of the distribution domain (traslation)
	double scale_;					// The size of the distribution domain (enlargement)

public:
	BetaDistribution(std::mt19937_64 & random_engine, double alpha=1, double beta=1, double loc=0, double scale=1)
		: Distribution(random_engine), g1_(alpha, 1), g2_(beta, 1), loc_(loc), scale_(scale) {};
	
	double operator()(){
		double x = g1_(random_engine_);
		double y = g2_(random_engine_);
		return x / (x + y) * scale_ + loc_;
	}

};



class BiBetaDistribution : public Distribution{

protected:
	std::uniform_real_distribution<> u_;
	BetaDistribution b1_;
	BetaDistribution b2_;

public:
	BiBetaDistribution(std::mt19937_64 & random_engine, double alpha=1., double beta=1.)
		: Distribution(random_engine), u_(), b1_(random_engine, alpha, beta), b2_(random_engine, beta, alpha) {};
	
	double operator()(){
		return (u_(random_engine_)<0.5) ? b1_() : b2_();
	};

};



class AntiCorrDistribution : public Distribution{

protected:
	std::vector<double> fear_;
	unsigned int cont_;

public:
	AntiCorrDistribution(std::mt19937_64 & random_engine, std::vector<int> & v) : Distribution(random_engine), fear_(v.size()), cont_(0) {
		BetaDistribution beta(random_engine_, 2, 1, 0., 3./4);
		auto fears = beta.generate(v.size());
		std::sort(fears.begin(), fears.end());
		auto indices = argsort(v,false);
		for (auto i = 0; i < fear_.size(); ++i){
			fear_[indices[i]] = fears[i];
		}
	};

	double operator()(){
		return fear_[cont_++];
	}

};



class CorrDistribution : public Distribution{

protected:
	std::vector<double> fear_;
	unsigned int cont_;

public:
	CorrDistribution(std::mt19937_64 & random_engine, std::vector<int> & v) : Distribution(random_engine), fear_(v.size()), cont_(0) {
		BetaDistribution beta(random_engine_, 1, 2, 1./4, 3./4);
		auto fears = beta.generate(v.size());
		std::sort(fears.begin(), fears.end());
		auto indices = argsort(v);
		for (auto i = 0; i < fear_.size(); ++i){
			fear_[indices[i]] = fears[i];
		}
	};

	double operator()(){
		return fear_[cont_++];
	}

};


#endif