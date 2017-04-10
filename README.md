#Word Count Engine

*Inspired by an interview question*

This repo contains a C++ implementation of a Word Count Engine. Given a document, we should output a count of all the unique words in the document.

##Logic

The program can use multiple threads to process the document in parallel. For small documents, only one job is necessary.

1. Split document into smaller chunks
2. For each document chunk
	3. split the document chunk into tokens/words
	4. Add each token/word as key, and count as value into a Map/Hash
5. Merge dictionaries into one
6. Sort word-count pairs by count
7. Print the word-count pairs

##Example
	./wordcount 4

###Output

	Document: practice makes perfect. get perfect by practice. just practice!
	Non Parallel
	{ practice: 3, perfect: 2, makes: 1, just: 1, get: 1, by: 1, }
	Parallel - 4 Jobs
	{ practice: 3, perfect: 2, makes: 1, just: 1, get: 1, by: 1, }
	
##Usage

To include it in your own projects, simply include the header file, and source in your project and compile with `-std=c++11`. See the [Makefile](Makefile) for reference.

	./wordcount <NUM_JOBS>
	
`NUM_JOBS` corresponds to how many worker threads

At the moment, the document can be placed in `main.cpp` as a string. Feel free to fork and add file `io` capabilities for processing text files.