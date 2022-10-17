#ifndef node_h
#define node_h

#include <map>
#include <utility>
#include "global.hpp"

using namespace std;

class Node{

protected:
	int id_;
	short int status_;
	int degree_;
	double fitness_;
	map<Node*,double> connections_;
	double fear_;
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
	
	void addConnection(Node* target, double weight=1){
		connections_.insert(pair<Node*,double>(target, weight));
		++degree_;
	};
	void changeConnection(Node* target, double weight){	connections_[target] = weight;	};
	void changeAllConnection(double weight){
		for (auto & p : connections_){
			p.second = weight;
		}
	};
	void removeConnection(Node* target){
		connections_.erase(target);
		--degree_;
	};

	double fear() const{	return fear_;	};
	void fear(double f){	fear_ = f;	};

	void updateStatus(){	status_ = new_status_; };

	map<Node*,double> connections() const{	return connections_;	};

	bool incubate(double latency_rate){
		if (status_ != 1){
			cerr << "Error: node " << this << "(" << id_ << ")" << " incubated while not exposed. Current status is " << status_ << endl;
		}
		bool incubated = uniform_distribution() <= latency_rate;
		if (incubated){
			new_status_ = 2;
			cout << id_ << " has incubated" << endl;
		}
		return incubated;
	}
	bool recover(double recovery_rate, double fatality_probability){
		if (status_ != 2){
			cerr << "Error: node " << this << "(" << id_ << ")" << " recovered while not infected. Current status is " << status_ << endl;
		}
		bool recovered = uniform_distribution() <= recovery_rate;
		if (recovered){
			if (uniform_distribution() <= fatality_probability){
				new_status_ = 4;
				cout << id_ << " has dead" << endl;
			}
			else{
				new_status_ = 3;
				cout << id_ << " has recovered" << endl;
			}
		}
		return recovered;
	}
	void infect(double transmission_rate) const{
		if (status_ != 2){
			cerr << "Error: node " << this << "(" << id_ << ")" << " infecting while not infected. Current status is " << status_ << endl;
		}
		cout << id_ << " infecting : ";
		for (auto p : connections_){
			if (p.first->status() == 0){
				if (uniform_distribution() <= p.second*transmission_rate*(1-fear_)){
					cout << p.first->id() << " ";
					p.first->status(1);
				}
			}
		}
		cout << endl;
	}
	
};

#endif