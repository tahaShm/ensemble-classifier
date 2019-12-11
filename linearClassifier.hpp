#ifndef LINEARCLASSIFIER_HPP
#define LINEARCLASSIFIER_HPP

#include "libraries.hpp"
using namespace std;

class Linear {
private:
    int classifierId;
    char* datasetAdr;
    char* weightVectorAdr;
    vector < vector<double> > weights; //each block consists of a double[3] -> Betha_0, Betha_1, Bias
    vector < vector<double> > data; //each block consists of a double[2] -> width, height
    vector < vector<double> > products;
    vector <int> classes;
public:
    Linear(char* datasetAdr, char* weightVectorAdr, char* id);
    void setWeights();
    void setData();
    double calculateInnerProduct(vector <double> first, vector <double> second);
    void calculateProducts();
    int findClass(vector <double> curProducts);
    void calculateClasses();
    string convertClassesToString();
    void sendClasses();
};

#endif