#ifndef ENSEMBLERCLASSIFIER_HPP
#define ENSEMBLERCLASSIFIER_HPP

#include "libraries.hpp"
using namespace std;

class Ensembler {
private:
	int linearClassifiers;
	string validationAdr;
	string weightVectorsAdr;
	double accuracy;
	vector <string> classifiersDirections;
	vector <int> labels;
	vector <int> voterLabels;
public:
	Ensembler(char* argv1, char* argv2);
	void makeClassifiersDirections();
	void forkLinearClassifiers();
	void forkVoter();
	void calculateAccuracy();
	void fetchData();
	double measureAccuracy();
};

#endif