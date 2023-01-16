#ifndef node_h
#define node_h

#include <set>
#include <utility>
#include "random.hpp"
#include "global.hpp"


class Awareness;


class Node{

protected:
	int id_;
	short int status_;
	int degree_;
	double fitness_;
	std::set<Node*> connections_;
	double fear_;

private:
	short int new_status_;

public:
	Node(int id) : id_(id), status_(), degree_(0),
	fitness_(), connections_(), fear_(), new_status_() {};

	~Node(){};

	int id() const{	return id_;	};

	short int status() const{	return status_;	};
	void status(short int s){	new_status_ = s;	};

	int degree() const{	return degree_;	}

	double fitness() const{	return fitness_;	};
	void fitness(double f){	fitness_ = f;	};
	
	void addConnection(Node* target){
		connections_.insert(target);
		++degree_;
	};
	void removeConnection(Node* target){
		connections_.erase(target);
		--degree_;
	};

	double fear() const{	return fear_;	};
	void fear(double f){	fear_ = f;	};

	void updateStatus(){	status_ = new_status_; };

	const std::set<Node*> & connections() const{	return connections_;	};
	void connections(std::set<Node*> connections) {	connections_ = connections;	};

	unsigned int neighboursInStatus(short int s){
		unsigned int counter = 0;
		for (auto n : connections_){
			if (n->status() == s){
				++counter;
			}
		}
		return counter;
	};

	bool incubate(double latency_rate){
		if (status_ != 1){
			std::cerr << "Error: node " << this << "(" << id_ << ")" << " incubated while not exposed. Current status is " << status_ << std::endl;
		}
		bool incubated = uniform_distribution() <= latency_rate;
		if (incubated){
			new_status_ = 2;
		}
		return incubated;
	}
	bool recover(double recovery_rate, double fatality_probability){
		if (status_ != 2){
			std::cerr << "Error: node " << this << "(" << id_ << ")" << " recovered while not infected. Current status is " << status_ << std::endl;
		}
		bool recovered = uniform_distribution() <= recovery_rate;
		if (recovered){
			if (uniform_distribution() <= fatality_probability){
				new_status_ = 4;
			}
			else{
				new_status_ = 3;
			}
		}
		return recovered;
	}
	void infect(double transmission_rate, std::map<Node*,double> & contacts) const{
		if (status_ != 2){
			std::cerr << "Error: node " << this << "(" << id_ << ")" << " infecting while not infected. Current status is " << status_ << std::endl;
		}
		for (auto c : contacts){
			if (c.first->status() == 0){
				if (uniform_distribution() < transmission_rate * c.second){
					c.first->status(1);
				}
			}
		}
	}
	
};

#endif