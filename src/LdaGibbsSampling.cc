#include "LdaGibbsSampling.h"
#include "FileUtil.h"
#include <algorithm>
#include <time.h>
#include <fstream>
#include <iostream>
#include <stdio.h>

bool compare(const PAIR &x, const PAIR &y)
{
	return x.second < y.second;
}

LdaGibbsSampling::LdaGibbsSampling(string sIteration/* = "0"*/)
{
	m_fAlpha = ALPHA;
	m_fBeta = BETA;
	m_iK = K;
	m_iIteration = ITERATION;
	m_iBeginSample = BEGIN_SAMPLE;
	m_iSampleLag = SAMPLE_LAG;

	m_sIteration = sIteration;
}

LdaGibbsSampling::~LdaGibbsSampling()
{

}

void LdaGibbsSampling::initialize(bool bChoice, Documents &oDocs)
{
	//initialize basic members
	m_bChoice = bChoice;
	m_iM = oDocs.m_vDocuments.size();
	m_iV = oDocs.m_mTermCount.size();

	cout << "M: " << oDocs.m_vDocuments.size() << " V: " << oDocs.m_mTermCount.size() << endl;
	cout << "K: " << m_iK << endl;
	INIT_2D_ARR(m_iK, m_iV, m_arr2_dPhi, double, DEFAULT_DOUBLE);
	INIT_2D_ARR(m_iM, m_iK, m_arr2_dTheta, double, DEFAULT_DOUBLE);
	INIT_2D_ARR(m_iK, m_iV, m_arr2_iN_kv, int, DEFAULT_INT);
	INIT_2D_ARR(m_iM, m_iK, m_arr2_iN_mk, int, DEFAULT_INT);

	INIT_ARR(m_iM, m_arr_iN_mk_Sum, int, DEFAULT_INT);
	INIT_ARR(m_iK, m_arr_iN_kv_Sum, int, DEFAULT_INT);

	if(!m_bChoice) 
		readModelParam((* this), m_sIteration);
	//initialize Document array
	for(int m = 0; m < m_iM; m++)
	{
		m_arr2_iDoc.push_back(vector<int>());
		int iN = oDocs.m_vDocuments[m]->m_vWords.size();
		for(int n = 0; n < iN; n++)
		{
			m_arr2_iDoc[m].push_back(oDocs.m_vDocuments[m]->m_vWords[n]);
		}
	}

	/*
	 * Test whether the map variables are right
	 */
	cout << "Document Test---------------------------------------------------------" << endl;
	for(int i = 0; i < m_iM; i++)
	{
		int iN = oDocs.m_vDocuments[i]->m_vWords.size();

		cout << "Docuemnt:" << i << "consists of " << iN << " words and its words are:" << endl;
		for(int j = 0; j < iN; j++)
		{
			cout << oDocs.m_vIndexToTerm[oDocs.m_vDocuments[i]->m_vWords[j]] << " ";
		}
		cout << endl;
	}


	//initialize z
	srand((unsigned)time(NULL));
	for(int m = 0; m < m_iM; m++)
	{
		m_arr2_iZ.push_back(vector<int>());
		int iN = oDocs.m_vDocuments[m]->m_vWords.size();
		for(int n = 0; n < iN; n++)
		{
			int iTopic = random();
//			cout << iTopic << endl;
			iTopic = random() % m_iK;
//			cout << "Z topic choice: z[" << m << "][" << n << "]=" << iTopic << endl;
			m_arr2_iZ[m].push_back(iTopic);
			m_arr2_iN_mk[m][iTopic]++;
//			if(m == 0 && iTopic == 0) cout << "n_mk initialize from:" << m << " " << n << "=" << m_arr2_iN_mk[m][iTopic] << endl;
			m_arr2_iN_kv[iTopic][m_arr2_iDoc[m][n]]++;
			m_arr_iN_kv_Sum[iTopic]++;
		}
		m_arr_iN_mk_Sum[m] = iN;
		cout << "initialize n_mk_sum[" << m << "]=" << m_arr_iN_mk_Sum[m] << endl;
	}
	
}

