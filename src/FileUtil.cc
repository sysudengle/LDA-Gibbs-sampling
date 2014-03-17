#include "FileUtil.h"
#include <fstream>
#include <iostream>

bool FileUtil::writeTrainResult(LdaGibbsSampling &oLdaGibbs, Documents &Docs, const char * sIteration)
{
	return FileUtil::writeMapResult(Docs.m_mTermToIndex, TERM_TO_INDEX, sIteration) &&
		FileUtil::writeMapResult(Docs.m_mTermCount, TERM_COUNT, sIteration) &&
		FileUtil::write2dArray(oLdaGibbs.m_arr2_iN_kv, N_KV, sIteration) &&
		FileUtil::writeArray(oLdaGibbs.m_arr_iN_kv_Sum, N_KV_SUM, sIteration);
}

bool FileUtil::readModelGibbsParam(LdaGibbsSampling &oLdaGibbs, string &sIteration)
{
	return FileUtil::read2dArray(oLdaGibbs, sIteration) && FileUtil::readArray(oLdaGibbs, sIteration);
}

bool FileUtil::readModelDocsParam(Documents &Docs, string &sIteration)
{
	return FileUtil::readMapResult(Docs, sIteration);
}


bool FileUtil::writeMapResult(map<string, int> &mMap, const char * sFileName, const char * sIteration)
{
	map<string, int>::iterator itMap = mMap.begin();
	char sFileFullName[100];
	bool bFlag = false;
	ofstream fout;
	
	sprintf(sFileFullName, "%s%s%s", TRAIN_RESULT_DIR, sIteration, sFileName);
	fout.open(sFileFullName);
	if(fout.is_open())
	{
		fout << mMap.size() << endl;
		for(; itMap != mMap.end(); itMap++)
		{
			fout << itMap->first << " " << itMap->second << endl;
		}
		bFlag = true;
	}
	fout.close();
	return bFlag; 
}

bool FileUtil::write2dArray(INT_2D_ARRAY &arr2_iArr, const char * sFileName, const char * sIteration)
{
	int iK = arr2_iArr.size();
	int iV = arr2_iArr[0].size();
	char sFileFullName[100];
	bool bFlag = false;
	ofstream fout;
	
	sprintf(sFileFullName, "%s%s%s", TRAIN_RESULT_DIR, sIteration, sFileName);
	fout.open(sFileFullName);
	if(fout.is_open())
	{
		for(int k = 0; k < iK; k++)
		{
			for(int v = 0; v < iV; v++)
			{
				fout << arr2_iArr[k][v] << " ";
			}
			fout << endl;
		}
		bFlag = true;
	}
	fout.close();
	return bFlag;
}

bool FileUtil::writeArray(INT_ARRAY &arr_iArr, const char * sFileName, const char * sIteration)
{
	int iK = arr_iArr.size();
	char sFileFullName[100];
	bool bFlag = false;
	ofstream fout;
	
	sprintf(sFileFullName, "%s%s%s", TRAIN_RESULT_DIR, sIteration, sFileName);
	fout.open(sFileFullName);
	if(fout.is_open())
	{
		for(int k = 0; k < iK; k++)
		{
			fout << arr_iArr[k] << " ";
		}
		fout << endl;
		bFlag = true;
	}
	fout.close();
	return bFlag;
}


bool FileUtil::readMapResult(Documents &oDocs, string &sIteration)
{
	char sFile_TermToIndex_FullName[100];
	char sFile_TermCount_FullName[100];
	string sLine;
	int iInt;
	char sStr[100];
	int iV;
	sprintf(sFile_TermToIndex_FullName, "%s%s%s", TRAIN_RESULT_DIR, sIteration.c_str(), TERM_TO_INDEX);
	sprintf(sFile_TermCount_FullName, "%s%s%s", TRAIN_RESULT_DIR, sIteration.c_str(), TERM_COUNT);
	ifstream finTermToIndex(sFile_TermToIndex_FullName);
	ifstream finTermCount(sFile_TermCount_FullName);

	cout << "read Docs Param" << endl;
//	finTermToIndex >> sLine;
//	iV = atoi(sLine.c_str());
	getline(finTermToIndex, sLine);
	sscanf(sLine.data(), "%d", &iV);
	cout << iV << endl;
	INIT_ARR(iV, oDocs.m_vIndexToTerm, string, DEFAULT_STR);
	int i = 0;
	while(getline(finTermToIndex, sLine))
	{
		sscanf(sLine.data(), "%s %d", sStr, &iInt);
		oDocs.m_mTermToIndex.insert(STR_INT_PAIR(sStr, iInt));
		oDocs.m_vIndexToTerm[iInt] = string(sStr);
//		if(i++ < 30) cout << "pair: " << sStr << " ::::: " << iInt << endl;
	}

	i = 0;
	while(getline(finTermCount, sLine))
	{
		sscanf(sLine.data(), "%s %d", sStr, &iInt);
//		if(i++ < 30) cout << "term count pair: " << sStr << "===" << iInt << endl;
		oDocs.m_mTermCount.insert(STR_INT_PAIR(sStr, iInt));
	}

	return true;
}

bool FileUtil::read2dArray(LdaGibbsSampling &oLdaGibbs, string &iIteration)
{
	return true;
}

bool FileUtil::readArray(LdaGibbsSampling &oLdaGibbs, string &sIteration)
{
	char sFileFullName[100];
	string sItem;
	int iN_kv_Sum;
	sprintf(sFileFullName, "%s%s%s", TRAIN_RESULT_DIR, sIteration.c_str(), N_KV_SUM);
	ifstream fin(sFileFullName);
	
	cout << "read file: " << sFileFullName << endl;
	while(fin >> sItem)
	{
		iN_kv_Sum = atoi(sItem.c_str());
		cout << "read Array: " << iN_kv_Sum << endl;
	}

	return true;
}

