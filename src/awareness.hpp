#ifndef awareness_h
#define awareness_h

#include <map>
#include <vector>
#include <algorithm>
#include <numeric>
#include "node.hpp"
#include "constants.hpp"


class Awareness{

public:
	using uvec = std::vector<unsigned>;

protected:
	const bool cut_connections_;

public:

	Awareness(bool cut_connections) : cut_connections_(cut_connections) {};

	virtual void setGlobalMetric(const uvec & I, const uvec & R, const uvec & D) = 0;
	
	std::map<Node*,double> computeContacts(Node * node){
		std::map<Node*,double> contacts;
		for (auto neighbour : node->connections()){
			if (cut_connections_){
				if (uniform_distribution() < 1./(1+std::max(node->fear()*feedback(node),neighbour->fear()*feedback(neighbour)))){
					contacts[neighbour] = 1.;
				}
				else{
					contacts[neighbour] = 0.;
				}
			}
			else{
				contacts[neighbour] = 1. / (1 + std::max(node->fear()*feedback(node), neighbour->fear()*feedback(neighbour)));
			}
			
		}
		return contacts;
	};

	virtual double feedback(Node * node) = 0;

	virtual void reset() = 0;

};



class NoAwareness : public Awareness{

public:
	using uvec = std::vector<unsigned>;

public:
	NoAwareness() : Awareness(false) {};
	
	void setGlobalMetric(const uvec &, const uvec &, const uvec &){};

	double feedback(Node *){
		return 0;
	};

	void reset(){};

};



class ShortAwareness : public Awareness{

public:
	using uvec = std::vector<unsigned>;

protected:
	const double delta_s_;

private:
	double Ddot_;

public:
	ShortAwareness(double delta_s, bool cut_connections)
		: Awareness(cut_connections), delta_s_(delta_s) {};
	
	void setGlobalMetric(const uvec &, const uvec & R, const uvec & D){
		Ddot_ = (R.back() + D.back()) * f_D;
	};

	double feedback(Node *){
		return Ddot_ / delta_s_;
	};

	void reset(){};

};



class LongAwareness : public Awareness{

public:
	using uvec = std::vector<unsigned>;

protected:
	const double delta_l_;

private:
	int Dtot_;

public:
	LongAwareness(double delta_l, bool cut_connections)
		: Awareness(cut_connections), delta_l_(delta_l) {};
	
	void setGlobalMetric(const uvec &, const uvec & R, const uvec & D){
		Dtot_ = (accumulate(R.begin(), R.end(), 0) + accumulate(D.begin(), D.end(), 0)) * f_D;
	};

	double feedback(Node *){
		return Dtot_ / delta_l_;
	};

	void reset(){};

};



class ShortLongAwareness : public Awareness{

public:
	using uvec = std::vector<unsigned>;

protected:
	const double delta_s_;
	const double delta_l_;

private:
	double Ddot_;
	int Dtot_;

public:
	ShortLongAwareness(double delta_s, double delta_l, bool cut_connections)
		: Awareness(cut_connections), delta_s_(delta_s), delta_l_(delta_l) {};
	
	void setGlobalMetric(const uvec &, const uvec & R, const uvec & D){
		Ddot_ = (R.back() + D.back()) * f_D;
		Dtot_ = (accumulate(R.begin(), R.end(), 0) + accumulate(D.begin(), D.end(), 0)) * f_D;
	};

	double feedback(Node *){
		double short_feedback = Ddot_ / delta_s_;
		double long_feedback = Dtot_ / delta_l_;
		return short_feedback + long_feedback;
	};

	void reset(){};

};



class NeighbourAwareness : public Awareness{

public:
	using uvec = std::vector<unsigned>;

protected:
	const double delta_nI_;
	const double delta_nD_;

public:
	NeighbourAwareness(double delta_nI, double delta_nD, bool cut_connections)
		: Awareness(cut_connections), delta_nI_(delta_nI), delta_nD_(delta_nD) {};
	
	void setGlobalMetric(const uvec &, const uvec &, const uvec &){};

	double feedback(Node * node){
		int neigh_infected = 0;
		int neigh_dead = 0;
		for (auto neighbour : node->connections()){
			if 		(neighbour->status() == 2){	++neigh_infected;	}
			else if	(neighbour->status() == 4){	++neigh_dead;		}
		}
		return 1. / node->connections().size() * (neigh_infected / delta_nI_ + neigh_dead / delta_nD_);
	};

	void reset(){};

};



class AllAwareness : public Awareness{

public:
	using uvec = std::vector<unsigned>;

protected:
	const double delta_s_;
	const double delta_l_;
	const double delta_nI_;
	const double delta_nD_;

private:
	double Ddot_;
	int Dtot_;

public:
	AllAwareness(double delta_s, double delta_l, double delta_nI, double delta_nD, bool cut_connections)
		: Awareness(cut_connections), delta_s_(delta_s), delta_l_(delta_l),
		delta_nI_(delta_nI), delta_nD_(delta_nD) {};
	
	void setGlobalMetric(const uvec &, const uvec & R, const uvec & D){
		Ddot_ =  (R.back() + D.back()) * f_D;
		Dtot_ = (accumulate(R.begin(), R.end(), 0) + accumulate(D.begin(), D.end(), 0)) * f_D;
	};

	double feedback(Node * node){

		double short_feedback = Ddot_ / delta_s_;
		
		double long_feedback = Dtot_ / delta_l_;

		int neigh_infected = 0;
		int neigh_dead = 0;
		for (auto neighbour : node->connections()){
			if 		(neighbour->status() == 2){	++neigh_infected;	}
			else if	(neighbour->status() == 4){	++neigh_dead;		}
		}
		double neighbour_feedback = 1. / node->connections().size() * (neigh_infected / delta_nI_ + neigh_dead / delta_nD_);

		return short_feedback + long_feedback + neighbour_feedback;
	};

	void reset(){};

};


#endif