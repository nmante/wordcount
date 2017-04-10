#include "wordcount.hpp"
#include <iostream>

int main(int argc, char *argv[]){
	std::string document = "practice makes perfect. get perfect by practice. just practice!";
	std::cout << "Document: " << document << std::endl;
	std::cout << "Non Parallel" << std::endl;
	WordCount w(document);
	w.printWordCounts();

	int numJobs = 2;
	if(argc > 1){
		numJobs = std::atoi(argv[1]);
	}
	std::cout << "Parallel - " << numJobs << " Jobs" << std::endl;
	WordCount w2(document, numJobs); 
	w2.printWordCounts();

	return 0;
}
