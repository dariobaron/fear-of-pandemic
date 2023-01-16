#ifndef epidemic_h
#define epidemic_h

#include <vector>
#include <unordered_set>
#include "global.hpp"
#include "node.hpp"
#include "discretizer.hpp"
#include "random.hpp"
#include "awareness.hpp"
#include "distribution.hpp"


class Epidemic{

protected:
	std::vector<Node*> nodes_;
	std::vector<unsigned int> S_;
	std::vector<unsigned int> E_;
	std::vector<unsigned int> I_;
	std::vector<unsigned int> R_;
	std::vector<unsigned int> D_;
	std::vector<double> t_;
	Awareness & awareness_;
	Distribution & fear_distribution_;
	double latency_rate_;
	double transmission_rate_;
	double recovery_rate_;
	double fatality_probability_;
	bool outbreak_;
	bool cut_connections_;


private:
	Discretizer time_gen_;
	std::unordered_set<Node*> exposeds_;
	std::unordered_set<Node*> infecteds_;

	void updateMetrics(const std::vector<unsigned int> & metrics_at_step){
		S_.push_back(metrics_at_step[0]);
		E_.push_back(metrics_at_step[1]);
		I_.push_back(metrics_at_step[2]);
		R_.push_back(metrics_at_step[3]);
		D_.push_back(metrics_at_step[4]);
	};

	void evolveStep(){
// getting new timestamp
		t_.push_back(time_gen_());
		awareness_.setGlobalMetric(I_, D_);
// loop over the infecteds
		for (auto node : infecteds_){
			auto contacts = awareness_.computeContacts(node);
			node->infect(transmission_rate_, contacts);
			node->recover(recovery_rate_, fatality_probability_);
		}
// loop over the exposed
		for (auto node : exposeds_){
			node->incubate(latency_rate_);
		}
// collecting new statuses
		std::vector<unsigned int> SEIRD(5, 0);
		exposeds_.clear();
		infecteds_.clear();
		for (auto node : nodes_){
			node->updateStatus();
			SEIRD[node->status()]++;
			if (node->status() == 1){
				exposeds_.insert(node);
			}
			else if (node->status() == 2){
				infecteds_.insert(node);
			}
		}
// updating the internal compartmental metrics
		updateMetrics(SEIRD);
// checking if the epidemic has broken out
		if (!outbreak_){
			outbreak_ = I_.back() > 2 * I_[0];
		}
	};

public:
	Epidemic(std::vector<Node*> v, Discretizer & time_gen, Awareness & awareness, Distribution & fear_distribution,
			double mu, double beta, double gamma, double f_D) :
			nodes_(v), S_(0), E_(0), I_(0), R_(0), D_(0), awareness_(awareness), fear_distribution_(fear_distribution),
			latency_rate_(mu), transmission_rate_(beta), recovery_rate_(gamma), fatality_probability_(f_D),
			outbreak_(false), time_gen_(time_gen), exposeds_(), infecteds_()
			{
				for (auto n : nodes_){
					n->status(0);
					n->updateStatus();
					n->fear(fear_distribution_());
				}

	};

	~Epidemic(){
		fear_distribution_.reset();
		awareness_.reset();
	};

	std::vector<double> gett() const{	return t_;	};
	std::vector<unsigned int> getS() const{	return S_;	};
	std::vector<unsigned int> getE() const{	return E_;	};
	std::vector<unsigned int> getI() const{	return I_;	};
	std::vector<unsigned int> getR() const{	return R_;	};
	std::vector<unsigned int> getD() const{	return D_;	};

	void seedEpidemic(unsigned int N_initial_infectiouses){
// selecting N_initial_infectiouses nodes at random		
		std::vector<Node*> initial_infecteds = randomChoice(nodes_, N_initial_infectiouses);
		for (auto & n : initial_infecteds){
			n->status(2);
			n->updateStatus();
			infecteds_.insert(n);
		}
// updating the metrics vectors
		t_.push_back(time_gen_.get());
		S_.push_back(nodes_.size() - N_initial_infectiouses);
		E_.push_back(0);
		I_.push_back(N_initial_infectiouses);
		R_.push_back(0);
		D_.push_back(0);
	}

	void evolve(){
		while (E_.back()+I_.back() != 0){
			evolveStep();
		}
		awareness_.reset();
	}

	bool outbreakHappened(){
		return outbreak_;
	}

};

#endif