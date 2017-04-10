#ifndef __WORDCOUNT_HPP__
#define __WORDCOUNT_HPP__
#include <map>
#include <vector>
#include <algorithm>
#include <future>
#include <string>

/*
 * Nii Mante
 * Word Count Engine
 *
 * - Offers parallel processing solution via 'async' lib
 * - Logic
 * 	- Document -> "Clean punctuation & Split on whitespace" -> [words]
 * 	- [words] -> insert words as keys into hash -> [word : count]
 * 	- [word : count] -> sort map by value -> { word1 : count, word2 : count, ... wordN : count}
 */
typedef std::vector<std::string> StringVector;
typedef std::vector<std::pair<std::string, int> > WordCountTupleVector; 
typedef std::map<std::string, int> WordCountMap;

class WordCount {
	std::string mDocument;
	WordCountMap mWordCountMap;
	WordCountTupleVector mWordTuples;
	int mNumChunks;

	// Lowercase the document
	void lowercase();
	
	// Return true if character is punctuation
	bool isSpecialChar(char c);

	// Split String into tokens/words
	StringVector split(std::string &document);

	// Add words to dictionary/hash with word counts as values
	WordCountMap hash(StringVector &words);

	// Sort words by their count values
	WordCountTupleVector sortHash(WordCountMap &wordDict);

	// Process the document
	WordCountMap processDocument(std::string &document);

	// Parallel Processing - Split document into sub documents
	StringVector generateSubdocuments();

	// Parallel Processing - Merge multiple wordcount maps together 
	void mergeWordCountMaps(std::vector<WordCountMap> &wordCountMaps);

	// This is the main function! Does the splitting, merging, etc. 
	void countWords();

public:
	WordCount(std::string document, int numChunks = 1)
	: mDocument(document), mNumChunks(numChunks)
	{
		if(numChunks < 1){
			mNumChunks = 1;
		}
		this->countWords();
	}

	// Get a dictionary/hash containing the word counts
	WordCountMap getWordCountMap() const { return this->mWordCountMap; }

	// Get a vector of word count tuples
	WordCountTupleVector getWordCounts() const { return this->mWordTuples; }

	// Get count for a word
	int getCountForWord(std::string word) {
		if(this->mWordCountMap.count(word) > 0){
			return this->mWordCountMap[word];
		}
		return 0;
	}

	// Print the list of word count tuples
	void printWordCounts();

};

#endif // __WORDCOUNT_HPP__
