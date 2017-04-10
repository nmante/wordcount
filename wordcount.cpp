#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <future>
#include "wordcount.hpp"

bool WordCount::isSpecialChar(char c){
	std::string special = "~!@#$%^&*().:;/?<>,-+_={}[]|";
	for(auto s : special){
		if(c == s){
			return true;
		}
	}
	return false;
}

StringVector WordCount::split(std::string &document){
	// Iterate through characters until end of document
	// Keep a temp string in loop
	// Add to temp string if not a special character 
	// if white space, you're done with temp string, add it to the 
	// result vector

	std::string temp = "";	
	std::vector<std::string> result({});
	int len = document.size();
	for(int i = 0; i < len; i++){
		char c = document[i];
		if(c == ' ' || (i == len - 1)){
			result.push_back(temp);
			temp = "";
		}else if(!isSpecialChar(c)){
			temp += c;
		}
		
	}
	return result;
}

std::map<std::string, int> WordCount::hash(std::vector<std::string> &words){
	std::map<std::string, int> wordDict;
	for(auto word : words){
		if(wordDict.count(word) == 0){
			wordDict[word] = 0;
		}
		wordDict[word] += 1;
	}
	return wordDict;
}


void merge(WordCountTupleVector &wordTuples, int lo, int mid, int hi){
	// Create temp arrays for left and right half
	WordCountTupleVector left(wordTuples.begin() + lo, wordTuples.begin() + mid + 1);
	WordCountTupleVector right(wordTuples.begin() + mid + 1, wordTuples.begin() + hi + 1);

	// Iterate through big array, choosing the larger values first
	int i = 0, j = 0, k = lo;
	int leftSize = left.size(), rightSize = right.size();
	while(i < leftSize && j < rightSize){
		if(left[i].second > right[j].second){
			wordTuples[k++] = left[i++]; 
		}else{
			wordTuples[k++] = right[j++]; 
		}
	}

	while(i < leftSize){
		wordTuples[k++] = left[i++]; 
	}
	while(j < rightSize){
		wordTuples[k++] = right[j++]; 
	}

}

void mergeSortHelper(WordCountTupleVector &arr, int lo, int hi){
	if(lo < hi){
		int mid = (lo + hi)/2;
		mergeSortHelper(arr, lo, mid);
		mergeSortHelper(arr, mid + 1, hi);
		merge(arr, lo, mid, hi);
	}
}

WordCountTupleVector WordCount::sortHash(WordCountMap &wordDict){
	WordCountTupleVector wv;
	for(auto pair : wordDict){
		wv.push_back(std::make_pair(pair.first, pair.second));
	}
	mergeSortHelper(wv, 0, wv.size() - 1);
	return wv;
}

void WordCount::lowercase(){
	std::transform(this->mDocument.begin(), this->mDocument.end(), this->mDocument.begin(), ::tolower);
}

WordCountMap WordCount::processDocument(std::string &document){
	auto words = this->split(document);
	WordCountMap wcm = this->hash(words);
	return wcm;
}

void WordCount::mergeWordCountMaps(std::vector<WordCountMap> &wordCountMaps){
	for(auto &wcm : wordCountMaps){
		for(auto &pair : wcm){
			if(this->mWordCountMap.count(pair.first) == 0){
				this->mWordCountMap[pair.first] = pair.second;
			}else{
				this->mWordCountMap[pair.first] += pair.second;
			}
		}
	}
}

void WordCount::countWords(){
	// lowercase document
	this->lowercase();
	if(this->mNumChunks == 1){
		WordCountMap wcm = this->processDocument(this->mDocument);
		this->mWordCountMap = wcm;
		this->mWordTuples = this->sortHash(wcm);
		return;
	}

	// We can process sub documents in parallel, or just use one 
	// Generate Chunks
	auto chunks = this->generateSubdocuments();
	
	// Create Asynchronous threads. Each one should process chunk
	std::vector<std::future<WordCountMap> > futures;
	std::vector<WordCountMap> wordCountMaps;
	for(auto &chunk : chunks){
		futures.push_back(std::async(std::launch::async, [&](){
			return this->processDocument(chunk);
		}));
	}
	
	// Get wordCountMap from each thread 
	for(auto &future : futures){
		wordCountMaps.push_back(future.get());
	}

	// Merge the word counts together
	this->mergeWordCountMaps(wordCountMaps);

	// Sort the resulting word count map
	this->mWordTuples = sortHash(this->mWordCountMap);
}

void WordCount::printWordCounts(){
	std::cout << "{ ";
	for(auto tup : this->mWordTuples){
		std::cout << tup.first << ": " << tup.second << ", ";
	}
	std::cout << "}" << std::endl;
}

StringVector WordCount::generateSubdocuments(){
	// Iterate through until len/numChunks
	// If we see a white space, break off and add to the string vector
	// otherwise keep going
	int len = this->mDocument.length();
	int chunkSize = len/mNumChunks;
	if(chunkSize == 0){
		return {mDocument};
	}
	std::vector<std::string> result;
	int start = 0;
	for(int i = 0; i < mNumChunks; i++){
		if(len - start < chunkSize){
			result.push_back(this->mDocument.substr(start, len));
			return result;
		}
		for(int ind = (i+1) * chunkSize; ind < len; ind++){
			if(this->mDocument[ind] == ' ' || ind == len - 1){
				int rest = std::min(ind - start, len - start);
				auto chunk = this->mDocument.substr(start, rest + 1);
				start = ind + 1;
				result.push_back(chunk);
				break;
			}
		}	
	}
	return result;
}



