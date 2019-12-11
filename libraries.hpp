#ifndef LIBRARIES_HPP
#define LIBRARIES_HPP

#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/socket.h> 
#include <fcntl.h>
#include <fstream>
#include <string.h>
#include <netdb.h> 
#include <netinet/in.h>
#include <arpa/inet.h> 
#include <stdlib.h> 
#include <signal.h>
#include <errno.h>
#include <sstream>
#include <cmath>
#include <sys/wait.h>

#define DATASET "dataset.csv"
#define LABELS "labels.csv"
#define CLASSIFIER "classifier_"
#define CSV ".csv"
#define PATHSIZE 512
#define CLASSIFIED "classified_"
#define VOTER "voter_"
#define MAXCLASSES 100

#endif