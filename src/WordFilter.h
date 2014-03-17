/* 
 * 
 *
 */
#ifndef _LDA_WORD_FILTER_H__
#define _LDA_WORD_FILTER_H__

#include <vector>
#include <string>
#include <set>
#include <boost/regex.hpp>

using namespace boost;
using namespace std;

//Singleton
class WordFilter
{
public:
	~WordFilter();
	static WordFilter * getInstance();
	bool isNoiseWord(const string &);
	bool isStopWord(const string &);
private:
	WordFilter();
	void loadStopWords();
	void loadNoiseFormat();

	static WordFilter * m_pInstance;
	set<string> m_setStopWords;
	vector<regex> m_vNoiseWordsPattern;
};

#endif
