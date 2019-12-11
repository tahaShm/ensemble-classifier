#include <iostream>
#include <unistd.h>
#include "voter.hpp"

using namespace std;

int main(int argc, char* argv[]){
    Voter voter(argv[0]);
    voter.fetchClasses();
    voter.determineTrueClasses();
    voter.sendToEnsemble();
}