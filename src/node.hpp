#ifndef node_h
#define node_h

#include <map>
#include <utility>
using namespace std;

class Node{

protected:
	int id_;
	short int status_;
	int degree_;
	double fitness_;
	map<int,double> connections_;
	double fear_;
	short int new_status_;

public:
	Node(int id) : id_(id), degree_(0) {};

	~Node(){};

	int id() const{	return id_;	};

	short int status() const{	return status_;	};
	void status(short int s){	status_ = s;	};

	int degree() const{	return degree_;	}

	double fitness() const{	return fitness_;	};
	void fitness(double f){	fitness_ = f;	};
	
	void addConnection(int target, double weight=1){
		connections_.insert(pair<int,double>(target, weight));
		++degree_;
	};
	void changeConnection(int target, double weight){	connections_[target] = weight;	};
	void removeConnection(int target){
		connections_.erase(target);
		--degree_;
	};

	double fear() const{	return fear_;	};
	void fear(double f){	fear_ = f;	};

	void updateStatus(){	status_ = new_status_; };

	map<int,double> connections() const{	return connections_;	};
	
};

#endif