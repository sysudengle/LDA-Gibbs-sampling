#include "WordTrimer.h"
#include "string.h"
#include <iostream>

void WordTrimer::trimAndToLowerCase(const string &sStr, string &sTrimedStr)
{
	char sWord[100];
	char sTrimedWord[100];
	int iLen = 0;
	int iBegin = 0, iEnd;

	iLen = sStr.size();
	for(int i = 0; i < iLen; i++)
		if(sStr[i] >= 'A' && sStr[i] <= 'Z')
			sWord[i] = sStr[i] - 'A' + 'a';
		else
			sWord[i] = sStr[i];
	
	while(iBegin < iLen && sWord[iBegin] < 'a' || sWord[iBegin] > 'z') iBegin++;

	if(iBegin == iLen) { sTrimedStr = ""; return; }

	iEnd = iBegin;
	while(iEnd < iLen && sWord[iEnd] >= 'a' && sWord[iEnd] <= 'z') iEnd++;
	
	iLen = iEnd - iBegin;
	memcpy(sTrimedWord, &sWord[iBegin], iLen);
	sTrimedWord[iLen] = '\0';

//	cout << "Word: " << sTrimedWord << endl;
	sTrimedStr = sTrimedWord;
}

void WordTrimer::toLowerCase(string & sWord)
{
	for(int i = 0; i < sWord.size(); i++)
		if(sWord[i] >= 'A' && sWord[i] <= 'Z')
			sWord[i] = sWord[i] - 'A' + 'a';
}
