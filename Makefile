all: EnsembleClassifier.out
	make linearClassifier
	make voter

EnsembleClassifier.out: ensembleClassifier.o linearClassifier.o voter.o
	g++ -std=c++11 ensembleClassifier.o linearClassifier.o voter.o $(CCFLAGS) -o EnsembleClassifier.out

ensembleClassifier.o: ensembleClassifier.cpp
	g++ -std=c++11 -c ensembleClassifier.cpp

linearClassifier : linearMain.o linearClassifier.o
	g++ -std=c++11 linearMain.o linearClassifier.o -o linearClassifier

voter : voterMain.o voter.o
	g++ -std=c++11 voterMain.o voter.o -o voter

linearMain.o : linearMain.cpp
	g++ -std=c++11 -c linearMain.cpp

linearClassifier.o: linearClassifier.cpp
	g++ -std=c++11 -c linearClassifier.cpp

voter.o: voter.cpp
	g++ -std=c++11 -c voter.cpp

clean:
	rm *.o
	rm EnsembleClassifier.out				