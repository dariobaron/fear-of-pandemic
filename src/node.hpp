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

	bool incubate(){

	}
	bool recover(){
		
	}
	vector<Node*> infect() const{
		
	}
	
};

#endif