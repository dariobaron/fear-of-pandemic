#ifndef network_h
#define network_h

#include "global.hpp"
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
using namespace std;

template<typename Node>
class Network{

protected:
	vector<Node*> nodes_;
	int N_nodes_;

public:
	Network(const string inputfile, const string format="adjlist"){
		ifstream input(inputfile);
		if (input.is_open()) {
			if (format == "adjlist"){
				// skippare le prime 3 righe di commento
				// numero di righe totali per allocare il vettore dei nodi
				// settare ogni nodo con nome (primo elemento della riga) e edges (altri elementi della riga)
			}
			else if (format == "edgelist"){
				// da implementare
			}
			input.close();
		}
		else{ throw ios_base::failure("File not found: " + inputfile); }
	};

	Network(const int N) : nodes_(N), N_nodes_(N) {
		for (auto i = 0; i < N; ++i){
			nodes_[i] = new Node(i)
		}
	}

	~Network(){
		for (auto n : nodes_){
			delete n;
		}
		nodes.clear()
	};

	template<typename Distribution>
	void initEdgesBianconiBarabasi(Distribution & fitness_distr, int edges_per_node){
		// initialization of network features vectors
		vector<int> id(N_nodes_);
		for (int i = 0; i < N_nodes_; ++i){
			id[i] = nodes_[i]->id();
		}
		//
		vector<int> degree(N_nodes_, 0);
		//
		vector<double> fitness(N_nodes_);
		for (auto & f : fitness){
			f = fitness_distr(random_engine);
		}


		for (int i = 0; i < N_nodes_; ++i){

		}
	}
	
	vector<Node*> getNodes() { return nodes_; };

};

#endif