#include "Assertion.h"
#include <iostream>


void Test_OnAssertionFailed_Float(float real, float expected, float marg, const char* comment, const char* functionName, int lineNumber, const char* fileName)
{
    printf("Assertion failed: %s\n", comment);
    printf("Expected: %f, Actual: %f, Margin: %f\n", expected, real, marg);
    printf("Location: %s:%d - %s\n", fileName, lineNumber, functionName);
    system("pause");
}

void Test_OnAssertionFailed_Truth(const char *expStr, const char* comment, const char* functionName, int lineNumber, const char* fileName)
{
    printf("Assertion failed: %s\n", comment);
    printf("Expected: %s to be true\n", expStr);
    printf("Location: %s:%d - %s\n", fileName, lineNumber, functionName);
    system("pause");
}

