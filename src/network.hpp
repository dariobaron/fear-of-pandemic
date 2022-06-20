#ifndef network_h
#define network_h

#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>
#include <functional>
#include <numeric>
#include "global.hpp"
#include "node.hpp"
#include "random.hpp"

using namespace std;

class Network{

protected:
	vector<Node*> nodes_;
	int N_nodes_;

private:
	map<int,unsigned int> ID_to_idx_;

public:
	Network(const string inputfile, const string format="adjlist") : nodes_(0), N_nodes_(0) {
		ifstream input(inputfile);
		if (input.is_open()){
			if (format == "adjlist"){
// skipping the first three rows of comments
				for (auto i = 0; i < 3; ++i){
					input.ignore(numeric_limits<streamsize>::max(), '\n');
				}
// reading the whole file storing in a vector of strings (lines)
// N.B. BE AWARE OF BIG FILES: they are fully loaded, and they must fit in RAM
				vector<string> lines;					// file content
				string line;							// temporary string to use getline
				int source;								// temporary string to use getline
				while (!input.eof()){
					getline(input, line);				// reading a line
					stringstream ss(line);				// using stringstream to read the values
// storing the first integer as node ID and creating the node
					if (ss >> source){					// check for the good input
						lines.push_back(line);			// storing the lines in RAM
						ID_to_idx_[source] = N_nodes_;	// N_nodes_ is not yet updated, so it is the index to the correct position
						++N_nodes_;						// N_nodes_ update
						nodes_.push_back(new Node(source));
					}
				}
// creating the edges
				for (auto & line : lines){
					stringstream ss(line);				// using stringstream to read the values
					int source, target;
					ss >> source;
					while (ss >> target || !ss.eof()){
						nodes_[ID_to_idx_[source]]->addConnection(target);
						nodes_[ID_to_idx_[target]]->addConnection(source);
					}
				}
			}
// to be implemented
			else if (format == "edgelist"){
			}
			input.close();
		}
		else{ throw ios_base::failure("File not found: " + inputfile); }
	};

	Network(const int N) : nodes_(N), N_nodes_(N) {
		for (auto i = 0; i < N; ++i){
			nodes_[i] = new Node(i);
			ID_to_idx_[i] = i;
		}
	}

	~Network(){
		for (auto n : nodes_){
			delete n;
		}
		nodes_.clear();
	};

	template<typename Distribution>
	void initEdgesBianconiBarabasi(Distribution & fitness_distr, int edges_per_node){
// utility vectors for the probability of new link
		vector<int> degree(N_nodes_, edges_per_node);
		vector<double> fitness(N_nodes_);
		for (auto i = 0; i < N_nodes_; ++i){
			fitness[i] = fitness_distr(random_engine);
			nodes_[i]->fitness(fitness[i]);
		}
// building the initial fully-connected core
		for (auto i = 0; i < edges_per_node + 1; ++i){
			for (auto j = i + 1; j < edges_per_node + 1; ++j){
				nodes_[i]->addConnection(nodes_[j]->id());
				nodes_[j]->addConnection(nodes_[i]->id());
			}
		}
		vector<double> probability(N_nodes_, 0);
		transform(degree.begin(), degree.begin()+edges_per_node+1, fitness.begin(), probability.begin(), multiplies<double>());
// building the rest of the network, node by node
		for (auto i = edges_per_node + 1; i < N_nodes_; ++i){
			vector<Node*> elected = randomChoice(vector<Node*>(nodes_.begin(), nodes_.begin()+i), edges_per_node, probability);
			for (auto n : elected){
				int n_id = ID_to_idx_[n->id()];
				++degree[n_id];
				probability[n_id] += fitness[n_id];
				n->addConnection(nodes_[i]->id());
				nodes_[i]->addConnection(n->id());
			}
			degree[i] += edges_per_node;
			probability[i] += edges_per_node * fitness[i];
			cout << "Processing... " << 100*i/N_nodes_ << "%\r" << flush;
		}
	}
	
	vector<Node*> getNodes() const{ return nodes_; };

	bool checkIdIntegrity() const{
		for (auto & p : ID_to_idx_){
			if (p.first != p.second){	return false;	}
		}
		return true;
	};

	void writeAdjlist(const string outputfile) const{
		ofstream output(outputfile);
		if (output.is_open()){
			// three (empty) comment lines
			output << "#\n#\n#";
			// nodes output
			for (auto node : nodes_){
				output << "\n" << node->id();
				auto connections = node->connections();
				for (auto & p : connections){
					if (p.first > node->id()){
						output << " " << p.first;
					}
				}
			}
			output.close();
		}
		else{ throw ios_base::failure("Impossible to open: " + outputfile); }
	};

};

#endif