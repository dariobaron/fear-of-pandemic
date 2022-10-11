#ifndef epidemic_h
#define epidemic_h

#include <vector>
#include <set>
#include "global.hpp"
#include "node.hpp"
#include "discretizer.hpp"
#include "random.hpp"

class Epidemic{

protected:
	vector<Node*> nodes_;
	vector<int> S_;
	vector<int> E_;
	vector<int> I_;
	vector<int> R_;
	vector<int> D_;
	vector<double> t_;

private:
	Discretizer time_gen_;
	set<Node*> exposeds;
	set<Node*> infecteds;

public:
	Epidemic(vector<Node*> v, Discretizer & time_gen) : nodes_(v),
	S_(0), E_(0), I_(0), R_(0), D_(0),
	time_gen_(time_gen), exposeds(), infecteds() {};

	~Epidemic() {};

	vector<double> gett() const{	return t_;	};
	vector<int> getS() const{	return S_;	};
	vector<int> getE() const{	return E_;	};
	vector<int> getI() const{	return I_;	};
	vector<int> getR() const{	return R_;	};
	vector<int> getD() const{	return D_;	};

	void seedEpidemic(int N_initial_infectiouses){
// selecting N_initial_infectiouses nodes at random		
		vector<Node*> initial_infecteds = randomChoice(nodes_, N_initial_infectiouses);
		for (auto & n : initial_infecteds){
			n->status(2);
			n->updateStatus();
			infecteds.insert(n);
		}
// updating the metrics vectors
		S_.push_back(nodes_.size() - N_initial_infectiouses);
		E_.push_back(0);
		I_.push_back(N_initial_infectiouses);
		R_.push_back(0);
		D_.push_back(0);
	}

	void updateMetrics(const vector<int> & metrics_at_step){
		S_.push_back(metrics_at_step[0]);
		E_.push_back(metrics_at_step[1]);
		I_.push_back(metrics_at_step[2]);
		R_.push_back(metrics_at_step[3]);
		D_.push_back(metrics_at_step[4]);
	};

	void evolveStep(){
// getting new timestamp
		t_.push_back(time_gen_());

// loop over the infecteds
		for (auto node : infecteds){
			node->infect();
			if (node->recover()){
				infecteds.erase(node);
			}
		}

// loop over the exposed
		for (auto node : exposeds){
			if (node->incubate()){
				exposeds.erase(node);
				infecteds.insert(node);
			}
		}

// collecting new statuses
		vector<int> SEIRD(5, 0);
		for (auto node : nodes_){
			node->updateStatus();
			SEIRD[node->status()]++;
			if (node->status() == 1){
				exposeds.insert(node);
			}
		}
		updateMetrics(SEIRD);
	}

};

#endif