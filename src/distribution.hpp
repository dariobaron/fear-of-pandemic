#ifndef distribution_h
#define distribution_h

#include <iostream>
#include <vector>
#include <random>
#include <algorithm>


class Distribution{

protected:
	std::mt19937_64 & random_engine_;

public:
	Distribution(std::mt19937_64 & random_engine) : random_engine_(random_engine) {};
	
	~Distribution(){};
	
	virtual double operator()() = 0;

	virtual void reset() = 0;

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

	void reset(){};

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

	void reset(){};
};



class BetaDistribution : public Distribution{

protected:
	std::gamma_distribution<> g1_;
	std::gamma_distribution<> g2_;

public:
	BetaDistribution(std::mt19937_64 & random_engine, double alpha=1., double beta=1.)
		: Distribution(random_engine), g1_(alpha, 1), g2_(beta, 1) {};
	
	double operator()(){
		double x = g1_(random_engine_);
		double y = g2_(random_engine_);
		return x / (x + y);
	}

	void reset(){};

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

	void reset(){};

};



class AntiCorrDistribution : public Distribution{

protected:
	std::vector<int> degree_;
	std::vector<double> fear_;
	std::normal_distribution<> perturbation_;
	unsigned int cont_;

private:
	int max_degree_, min_degree_;

public:
	AntiCorrDistribution(std::mt19937_64 & random_engine, std::vector<int> & v) : Distribution(random_engine),
		degree_(v), fear_(v.size()), perturbation_(), cont_(0),
		max_degree_(*max_element(degree_.begin(), degree_.end())), min_degree_(*min_element(degree_.begin(), degree_.end())) {
			double amplitude = max_degree_ - min_degree_;
			for (auto i = 0; i < degree_.size(); ++i){
				double epsilon = perturbation_(random_engine_);
				fear_[i] = std::max(std::min((max_degree_-degree_[i])/amplitude + epsilon*0.15, 1.), 0.);
			}
	};

	double operator()(){
		return fear_[cont_++];
	}

	void reset(){
		double amplitude = max_degree_ - min_degree_;
		for (auto i = 0; i < degree_.size(); ++i){
			double epsilon = perturbation_(random_engine_);
			fear_[i] = std::max(std::min((max_degree_-degree_[i])/amplitude + epsilon*0.15, 1.), 0.);
		}
		cont_ = 0;
	};

};



class CorrDistribution : public Distribution{

protected:
	std::vector<int> degree_;
	std::vector<double> fear_;
	std::normal_distribution<> perturbation_;
	unsigned int cont_;

private:
	int max_degree_, min_degree_;

public:
	CorrDistribution(std::mt19937_64 & random_engine, std::vector<int> & v) : Distribution(random_engine),
		degree_(v), fear_(v.size()), perturbation_(), cont_(0),
		max_degree_(*max_element(degree_.begin(), degree_.end())), min_degree_(*min_element(degree_.begin(), degree_.end())) {
			double amplitude = max_degree_ - min_degree_;
			for (auto i = 0; i < degree_.size(); ++i){
				double epsilon = perturbation_(random_engine_);
				fear_[i] = std::max(std::min((degree_[i]-min_degree_)/amplitude + epsilon*0.15, 1.), 0.);
			}
	};

	double operator()(){
		return fear_[cont_++];
	}

	void reset(){
		double amplitude = max_degree_ - min_degree_;
		for (auto i = 0; i < degree_.size(); ++i){
			double epsilon = perturbation_(random_engine_);
			fear_[i] = std::max(std::min((degree_[i]-min_degree_)/amplitude + epsilon*0.15, 1.), 0.);
		}
		cont_ = 0;
	};

};


#endif