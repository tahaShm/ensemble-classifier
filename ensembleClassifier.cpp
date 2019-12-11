#include "ensembleClassifier.hpp"

using namespace std;
string charToString(char* input, int size) {
	string output = "";
	for (int i = 0; i < size; i++)
		output += input[i];
	return output;
}
Ensembler::Ensembler(char* argv1, char* argv2){
	this->validationAdr = charToString(argv1, strlen(argv1));
	this->weightVectorsAdr = charToString(argv2, strlen(argv2));
	this->accuracy = 0.0;
	this->linearClassifiers = 0;
}

void Ensembler::makeClassifiersDirections(){
	DIR *direction;
	struct dirent *dent;
	if ((direction = opendir (weightVectorsAdr.c_str())) != NULL) {
		while ((dent = readdir (direction)) != NULL) {
			if (strcmp(dent->d_name,".") && strcmp(dent->d_name,"..")){
				string newDirection = weightVectorsAdr + '/' + CLASSIFIER + to_string(linearClassifiers) + ".csv";
				classifiersDirections.push_back(newDirection);
				linearClassifiers++;
			}
		}
	} else {
        cerr << "opening directory failed!\n";
        exit(1);
    }
}

void Ensembler::forkLinearClassifiers() {
	int p[linearClassifiers][2];
    vector<int> pids;
	for (int i = 0; i < linearClassifiers; i++){
        if (pipe(p[i])== -1){
            perror("pipe() creation failed!");
            exit(1);
        }
        int pid = fork();
        if(pid > 0){
            pids.push_back(pid);
            close(p[i][0]);
            write(p[i][1], classifiersDirections[i].c_str() , PATHSIZE);
            write(p[i][1], (validationAdr + "/" + DATASET).c_str() , PATHSIZE);
            write(p[i][1], to_string(i).c_str() , 3);
            close(p[i][1]);
        }
        else if (pid == 0){
            close(p[i][1]);
            char path[PATHSIZE];
            char dataSetPath[PATHSIZE];
            char id[3]; 
            read(p[i][0], path, PATHSIZE);
            read(p[i][0], dataSetPath, PATHSIZE);
            read(p[i][0], id, 3);
            close(p[i][0]);
            char* argv[] = {dataSetPath, path, id, NULL};
            execv("./linearClassifier", argv);
            exit(0);
        }
        else {
        	cerr << "fork() creation failed! \n";
        }
    }
    int pidSize = pids.size();
    for (int i = 0; i < pidSize; i++){
        int status;
        waitpid(pids[i], &status, 0);
    }
}

void Ensembler::forkVoter(){
	int p[2];
    int voterPid;
    if (pipe(p)== -1){
        perror("pipe() creation failed!");
        exit(1);
    }
    int pid = fork();
    if(pid > 0){
        voterPid = pid;
        close(p[0]);
        write(p[1], to_string(linearClassifiers).c_str() , 3);
        close(p[1]);
    }
    else if (pid == 0){
        close(p[1]);
        char classifiers[3]; 
        read(p[0], classifiers, 3);
        string test = classifiers;
        close(p[0]);
        char* argv[] = {classifiers, NULL};
        execv("./voter", argv);
        exit(0);
    }
    else {
    	cerr << "fork() creation for voter failed! \n";
    }
    int status;
    waitpid(voterPid, &status, 0);
}

void Ensembler::fetchData(){
    string voterRoute = VOTER;
    fstream fin; 
    bool isEmpty = true;
    bool firstLine = true;
    fin.open(voterRoute.c_str(), ios::in); 
    string line; 
    while (fin >> line) {
        isEmpty = false;
        int currentVal = atoi(line.c_str());
        voterLabels.push_back(currentVal);
    }
    fin.close();
    if (isEmpty){
        cout << "voter_ file not found" << endl;
        exit(1);
    }

    string labelRoute = "./" + validationAdr + "/" + LABELS;
    isEmpty = true;
    fin.open(labelRoute.c_str(), ios::in); 
    line = ""; 
    while (fin >> line) { 
            isEmpty = false;
            int currentVal = atoi(line.c_str());
            labels.push_back(currentVal);
    }
    fin.close();
    if (isEmpty){
        cout << "label file not found" << endl;
        exit(1);
    }
}

double Ensembler::measureAccuracy() {
    int size = voterLabels.size();
    int correspondences = 0;
    for (int i = 0; i < size; i++) {
        // cout << i+1 << " : " << voterLabels[i] << endl;
        if (labels[i+2] == voterLabels[i])
            correspondences++;
    }
    double ans = double(correspondences) / double(size) * 100;

}

int main(int argc, char* argv[]){
	if (argc != 3)
		cerr << "invalid input format" << endl;
	else {
		Ensembler ensembler(argv[1], argv[2]);
		ensembler.makeClassifiersDirections();
		ensembler.forkLinearClassifiers();
		ensembler.forkVoter();
        ensembler.fetchData();
        double accuracy = ensembler.measureAccuracy();
        cout << "Accuracy: " << fixed << setprecision(2) << accuracy << "%" << endl;
	}
}