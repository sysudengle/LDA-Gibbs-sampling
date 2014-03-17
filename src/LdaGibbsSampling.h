/*
 *
 *
 */
#ifndef _LDA_GIBBS_SAMPLING_H__
#define _LDA_GIBBS_SAMPLING_H__

#include "LdaMacro.h"
#include "Documents.h"
#include "LdaConfig.h"
#include <vector>

using namespace std;

class Documents;

class LdaGibbsSampling
{
public:
	LdaGibbsSampling(string sIteration = "0");
	~LdaGibbsSampling();

	void initialize(bool, Documents &);
	void trainingModel(Documents &);
	int sampleZ(int, int);
	void infer(Documents &);

	void updateParam(Documents &);
	void saveResult(Documents &, int iIteration);

	void readModelParam(LdaGibbsSampling &, string &);
	static bool compare(const PAIR &x, const PAIR &y);

	int m_iM, m_iK, m_iV;				//count of documents, topics, terms respectively
	INT_2D_ARRAY m_arr2_iDoc;		//document-word array
	DOUBLE_2D_ARRAY m_arr2_dPhi;	//K * V, topic-term
	DOUBLE_2D_ARRAY m_arr2_dTheta;//M * K, document-topic
	INT_2D_ARRAY m_arr2_iZ;		//topic label array
	INT_2D_ARRAY m_arr2_iN_mk; 	//M * K, document-topic count. Given document m, the count of times of topic k
	INT_2D_ARRAY m_arr2_iN_kv; 	//K * V, topic-term count. Given topic k, the count of times of word v
	INT_ARRAY m_arr_iN_mk_Sum; 			//sum for document-topic count of each row
	INT_ARRAY m_arr_iN_kv_Sum; 			//sum for topic-term count of each row

	double m_fAlpha;
	double m_fBeta;
	
	int m_iIteration;
	int m_iSampleLag;
	int m_iBeginSample;
	
	bool m_bChoice;				//true means training, else infer
	bool m_bInitialized;

	string m_sIteration;
};

#endif
