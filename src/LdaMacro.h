#ifndef _LDA_MACRO_H__
#define _LDA_MACRO_H__

#include <boost/tokenizer.hpp>
#include <vector>
#include <iostream>
#include <map>

using namespace std;

typedef boost::tokenizer<boost::char_separator<char> > TOKENIZER;

typedef vector<vector<int> > INT_2D_ARRAY;
typedef vector<vector<double> > DOUBLE_2D_ARRAY;

typedef vector<int> INT_ARRAY;
typedef vector<double> DOUBLE_ARRAY;

typedef map<string, int> STR_INT_MAP;
typedef pair<string, int> STR_INT_PAIR;
typedef pair<int, double> PAIR;
#define INIT_2D_ARR(M, N, ARRAY, TYPE, VALUE) \
	for(int i = 0; i < M; i++) \
	{ \
		ARRAY.push_back(vector<TYPE>()); \
		for(int j = 0; j < N; j++) \
		{ \
			ARRAY[i].push_back(VALUE); \
		} \
	}

#define INIT_ARR(N, ARRAY, TYPE, VALUE) \
	for(int i = 0; i < N; i++) \
	{ \
		ARRAY.push_back(VALUE); \
	}
#endif
