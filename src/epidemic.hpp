#ifndef epidemic_h
#define epidemic_h

#include <vector>
#include <unordered_set>
#include "global.hpp"
#include "node.hpp"
#include "discretizer.hpp"
#include "random.hpp"

class Epidemic{

protected:
	vector<Node*> nodes_;
	vector<unsigned int> S_;
	vector<unsigned int> E_;
	vector<unsigned int> I_;
	vector<unsigned int> R_;
	vector<unsigned int> D_;
	vector<double> t_;
	double latency_rate;
	double transmission_rate;
	double recovery_rate;
	double fatality_probability;


private:
	Discretizer time_gen_;
	unordered_set<Node*> exposeds;
	unordered_set<Node*> infecteds;

	void updateMetrics(const vector<unsigned int> & metrics_at_step){
		S_.push_back(metrics_at_step[0]);
		E_.push_back(metrics_at_step[1]);
		I_.push_back(metrics_at_step[2]);
		R_.push_back(metrics_at_step[3]);
		D_.push_back(metrics_at_step[4]);
	};

	void evolveStep(){
// getting new timestamp
		t_.push_back(time_gen_());
		logstream << "\n\ntime : " << t_.back() << endl; //////////////////////////
// loop over the infecteds
		for (auto node : infecteds){
			node->infect(transmission_rate);
			node->recover(recovery_rate, fatality_probability);
		}
// loop over the exposed
		for (auto node : exposeds){
			node->incubate(latency_rate);
		}
// collecting new statuses
		vector<unsigned int> SEIRD(5, 0);
		exposeds.clear();
		infecteds.clear();
		for (auto node : nodes_){
			node->updateStatus();
			SEIRD[node->status()]++;
			if (node->status() == 1){
				exposeds.insert(node);
			}
			else if (node->status() == 2){
				infecteds.insert(node);
			}
		}
		logstream << "SEIRD :"; //////////////////////////
		for (auto i : SEIRD){ //////////////////////////
			logstream << " " << i; //////////////////////////
		} //////////////////////////
		logstream << endl; //////////////////////////
		logstream << "infecteds :"; //////////////////////////
		for (auto i : infecteds){ //////////////////////////
			logstream << " " << i->id() << "(" << i->status() << ")"; //////////////////////////
		} //////////////////////////
		logstream << endl; //////////////////////////
		logstream << "exposeds :"; //////////////////////////
		for (auto i : exposeds){ //////////////////////////
			logstream << " " << i->id() << "(" << i->status() << ")"; //////////////////////////
		} //////////////////////////
		logstream << endl; //////////////////////////
		updateMetrics(SEIRD);
	};

public:
	Epidemic(vector<Node*> v, Discretizer & time_gen,
		double mu, double beta, double gamma, double f_D) : nodes_(v),
	S_(0), E_(0), I_(0), R_(0), D_(0),
	latency_rate(mu), transmission_rate(beta), recovery_rate(gamma), fatality_probability(f_D),
	time_gen_(time_gen), exposeds(), infecteds() {};

	~Epidemic() {};

	vector<double> gett() const{	return t_;	};
	vector<unsigned int> getS() const{	return S_;	};
	vector<unsigned int> getE() const{	return E_;	};
	vector<unsigned int> getI() const{	return I_;	};
	vector<unsigned int> getR() const{	return R_;	};
	vector<unsigned int> getD() const{	return D_;	};

	void seedEpidemic(unsigned int N_initial_infectiouses){
// selecting N_initial_infectiouses nodes at random		
		vector<Node*> initial_infecteds = randomChoice(nodes_, N_initial_infectiouses);
		for (auto & n : initial_infecteds){
			n->status(2);
			n->updateStatus();
			infecteds.insert(n);
		}
// updating the metrics vectors
		t_.push_back(time_gen_.get());
		S_.push_back(nodes_.size() - N_initial_infectiouses);
		E_.push_back(0);
		I_.push_back(N_initial_infectiouses);
		R_.push_back(0);
		D_.push_back(0);
		logstream << "\n\ntime : " << t_.back() << "\tSEIRD : ";
		logstream << S_.back() << " " << E_.back() << " " << I_.back() << " " << R_.back() << " " << D_.back() << endl;
		logstream << "infecteds :";
		for (auto n : infecteds){
			logstream << " " << n->id() << "(" << n->status() << ")";
		}
		logstream << endl;
		logstream << "exposeds :";
		for (auto n : exposeds){
			logstream << " " << n->id() << "(" << n->status() << ")";
		}
		logstream << endl;
	}

	void evolve(){
		while (E_.back()+I_.back() != 0){
			evolveStep();
		}
	}

};

#endif