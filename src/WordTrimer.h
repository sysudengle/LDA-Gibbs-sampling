/* 
 * 
 *
 */
#ifndef _LDA_WORD_TRIMER_H__
#define _LDA_WORD_TRIMER_H__

#include <string>

using namespace std;

class WordTrimer
{
public:
	static void trimAndToLowerCase(const string &, string &);
	static void toLowerCase(string &);
};

#endif
