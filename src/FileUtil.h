/*
 *
 *
 */
#ifndef _LDA_FileUtil_H__
#define _LDA_FileUtil_H__

#include "LdaMacro.h"
#include "Documents.h"
#include "LdaGibbsSampling.h"
#include "LdaConfig.h"
#include <string>
#include <map>
#include <vector>

using namespace std;

class LdaGibbsSampling;
class Documents;

class FileUtil
{
public:
	static bool writeTrainResult(LdaGibbsSampling &, Documents &, const char *);
	static bool readModelGibbsParam(LdaGibbsSampling &, string &);
	static bool readModelDocsParam(Documents &, string &);

	static bool writeMapResult(map<string, int> &, const char *, const char *);
	static bool write2dArray(INT_2D_ARRAY &, const char *, const char *);
	static bool writeArray(INT_ARRAY &, const char *, const char *);

	static bool readMapResult(Documents &, string &);
	static bool read2dArray(LdaGibbsSampling &, string &);
	static bool readArray(LdaGibbsSampling &, string &);
};

#endif
