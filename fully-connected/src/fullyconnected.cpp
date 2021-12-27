/*************
 * PROGRAM TO GENERATE A FULLY-CONNECTED NETWORK
 * 
 * The produced network is saved into the input folder to
 * be used by the main program.
 * 
 * **********/

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

ofstream output;

int main(){

	// setup network structure
	cout << "Insert the number of nodes wanted" << endl;
	int N;
	cin >> N;

	// setting filename and path
	string outputfile = "../input/fully_" + to_string(N) + ".adjlist";
	cout << "Saving the network as adjlist to: " << outputfile << endl;

	// writing to file
	{
		output.open(outputfile);
	
		// headers
		output << "#" << endl << "#" << endl << "#" << endl;
	
		// actual network structure
		for (int i = 0; i < N; ++i){
			output << i;
	
			for (int j = i + 1; j < N; ++j){
				output << " " << j;
			}
	
			output << endl;
		}
	
		output.close();
		output.clear();
	}


	return 0;
}