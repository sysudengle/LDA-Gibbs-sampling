#include "Documents.h"
#include "FileUtil.h"
#include <iostream>
//#include "io.h"
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <boost/lexical_cast.hpp>

Documents::Documents(bool bChoice /*=TRAIN_CHOICE*/)
{
	m_bChoice = bChoice;
}

Documents::~Documents()
{

}

void Documents::readDocs(const char * sDirPath)
{
//	_finddata_t file;
//	long lf = _findfirst(sDirPath, file);
//
//	if(lf != -1)
//	{
//		while(_findnext(lf, &file) == 0)
//		{
//			cout << file.name << endl;
//		}
//	}
	DIR *pDir;
	struct dirent *ent;
	pDir = opendir(sDirPath);

	while((ent = readdir(pDir)) != NULL)
	{
		if(strncmp(ent->d_name, ".", 1) == 0)
			continue;

		char cFileName[100];

		strcpy(cFileName, sDirPath);
		strcat(cFileName, ent->d_name);
		cout << cFileName << endl; 

		tr1::shared_ptr<Document> pDoc(new Document(m_bChoice, cFileName, m_vIndexToTerm, m_mTermToIndex, m_mTermCount));
		m_vDocuments.push_back(pDoc);
		cout << ent->d_name << endl;
	}

	closedir(pDir);
}

void Documents::readModelParam(string &sIteration)
{
	FileUtil::readModelDocsParam((* this), sIteration);
}
