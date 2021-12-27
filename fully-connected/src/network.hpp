#ifndef network_h
#define network_h

#include 
#include <string>
#include <fstream>
#include <vector>
#include <utility>
using namespace std;

template<typename Node>
class Network{

public:
	Network(const string inputfile){
		ifstream input(inputfile);
		if (input.is_open()) {
			// skippare le prime 3 righe di commento
			// numero di righe totali per allocare il vettore dei nodi
			// settare ogni nodo con nome (primo elemento della riga) e edges (altri elementi della riga)
			input.close();
		}
		else{ throw ios_base::failure("File not found: " + inputfile); }
	};

	~Network(){
		for (auto n : nodes){
			delete n;
		}
		nodes.clear()
	};
	
	vector<Node*> getNodes() { return nodes; };
	
	vector<Node*> evolve();

protected:
	vector<Node*> nodes;
};

#endif