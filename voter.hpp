#ifndef VOTER_HPP
#define VOTER_HPP

#include "libraries.hpp"

using namespace std;

class Voter {
private:
	int linearClassifiers;
	vector < vector<int> > classes;
    vector <int> trueClasses;
public:
	Voter(char* linearClassifiers);
    void fetchClasses();
    int findTrueClass(int index);
    void determineTrueClasses();
    string convertClassesToString();
    void sendToEnsemble();
};

#endif