void LdaGibbsSampling::trainingModel(Documents &oDocs)
{
	//judge whether the paramters have problems
	
	for(int iIteration = 1; iIteration <= m_iIteration; iIteration++)
	{
		cout << "Iteration: " << iIteration << endl;

		//Gibbs sample z
		for(int m = 0; m < m_iM; m++ )
		{
			int iN = oDocs.m_vDocuments[m]->m_vWords.size();
			for(int n = 0; n < iN; n++)
			{
				m_arr2_iZ[m][n] = sampleZ(m, n);
//				cout << "Gibbs sample topic: z[" << m << "][" << n << "]=" << m_arr2_iZ[m][n] << endl;
			}
		}
		//save the model in this iteration
		if(iIteration >= m_iBeginSample && ((iIteration - m_iBeginSample) % m_iSampleLag == 0))
		{
			updateParam(oDocs);
			saveResult(oDocs, iIteration);
		}
	}
	
}

int LdaGibbsSampling::sampleZ(int iM, int iN)
{
	int iOldTopic = m_arr2_iZ[iM][iN];
	m_arr2_iN_mk[iM][iOldTopic]--;
//	if(iM == 0 && iOldTopic == 0)
//	cout << "Old Topic n_mk from" << iM << " " << iN << "[" << iM << "][" << iOldTopic << "]=" << m_arr2_iN_mk[iM][iOldTopic] << "----" << endl;
	m_arr2_iN_kv[iOldTopic][m_arr2_iDoc[iM][iN]]--;
//	cout << "old n_kv=" << m_arr2_iN_kv[iOldTopic][m_arr2_iDoc[iM][iN]] << endl;
	m_arr_iN_kv_Sum[iOldTopic]--;
//	cout << "old n_kv_sum=" << m_arr_iN_kv_Sum[iOldTopic] << endl;
	m_arr_iN_mk_Sum[iM]--;
//	cout << "old n_mk_sum=" << m_arr_iN_mk_Sum[iM] << endl;

	DOUBLE_ARRAY arr_dP_k;
	INIT_ARR(m_iK, arr_dP_k, double, DEFAULT_DOUBLE);
	for(int k = 0; k < m_iK; k++)
	{
		arr_dP_k[k] = (m_arr2_iN_mk[iM][k] + m_fAlpha) / (m_arr_iN_mk_Sum[iM] + m_fAlpha * m_iK)
			* (m_arr2_iN_kv[k][m_arr2_iDoc[iM][iN]] + m_fBeta) / (m_arr_iN_kv_Sum[k] + m_fBeta * m_iV);
//		cout << "P" << k << "=" << arr_dP_k[k] << endl;
	}
	
//	for(int k = 0; k < m_iK; k++) cout << "p" << k << "=" << arr_dP_k[k] << endl;
	for(int k = 0; k < m_iK - 1; k++) arr_dP_k[k + 1] += arr_dP_k[k];

//
//	srand((unsigned)time(NULL));
	int iNewTopic = 0;
	double temp = (double) random() / RAND_MAX;
	//cout << "random:" << temp << endl;
	double dP = temp * arr_dP_k[m_iK - 1];
//	cout << "P=" << arr_dP_k[m_iK - 1] << endl;
	//cout << "random possibility: " << dP << endl;
	for(int k = 0; k < m_iK; k++)
	{
		if(dP < arr_dP_k[k])
		{
			iNewTopic = k;
			break;
		}
	}

//	m_arr2_iZ[iM][iN] = iNewTopic;
	m_arr2_iN_mk[iM][iNewTopic]++;
	/*if(iM == 0 && iNewTopic == 0)
		cout << "New topic[" << iM << "][" << iNewTopic <<"]from" << iM << " " << iN << "=" << m_arr2_iN_mk[iM][iNewTopic] << "++++++" << endl;
	*/
	m_arr2_iN_kv[iNewTopic][m_arr2_iDoc[iM][iN]]++;
	m_arr_iN_kv_Sum[iNewTopic]++;
	m_arr_iN_mk_Sum[iM]++;
	return iNewTopic;
}

void LdaGibbsSampling::infer(Documents &oDocs)
{
	
	for(int iIteration = 1; iIteration <= m_iIteration; iIteration++)
	{
		cout << "Iteration: " << iIteration << endl;

		//Gibbs sample z
		for(int m = 0; m < m_iM; m++ )
		{
			int iN = oDocs.m_vDocuments[m]->m_vWords.size();
			for(int n = 0; n < iN; n++)
			{
				m_arr2_iZ[m][n] = sampleZ(m, n);
//				cout << "Gibbs sample topic: z[" << m << "][" << n << "]=" << m_arr2_iZ[m][n] << endl;
			}
		}
		//save the model in this iteration
		if(iIteration >= m_iBeginSample && ((iIteration - m_iBeginSample) % m_iSampleLag == 0))
		{
			updateParam(oDocs);
		}
	}
}

