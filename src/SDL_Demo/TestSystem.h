#ifndef __TESTSYSTEM_H__
#define __TESTSYSTEM_H__

#include <math.h>
#include <stdio.h>


// ---- Test runner state (internal) ----
#define TESTSYSTEM_MAX_TESTS 64

struct TestEntry {
	const char* name;
	void (*func)();
};

extern int g_testsPassed;
extern int g_testsFailed;
extern int g_testsChecksPassed;
extern int g_testsChecksFailed;
extern const char* g_currentTestName;
extern TestEntry g_testEntries[];
extern int g_testCount;

// Register a test function
#define TEST_REGISTER(testName)                                 \
	static struct _TestReg_##testName {                         \
		_TestReg_##testName() {                                 \
			g_testEntries[g_testCount].name = #testName;        \
			g_testEntries[g_testCount].func = &Test_##testName; \
			g_testCount++;                                      \
		}                                                       \
	} _testRegInstance_##testName;

// Define + register a test in one shot
#define TEST(testName)             \
	static void Test_##testName(); \
	TEST_REGISTER(testName)        \
	static void Test_##testName()

// Check a boolean expression — logs but never bails
#define TEST_CHECK(expr, msg)                                    \
	do {                                                         \
		if (expr) {                                              \
			g_testsChecksPassed++;                               \
		} else {                                                 \
			g_testsChecksFailed++;                               \
			printf("  FAIL: %s\n", (msg));                       \
			printf("    Expression: %s\n", #expr);               \
			printf("    Location: %s:%d\n", __FILE__, __LINE__); \
		}                                                        \
	} while (0)

// Check float equality within margin — logs but never bails
#define TEST_CHECK_FLOAT(real, expected, marg, msg)                                    \
	do {                                                                               \
		float _r = (real);                                                             \
		float _e = (expected);                                                         \
		if (fabs(_r - _e) <= (marg)) {                                                 \
			g_testsChecksPassed++;                                                     \
		} else {                                                                       \
			g_testsChecksFailed++;                                                     \
			printf("  FAIL: %s\n", (msg));                                             \
			printf("    Expected: %f, Got: %f (margin: %f)\n", _e, _r, (float)(marg)); \
			printf("    Location: %s:%d\n", __FILE__, __LINE__);                       \
		}                                                                              \
	} while (0)

// Run all registered tests and print summary
void TestSystem_RunAll();

#endif	// __TESTSYSTEM_H__
