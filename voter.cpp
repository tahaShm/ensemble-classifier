#include "voter.hpp"

using namespace std;

Voter::Voter(char* num){
    string numb = num;
    this->linearClassifiers = stoi(num);
}
void Voter::fetchClasses(){ //here smashes
    for (int i = 0; i < linearClassifiers; i++) {
        string currentRoute = CLASSIFIED + to_string(i);
        fstream fin; 
        bool isEmpty = true;
        fin.open(currentRoute.c_str(), ios::in); 
        string line; 
        vector <int> currentRow;
        while (fin >> line) { 
            isEmpty = false;
            int currentVal = atoi(line.c_str());
            currentRow.push_back(currentVal);
        }
        classes.push_back(currentRow);
        fin.close();
        if (isEmpty){
            cout << "file not found" << endl;
            exit(1);
        }   
    }
    // for (int i = 0; i < linearClassifiers; i++) {
    //     for (int j = 0; j < classes[0].size();j++){
    //         cout << classes[i][j] << " ";
    //     }
    //     cout << endl;
    // }
}
int Voter::findTrueClass(int index) {
    int values[MAXCLASSES];
    for (int i = 0; i < MAXCLASSES; i++)
        values[i] = 0;
    int size = classes.size();
    for (int i = 0; i < size; i++)
        values[classes[i][index]]++;
    int ans = -1;
    int max = -1;
    for (int i = 0; i < MAXCLASSES; i++){
        if (values[i] > max) {
            ans = i;
            max = values[i];
        }
    }
    return ans;

}
void Voter::determineTrueClasses(){
    for (int i = 0;i < classes[0].size(); i++) { 
        int trueClass = findTrueClass(i);
        trueClasses.push_back(trueClass);
    }
    // for (int i = 0; i < trueClasses.size(); i++)
    //     cout << trueClasses[i];
}
string Voter::convertClassesToString(){
	string ans = "";
	int classSize = trueClasses.size();
	for (int i = 0; i < classSize; i++){
		ans += to_string(trueClasses[i]);
		ans += '\n';
	}
	return ans;
}
void Voter::sendToEnsemble(){
    string route = VOTER; 
	string labels = convertClassesToString();
	int fd = open(route.c_str(), O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
	close(fd);
	mkfifo(route.c_str(),0666);
	fd = open(route.c_str(), O_WRONLY );
    int writed = write(fd,labels.c_str(),labels.size());
    close(fd);
}