/* 
 * 
 *
 */
#ifndef _LDA_Document_H__
#define _LDA_Document_H__

#include "WordFilter.h"
#include "LdaMacro.h"
#include "LdaConfig.h"
#include <vector>
#include <string>
#include <set>
#include <map>

using namespace std;

class Document
{
public:
	Document(bool, const char *, vector<string> &, map<string, int> &, map<string, int> &);
	~Document();

	string m_sDocName;
//	set<string> m_setWords;
	vector<int> m_vWords;
	
	bool isFiltered(const string &, WordFilter *);
};

#endif
