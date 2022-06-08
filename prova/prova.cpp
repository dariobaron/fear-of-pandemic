#include <iostream>
#include <vector>

using namespace std;

class Prova{
public:
	Prova(int a) : id(a){};
	int getId(){	return id;	};
protected:
	int id;
};


int operator<<(ostream & os, Prova & m){
	return m.getId();
}


int main(int argc, char const *argv[]){
	
	vector<Prova*> v(100);
	//vector<int*> v(100);
	for(auto i = 0; i < 100; ++i){
		v[i] = new Prova(i);
	}

	cout << "Size of vector : " << v.size() << endl;
	cout << "First element ID : ";
	cout << v[0]->getId() << endl;
	cout << "Secon element ID : ";
	cout << v[1]->getId() << endl;


return 0;
}