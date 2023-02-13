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


class Network{

protected:
	std::vector<Node*> nodes_;
	int N_nodes_;

private:
	std::map<int,unsigned int> ID_to_idx_;

public:
	Network(const std::string inputfile, const std::string format="adjlist") : nodes_(0), N_nodes_(0) {
		std::ifstream input(inputfile);
		if (input.is_open()){
			if (format == "adjlist"){
// skipping the first three rows of comments
				for (auto i = 0; i < 3; ++i){
					input.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				}
// reading the whole file storing in a vector of strings (lines)
// N.B. BE AWARE OF BIG FILES: they are fully loaded, and they must fit in RAM
				std::vector<std::string> lines;				// file content
				std::string line;							// temporary string to use getline
				int source;									// temporary string to use getline
				while (!input.eof()){
					std::getline(input, line);				// reading a line
					std::stringstream ss(line);				// using stringstream to read the values
// storing the first integer as node ID and creating the node
					if (ss >> source){						// check for the good input
						lines.push_back(line);				// storing the lines in RAM
						ID_to_idx_[source] = N_nodes_;		// N_nodes_ is not yet updated, so it is the index to the correct position
						++N_nodes_;							// N_nodes_ update
						nodes_.push_back(new Node(source));
					}
				}
// creating the edges
				for (auto & line : lines){
					std::stringstream ss(line);				// using stringstream to read the values
					int source, target;
					ss >> source;
					while (ss >> target || !ss.eof()){
						nodes_[ID_to_idx_[source]]->addConnection(nodes_[ID_to_idx_[target]]);
						nodes_[ID_to_idx_[target]]->addConnection(nodes_[ID_to_idx_[source]]);
					}
				}
			}
// to be implemented
			else if (format == "edgelist"){
			}
			input.close();
		}
		else{ throw std::ios_base::failure("File not found: " + inputfile); }
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
		std::vector<int> degree(N_nodes_, edges_per_node);
		std::vector<double> fitness(N_nodes_);
		for (auto i = 0; i < N_nodes_; ++i){
			fitness[i] = fitness_distr();
			nodes_[i]->fitness(fitness[i]);
		}
// building the initial fully-connected core
		for (auto i = 0; i < edges_per_node + 1; ++i){
			for (auto j = i + 1; j < edges_per_node + 1; ++j){
				nodes_[i]->addConnection(nodes_[j]);
				nodes_[j]->addConnection(nodes_[i]);
			}
		}
		std::vector<double> probability(N_nodes_, 0);
		std::transform(degree.begin(), degree.begin()+edges_per_node+1, fitness.begin(), probability.begin(), std::multiplies<double>());
// building the rest of the network, node by node
		for (auto i = edges_per_node + 1; i < N_nodes_; ++i){
			std::vector<Node*> elected = randomChoice(std::vector<Node*>(nodes_.begin(), nodes_.begin()+i), edges_per_node, probability);
			for (auto n : elected){
				int n_id = ID_to_idx_[n->id()];
				++degree[n_id];
				probability[n_id] += fitness[n_id];
				n->addConnection(nodes_[i]);
				nodes_[i]->addConnection(n);
			}
			degree[i] += edges_per_node;
			probability[i] += edges_per_node * fitness[i];
			std::cout << "Processing... " << 100*i/N_nodes_ << "%\r" << std::flush;
		}
// extending the initial fully-connected core to add the links not added in the initial phase
		for (auto i = 1; i < edges_per_node + 1; ++i){
			unsigned added = 0;
			while (added < i){
				Node * candidate = nodes_[uniform_distribution() * N_nodes_];
				if (!nodes_[i]->isInContact(candidate)){
					candidate->addConnection(nodes_[i]);
					nodes_[i]->addConnection(candidate);
					++added;
				}
			}
		}
	}
	
	std::vector<Node*> getNodes() const{ return nodes_; };

	double getMeanDegree() const{
		int total_degree = 0;
		for (auto node : nodes_){
			total_degree += node->degree();
		}
		return 1. * total_degree / N_nodes_;
	};

	int size() const{
		return nodes_.size();
	}

	bool checkIdIntegrity() const{
		for (auto & p : ID_to_idx_){
			if (p.first != p.second){	return false;	}
		}
		return true;
	};

	void writeAdjlist(const std::string outputfile) const{
		std::ofstream output(outputfile);
		if (output.is_open()){
			// three (empty) comment lines
			output << "#\n#\n#";
			// nodes output
			for (auto node : nodes_){
				output << "\n" << node->id();
				auto connections = node->connections();
				for (auto & p : connections){
					if (p->id() > node->id()){
						output << " " << p->id();
					}
				}
			}
			output.close();
		}
		else{ throw std::ios_base::failure("Impossible to open: " + outputfile); }
	};

};

#endif