void LdaGibbsSampling::updateParam(Documents &oDocs)
{
	//update Theta
	cout << "------------------------Theta--------------------------" << endl;
	for(int m = 0; m < m_iM; m++)
	{
		cout << "           ";
		for(int i = 0; i < m_iK; i++) cout << i << "\t";
		cout << endl;

		cout << "document " << m << ":";
		for(int k = 0; k < m_iK; k++)
		{
			m_arr2_dTheta[m][k] = (m_arr2_iN_mk[m][k] + m_fAlpha) / (m_arr_iN_mk_Sum[m] + m_fAlpha * m_iK);
			cout << m_arr2_dTheta[m][k] << "\t";
		}
		cout << endl;
	}
	cout << "------------------------------------------------------" << endl;

	//update Phi
//	DOUBLE_2D_ARRAY arr2_dTempPhi;
//	INIT_2D_ARR(m_iK, m_iV, arr2_dTempPhi, DEFAULT_DOUBLE);
	for(int k = 0; k < m_iK; k++)
	{
//		map<int, double> mTermPro;
		vector<PAIR > vTermPro;
		for(int v = 0; v < m_iV; v++)
		{
			m_arr2_dPhi[k][v] = (m_arr2_iN_kv[k][v] + m_fBeta) / (m_arr_iN_kv_Sum[k] + m_fBeta * m_iV);
//			cout << m_arr2_dPhi[k][v] << endl;
			vTermPro.push_back(make_pair(v, m_arr2_dPhi[k][v]));
//			mTermPro.insert(pair<int, double>(v, m_arr2_dPhi[k][v]));
		}
		sort(vTermPro.begin(), vTermPro.end(), LdaGibbsSampling::compare);
		cout << "Topic " << k << ":"; 
		for(int i = 0; i < 5; i++)
			cout << oDocs.m_vIndexToTerm[vTermPro[i].first] << ":" << vTermPro[i].second << "\t";
		cout << endl;
		vTermPro.clear();
	}
}

void LdaGibbsSampling::saveResult(Documents &oDocs, int iIteration)
{
/*	ofstream foutN_kv, foutN_kv_Sum;
	char cN_kv_FileName[50], cN_kv_SumFileName[50];
	sprintf(cN_kv_FileName, "%s%d_N_kv.csv", TRAIN_RESULT_DIR, iIteration);
	sprintf(cN_kv_SumFileName, "%s%d_N_kv_Sum.csv", TRAIN_RESULT_DIR, iIteration);
	foutN_kv.open(cN_kv_FileName);
	foutN_kv_Sum.open(cN_kv_SumFileName);
	cout << "save file name: +++++++++++++" << cN_kv_FileName << endl;//*/
//	foutN_kv_Sum.open("%s%d_N_kv_Sum.csv", TRAIN_RESULT_DIR, iIteration);
	
	char sIteration[50];
	sprintf(sIteration, "%d", iIteration);
	FileUtil::writeTrainResult(* this, oDocs, sIteration);
	//save N_kv
/*	if(foutN_kv.is_open())
	{
		for(int k = 0; k < m_iK; k++)
		{
			for(int v = 0; v < m_iV; v++)
			{
				foutN_kv << m_arr2_iN_kv[k][v] << " ";
			}
			foutN_kv << endl;
		}
	}

	//save N_kv_Sum
	if(foutN_kv_Sum.is_open())
	{
		for(int k = 0; k < m_iK; k++)
		{
			foutN_kv_Sum << m_arr_iN_kv_Sum[k] << endl;
		}
	}

	foutN_kv.close();
	foutN_kv_Sum.close();*/
}

void LdaGibbsSampling::readModelParam(LdaGibbsSampling &oLdaGibbs, string &sIteration)
{
	FileUtil::readModelGibbsParam((* this), sIteration);
}

bool LdaGibbsSampling::compare(const PAIR &x, const PAIR &y)
{
	return x.second > y.second;
}

