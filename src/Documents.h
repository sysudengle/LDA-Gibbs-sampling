/* 
 * 
 *
 */
#ifndef _LDA_Documents_H__
#define _LDA_Documents_H__

#include "Document.h"
#include "LdaGibbsSampling.h"
#include <vector>
#include <map>
#include <set>
#include <string>
#include <tr1/memory>

using namespace std;

class Documents
{
public:
	Documents(bool bChoice = TRAIN_CHOICE);
	~Documents();

	void readDocs(const char *);
	void readModelParam(string &);

	vector<tr1::shared_ptr<Document> > m_vDocuments;
	vector<string> m_vIndexToTerm;
	map<string, int> m_mTermToIndex;
	map<string, int> m_mTermCount;

	bool m_bChoice;
//	WordsFilter m_oWordsFilter;
};

#endif
