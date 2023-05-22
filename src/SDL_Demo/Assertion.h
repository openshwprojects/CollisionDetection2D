#ifndef __ASSERTION_H__
#define __ASSERTION_H__

#include <math.h>

void Test_OnAssertionFailed_Float(float real, float expected, float marg, const char* comment, const char* functionName, int lineNumber, const char* fileName);
void Test_OnAssertionFailed_Truth(const char *expStr, const char* comment, const char* functionName, int lineNumber, const char* fileName);

#define ASSERT_FLOAT_EQUALS(real, expected, marg, comment) \
	if(fabs((real)-(expected))>marg) { \
		Test_OnAssertionFailed_Float((real),(expected),(marg), (comment),__FUNCTION__, __LINE__, __FILE__); \
	} \


#define ASSERT_TRUTH(expectedTruth, comment) \
	if(!expectedTruth) { \
		Test_OnAssertionFailed_Truth(#expectedTruth, (comment),__FUNCTION__, __LINE__, __FILE__); \
	} \

#endif // __ASSERTION_H__
