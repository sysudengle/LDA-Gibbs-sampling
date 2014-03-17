#include <iostream>
#include <stdio.h>
#include "Documents.h"
#include "Document.h"
#include "WordFilter.h"
#include "LdaConfig.h"
#include "LdaMacro.h"
#include "LdaGibbsSampling.h"
//#include <boost/smart_ptr.hpp>
#include <tr1/memory>

using namespace std;

int main(int argc, char *argv[])
{
	
	if(argc == 1) {cout << "Argument must be at least 1" << endl; return 0;}
	string sChoice = string(argv[1]);
	string sIteration;
	if(argc == 3) sIteration = string(argv[2]);

	if(sChoice == "train")
	{
		LdaGibbsSampling oLdaGibbs;
		tr1::shared_ptr<Documents> pDocuments(new Documents(TRAIN_CHOICE));

		pDocuments->readDocs(TRAIN_DOCS_DIR_PATH);
		oLdaGibbs.initialize(TRAIN_CHOICE, (* pDocuments));
		oLdaGibbs.trainingModel((* pDocuments));
	}
	else if(sChoice == "infer")
	{
		LdaGibbsSampling oLdaGibbs(sIteration);
		tr1::shared_ptr<Documents> pDocuments(new Documents(INFER_CHOICE));

		pDocuments->readModelParam(sIteration);
		pDocuments->readDocs(INFER_DOCS_DIR_PATH);
		oLdaGibbs.initialize(INFER_CHOICE, (* pDocuments));
		oLdaGibbs.infer((* pDocuments));
	}
	else cout << "wrong command!!~~" << endl;

	return 0;
}
