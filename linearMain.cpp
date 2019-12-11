#include <iostream>
#include <unistd.h>
#include "linearClassifier.hpp"

using namespace std;

int main(int argc, char* argv[]){
	Linear linear(argv[0], argv[1], argv[2]);
	linear.setData();
	linear.setWeights();
	linear.calculateClasses();
	linear.sendClasses();
	return 0;
}