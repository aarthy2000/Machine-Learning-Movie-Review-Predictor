////////////////////////////////////////////////////////////////////////////////


// File Name:      a2.cpp
//
// Author:         Gerald
// CS email:       gerald@cs.wisc.edu
//
// Description:    The source file for a2.
//
// IMPORTANT NOTE: THIS IS THE ONLY FILE THAT YOU SHOULD MODIFY FOR A2.
//                 You SHOULD NOT MODIFY any of the following:
//                   1. Name of the functions/methods.
//                   2. The number and type of parameters of the functions.
//                   3. Return type of the functions.
////////////////////////////////////////////////////////////////////////////////
#include "a2.hpp"
#include "trim.hpp"
#include <algorithm>
#include <cctype>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>
void cleanData(std::ifstream &inFile, std::ofstream &outFile,
		std::unordered_set<std::string> &stopwords) {
	std::string line;
	//vector containing the words
	std::vector<std::string> words;
	//vector that contains the words without punctuations and stopwords
	std::vector<std::string> wordswithout;
	while(true)
	{
		std::getline(inFile,line);
		wordswithout.clear();
		//   1. Replace hyphens with spaces.
		replaceHyphensWithSpaces(line);
		//   2. Split the line of text into individual words.
		splitLine(line,words);
		//   3. Remove the punctuation marks from the words.
		removePunctuation(words,wordswithout);
		//   5. Remove the empty words.

		removeEmptyWords(wordswithout);
		//   4. Remove the trailing and the leading whitespaces in each word.
		removeWhiteSpaces(wordswithout);
		//   6. Remove words with just one character in them. You should NOT remove
		//      numbers in this step because if you do so, you'll lose the ratings.
		removeSingleLetterWords(wordswithout);
		//7 . Remove stopwords
		removeStopWords(wordswithout,stopwords);

		if(inFile.eof())
			break;

		for(auto it=wordswithout.begin();it!=wordswithout.end();++it)
		{
			outFile<<*it<< " ";
		}
		outFile<<std::endl;
	}

}

void fillDictionary(std::ifstream &newInFile,
		std::unordered_map<std::string, std::pair<long, long>> &dict) {
	// TODO: Implement this method.
	// approximate # of lines of code in Gerald's implementation: < 20
	std::string line;
	std::string word;
	int j=0,count=0;
	while(true)
	{
		count=0;
		std::getline(newInFile,line);
		for(int i=0;i<line.length();i++)
		{

			if(!isspace(line[i])&&!isdigit(line[i]))
			{
				word.push_back(line[i]);
				continue;

			}


			auto p=dict.find(word);
			if(p!=dict.end())
			{
				dict[word].first+=(line[0]-'0');
				dict[word].second+=1;
				word.clear();
			}
			else
			{
				dict.emplace(std::make_pair(word,std::make_pair((line[0]-'0'),1)));
				word.clear();
			}
			//erases any empty strings in the dictionary
			dict.erase("\0");
		}
		
		if(newInFile.eof())
			break;
	}

}


void fillStopWords(std::ifstream &inFile,
		std::unordered_set<std::string> &stopwords) {
	// TODO: Implement this method.
	// approximate # of lines of code in Gerald's implementation: < 5
	std::string line;
	while(true)
	{
		std::getline(inFile,line);
		stopwords.insert(line);
		if(inFile.eof())
			break;
	}
}

void rateReviews(std::ifstream &testFile,
		std::unordered_map<std::string, std::pair<long, long>> &dict,
		std::ofstream &ratingsFile) {
	// TODO: Implement this method.
	// approximate # of lines of code in Gerald's implementation: < 20
	std::string line,word;
	double review=0;
	int wordcount=0;
	double avgreview=0;
	while(true)
	{
		wordcount=0;
		review=0;
		avgreview=0;
		std::getline(testFile,line);
		for(int i=0;i<line.length();++i)
		{
			
			std::stringstream str(line);
			while(str>>word)
			{
			if(!isspace(word[0]))
			{
				auto p=dict.find(word);
				if(p!=dict.end())
				{
					review+=((double)dict[word].first/(double)dict[word].second);
					wordcount++;
					word.clear();
				}
				else
				{
					review+=neutral_rating;
					wordcount++;
					word.clear();
				}
			}
			}
		}
		avgreview=review/wordcount;
		if(!std::isnan(avgreview))
			ratingsFile<<std::fixed<<std::setprecision(2)<<(double)avgreview<<std::endl;

		if(testFile.eof())
			break;

	}

}
void removeEmptyWords(std::vector<std::string> &tokens) {
	// TODO: Implement this method.
	// approximate # of lines of code in Gerald's implementation: < 5
	for(auto it=tokens.begin();it!=tokens.end();)
	{

		if(it->length()==0)
		{

			it=tokens.erase(it);
		}
		else 
		{
			++it;
		}
	}

}
void removePunctuation(std::vector<std::string> &inTokens,
		std::vector<std::string> &outTokens) {
	// TODO: Implement this method.
	// approximate # of lines of code in Gerald's implementation: < 10
	std::string x;
	int start,end;

	for(auto it=inTokens.begin();it!=inTokens.end();++it)
	{
		x=(*it);
		for(int i=0;i<(*it).size();)
		{
			if(ispunct(x[i]))
				x.erase(i,1);
			else
				i++;
		}
		outTokens.push_back(x);

	}
}
void removeSingleLetterWords(std::vector<std::string> &tokens) {
	// TODO: Implement this method.
	// approximate # of lines of code in Gerald's implementation: < 5
	for(auto it=tokens.begin();it!=tokens.end();)
	{

		if((it->length()==1)&&(!std::isdigit((*it)[0])))
		{

			it=tokens.erase(it);
		}
		else
			++it;
	}
}


void removeStopWords(std::vector<std::string> &tokens,
		std::unordered_set<std::string> &stopwords) {
	// TODO: Implement this method.

	// approximate # of lines of code in Gerald's implementation: < 5
	for(auto it=tokens.begin();it!=tokens.end();)
	{
		auto p=stopwords.find(*it);
		if(p!=stopwords.end())
		{	
			auto q=std::find(tokens.begin(),tokens.end(),*p);
			it=tokens.erase(it);
		}
		else
		{
			++it;
		}
	}
}



void removeWhiteSpaces(std::vector<std::string> &tokens) {
	// TODO: Implement this method.
	// approximate # of lines of code in Gerald's implementation: < 5
	// You may want to use the trim() method from the trim.*pp files in a2.
	std::string x;
	const std::string whitespace = " \t\f\v\n\r";
	int start,end;
	for(auto it=tokens.begin();it!=tokens.end();++it)
	{
		*it=trim(*it);
	}
}
void replaceHyphensWithSpaces(std::string &line) {
	// TODO: Implement this method.
	//	// approximate # of lines of code in Gerald's implementation: < 5

	std::replace( line.begin(), line.end(), '-', ' ');
}

void splitLine(std::string &line, std::vector<std::string> &words) {
	// TODO: Implement this method.
	// approximate # of lines of code in Gerald's implementation: < 10
	std::string temp;
	words.clear();

	for(int i=0;i<line.length();i++)
	{
		if(!isspace(line[i])&&(line[i]!='\0'))
		{
			temp.push_back(line[i]);
			continue;
		}

		if(temp!=" ")
			words.push_back(temp);
		temp.clear();

	}
	words.push_back(temp);
	std::remove(words.begin(),words.end(),"\t");
}

