/*
 *
 *
 */

#ifndef _LDA_CONFIG_H__
#define _LDA_CONFIG_H__

#define TRAIN_DOCS_DIR_PATH "../data/trainingDocuments/"

#define INFER_DOCS_DIR_PATH "../data/inferDocuments/"

#define STOPWORDS_FILE_PATH "../data/filterWords/stopWords"

#define NOISEFORMAT_FILE_PATH "../data/filterwords/noiseFormat"

#define TRAIN_RESULT_DIR "../data/trainingResult/"

#define TERM_TO_INDEX "_TermToIndex.train"
#define TERM_COUNT "_TermCount.train"
#define N_KV "_N_kv.train"
#define N_KV_SUM "_N_kv_Sum.train"

const int DEFAULT_INT = 0;
const double DEFAULT_DOUBLE = 0;
#define DEFAULT_STR "0"

const int K = 9;
const double ALPHA = 25;
const double BETA = 0.1;
const int ITERATION = 100;
const int SAMPLE_LAG = 10;
const int BEGIN_SAMPLE = 80;

const bool TRAIN_CHOICE = true;
const bool INFER_CHOICE = false;
#endif
