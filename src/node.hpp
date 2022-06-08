#ifndef node_h
#define node_h

#include <map>
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

	int id(){	return id_;	};

	short int status(){	return status_;	};
	void status(short int s){	status_ = s;	};

	int degree(){	return degree_;	}

	double fitness(){	return fitness_;	};
	void fitness(double f){	fitness_ = f;	};
	
	void addConnection(int target, double weight=1){
		connections_.insert(target, weight);
		++degree_;
	};
	void changeConnection(int target, double weight){	connections_[target] = weight;	};
	void removeConnection(int target){
		connections_.erase(target);
		--degree_;
	};

	double fear(){	return fear;	};
	void fear(double f){	fear_ = f;	};

	void updateStatus(){	status_ = new_status_; };
	
};

#endif