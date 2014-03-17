#include "Document.h"
#include "WordTrimer.h"
#include <iostream>
#include <fstream>
#include <streambuf>
#include <string>
#include <boost/tokenizer.hpp>

Document::Document(bool bChoice/*=TRAIN_CHOICE*/, const char * cFileName, vector<string> &vIndexToTerm, map<string, int> &mTermToIndex, map<string, int> &mTermCount)
{
//	cout << cFileName << endl;
	ifstream in(cFileName, ios::in);
	istreambuf_iterator<char> begin(in), end;
	string sContent(begin, end);
//	cout << sContent << endl;
	
	boost::char_separator<char> oSep(" \t\n.,[]()<>{}+-?\"*!_`\'\\/:;~&^%$#");
//	boost::char_separator<char> oSep(" \n\t,.");
	TOKENIZER tokens(sContent, oSep);
	WordFilter * pWordFilter = WordFilter::getInstance();

	for(TOKENIZER::iterator tok_iter = tokens.begin(); tok_iter != tokens.end(); tok_iter++)
	{
//		cout << (* tok_iter) << endl;
		string sLowerTrimedWord;
		WordTrimer::trimAndToLowerCase((* tok_iter), sLowerTrimedWord);
//		WordTrimer::toLowerCase(sLowerTrimedWord);

		//judge whether a word is a noise word or stop word, if so remove it
		if(!isFiltered(sLowerTrimedWord, pWordFilter))
		{
			//if(m_setWords.count((* tok_iter)) == 1)
			if(mTermToIndex.find(sLowerTrimedWord) == mTermToIndex.end())
			{
				if(!bChoice) continue;

				int iIndex = vIndexToTerm.size();
				mTermToIndex.insert(pair<string, int>(sLowerTrimedWord, iIndex));
				vIndexToTerm.push_back(sLowerTrimedWord);
				mTermCount.insert(pair<string, int>(sLowerTrimedWord, 1));
				m_vWords.push_back(iIndex);
			}
			else
			{
				mTermCount[sLowerTrimedWord]++;
				cout << "push back to documents:" << mTermToIndex[sLowerTrimedWord] << endl;
				m_vWords.push_back(mTermToIndex[sLowerTrimedWord]);
			}
//			cout << sLowerTrimedWord << " " << mTermCount[sLowerTrimedWord] << endl;
		}
	}
//	cout << "Document size:" << m_vWords.size() << endl;
}

Document::~Document()
{

}


bool Document::isFiltered(const string & sWord, WordFilter * pWordFilter)
{
	return pWordFilter->isNoiseWord(sWord) || pWordFilter->isStopWord(sWord);
}
