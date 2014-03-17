#include "WordFilter.h"
#include "fstream"
#include "LdaMacro.h"
#include <iostream>

WordFilter * WordFilter::m_pInstance;

WordFilter * WordFilter::getInstance()
{
	if(m_pInstance == NULL)
	{
		m_pInstance = new WordFilter(); 
		m_pInstance->loadStopWords();
		m_pInstance->loadNoiseFormat();
	}
	return m_pInstance;
}

WordFilter::WordFilter()
{
		
}

WordFilter::~WordFilter()
{
	delete m_pInstance;
}

void WordFilter::loadStopWords()
{
	ifstream fin("../data/filterWords/stopWords");
	string sStopWord;

	while(getline(fin, sStopWord))
	{
	//	cout << "Stop Word: " << sStopWord << endl;
		m_setStopWords.insert(sStopWord);
	}
}

void WordFilter::loadNoiseFormat()
{
	ifstream fin("../data/filterWords/noiseFormat");
	string sNoiseFormat;

	while(getline(fin, sNoiseFormat))
	{
		m_vNoiseWordsPattern.push_back(regex(sNoiseFormat));
	}
}

bool WordFilter::isNoiseWord(const string &sWord)
{
	bool bFlag = false;
	regex reg(".*[a-z].*");

	if(!regex_match(sWord.c_str(), reg)) return true;
//		cout << "match regex!" << sWord << endl;

	for(int i = 0; i < m_vNoiseWordsPattern.size(); i++)
	{
		if(regex_match(sWord.c_str(), m_vNoiseWordsPattern[i]))
		{
			//cout << "Noise word: " << sWord << endl;
			bFlag = true;
			break;
		}
	}
	return bFlag;
}

bool WordFilter::isStopWord(const string &sWord)
{
/*	if(m_setStopWords.count(sWord) > 0)
		cout << "Stop Word:" << sWord << endl;*/
	return m_setStopWords.count(sWord) > 0;
}

