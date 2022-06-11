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
				getline(input, line);
				while (!input.eof()){
					lines.push_back(line);
					stringstream ss(line);				// reading each line content as stringstream
// storing the first integer as node ID and creating the node
					int source;
					ss >> source;
					++N_nodes_;
					nodes_.push_back(new Node(source));
					ID_to_idx_[source] = N_nodes_ - 1;
					getline(input, line);
				}
// creating the edges
				for (auto & line : lines){
					stringstream ss(line);				// reading each line content as stringstream
					int source, target;
					ss >> source;
					while (!ss.eof()){
						ss >> target;
						nodes_[ID_to_idx_[source]]->addConnection(ID_to_idx_[target]);
						nodes_[ID_to_idx_[target]]->addConnection(ID_to_idx_[source]);
					}
				}
			}
			else if (format == "edgelist"){
// to be implemented
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
		vector<int> degree(N_nodes_, 0);
		vector<double> fitness(N_nodes_);
		for (auto & f : fitness){
			f = fitness_distr(random_engine);
		}

// building the initial fully-connected core
		for (auto i = 0; i < edges_per_node + 1; ++i){
			for (auto j = i + 1; j < edges_per_node + 1; ++j){
				nodes_[i]->addConnection(ID_to_idx_[j]);
				nodes_[j]->addConnection(ID_to_idx_[i]);
			}
			degree[i] = edges_per_node - 1;
		}

// building the rest of the network, node by node
		for (auto i = edges_per_node + 1; i < N_nodes_; ++i){
			vector<double> probability(i);
			transform(degree.begin(), degree.begin()+i, fitness.begin(), probability.begin(), multiplies<double>());
			discrete_distribution<int> distr(probability.begin(), probability.end());
			for (auto j = 0; j < edges_per_node; ++j){
				auto target_idx = distr(random_engine);
				cout << i << " " << target_idx << endl;
				++degree[target_idx];
				nodes_[i]->addConnection(ID_to_idx_[target_idx]);
				nodes_[target_idx]->addConnection(ID_to_idx_[i]);
				probability[target_idx] = 0;
				distr = discrete_distribution<int>(probability.begin(), probability.end());
			}
		}

	};
	
	vector<Node*> getNodes() const{ return nodes_; };

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