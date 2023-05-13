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

private:
	std::map<Node*,double> feedbacks_;

public:

	Awareness(bool cut_connections) : cut_connections_(cut_connections) {};

	virtual void setGlobalMetric(const uvec & I, const uvec & R, const uvec & D) = 0;

	void computeFeedbacks(std::vector<Node*> & nodes){
		for (auto node : nodes){
			feedbacks_[node] = feedback(node);
		}
	};
	
	std::map<Node*,double> computeContacts(Node * node){
		std::map<Node*,double> contacts;
		for (auto neighbour : node->connections()){
			contacts[neighbour] = 1. / (1 + std::max( node->fear()*feedbacks_[node], neighbour->fear()*feedbacks_[neighbour] ));
		}
		if (cut_connections_){
			// compute the number of contacts
			double N_contacts = std::accumulate(
				contacts.begin(), contacts.end(), 0.,
				[](const double sum, const auto & element){ return sum + element.second; }
			);
			// order the contacts by decreasing intensity
			std::multimap<double,Node*,std::greater<>> ordered_contacts;
			for (auto & p : contacts){
				ordered_contacts.insert(std::make_pair(p.second, p.first));
			}
			// change the contact intensity according to the rule:
			// fill the "floor(N_contacts)" most intense contacts with 1 intensity
			// fill the "N_contacts-ceil(N_contacts)" less intense contacts with 0 intensity
			// fill the "ceil(N_contacts)"-th most intense contact with the remainder intensity
			double used = 0;
			for (auto & p : ordered_contacts){
				if		(used + p.first < std::floor(N_contacts))	{	contacts[p.second] = 1;					used += 1;			}
				else if	(used + p.first < std::ceil(N_contacts))	{	contacts[p.second] = N_contacts - used;	used = N_contacts;	}
				else												{	contacts[p.second] = 0;										}
			}
		}
		return contacts;
	};

	virtual double feedback(Node * node) = 0;

	virtual void reset(){
		feedbacks_.clear();
	};

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
		Ddot_ = (R.back() + D.back() - R.rbegin()[1] - D.rbegin()[1]) * f_D;
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
		Dtot_ = (R.back() + D.back()) * f_D;
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
		Ddot_ = (R.back() + D.back() - R.rbegin()[1] - D.rbegin()[1]) * f_D;
		Dtot_ = (R.back() + D.back()) * f_D;
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
		Ddot_ = (R.back() + D.back() - R.rbegin()[1] - D.rbegin()[1]) * f_D;
		Dtot_ = (R.back() + D.back()) * f_D;
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