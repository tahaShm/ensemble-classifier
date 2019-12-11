#include "linearClassifier.hpp"

using namespace std;

Linear::Linear(char* datasetAdr, char* weightVectorAdr, char* id) {
	this->datasetAdr = datasetAdr;
	this->weightVectorAdr = weightVectorAdr;
	this->classifierId = stoi(id);
}
void Linear::setWeights(){
    fstream fin; 
    string weightVectorPath = this->weightVectorAdr;
	bool isFirstLine = true;
    bool isEmpty = true;
    fin.open("./" + weightVectorPath, ios::in);
    vector<string> row; 
    string line; 
    while (fin >> line) {
		// if (isFirstLine) cout << "taha weights : " <<  line << endl;
		if (isFirstLine == false){
  		isEmpty = false;
			stringstream ss(line);
			string token;
			vector<double> currentVals(3);
			int index = 0;
			while (getline(ss,token, ','))
			{
				currentVals[index] = stod(token.c_str());;
				index++;
			}
			weights.push_back(currentVals);
		}
		else isFirstLine = false;
    }
    fin.close();
    if (isEmpty){
    	cout << "file not found" << endl;
    	exit(1);
    }
}

void Linear::setData(){
    fstream fin; 
    string datasetPath = this->datasetAdr;
	bool isFirstLine = true;
    bool isEmpty = true;
    fin.open("./" + datasetPath, ios::in);
    vector<string> row; 
    string line; 
    while (fin >> line) {
		// if (isFirstLine) cout <<"taha datasets : " <<  line << endl;
		if (isFirstLine == false){
			isEmpty = false;
			stringstream ss(line);
			string token;
			vector<double> currentVals(2);
			int index = 0;
			while (getline(ss,token, ','))
			{
				currentVals[index] = stod(token.c_str());;
				index++;
			}
			data.push_back(currentVals);
		}
		else isFirstLine = false; 
    }
    fin.close();
    if (isEmpty){
    	cout << "file not found" << endl;
    	exit(1);
    }
}
double Linear::calculateInnerProduct(vector <double> dataset, vector <double> weight){
	int size = dataset.size();
	double product = 0;
	for (int i = 0; i < size; i++)
		product += dataset[i]*weight[i];
	product += weight[size];
	return product;
}
void Linear::calculateProducts(){
	int dataSize = data.size();
	int weightsSize = weights.size();
	for (int i = 0; i < dataSize; i++) {
		vector<double> innerProducts;
		double currentProduct = 0;
		for (int j = 0; j < weightsSize; j++) {
			currentProduct = calculateInnerProduct(data[i], weights[j]);
			innerProducts.push_back(currentProduct);
		}
		products.push_back(innerProducts);
	}
}

int Linear::findClass(vector <double> curProducts) {
	int productSize = curProducts.size();
	int ans = 0;
	double maxVal = curProducts[0];
	for (int i = 0; i < productSize; i++) {
		if (curProducts[i] > maxVal){
			maxVal = curProducts[i];
			ans = i;
		}
	}
	return ans;
}
void Linear::calculateClasses() {
	calculateProducts();
	int dataSize = products.size();
	for (int i = 0;i < dataSize;i++){
		int currentClass = findClass(products[i]);
		classes.push_back(currentClass);
	}
}
string Linear::convertClassesToString(){
	string ans = "";
	int classSize = classes.size();
	for (int i = 0; i < classSize; i++){
		ans += to_string(classes[i]);
		ans += "\n";
	}
	return ans;
}
void Linear::sendClasses(){
	string route = CLASSIFIED + to_string(classifierId); 
	string labels = convertClassesToString();
	int fd = open(route.c_str(), O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
	close(fd);
	mkfifo(route.c_str(),0666);
	fd = open(route.c_str(), O_WRONLY);
    int writed = write(fd,labels.c_str(),labels.size());
    close(fd);
}