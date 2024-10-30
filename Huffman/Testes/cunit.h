/*
 * File: cunit.h
 * Name: Jaroslaw Bielski (bielski.j@gmail.com)
 * Date: 01/22/2020
 */

#ifndef CUNIT_H_
#define CUNIT_H_

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <stddef.h>
#include <stdint.h>
#include <inttypes.h>

#if !defined IS_NULL
#define IS_NULL(x)  ((x) == ((void *) 0))
#endif
#if !defined NOT_NULL
#define NOT_NULL(x) (! IS_NULL(x))
#endif
#define _STR(x) (#x)

#define CUNIT_NAME_LENGTH_MAX 64

#define CUNIT_CTX_NAME       __cunitGlobals
#define CUNIT_CTX            CUnitContext CUNIT_CTX_NAME;
#define CUNIT_CTX_EXT extern CUnitContext CUNIT_CTX_NAME;

#define CUNIT_ATTR_WEAK        __attribute__((weak))
#define CUNIT_ATTR_CONSTRUCTOR __attribute__((constructor))

#define CUNIT_COLOR_GREEN   "\e[32m"
#define CUNIT_COLOR_RED     "\e[31m"
#define CUNIT_COLOR_DEFAULT "\e[0m"

#define CUNIT_STRING_OK     CUNIT_COLOR_GREEN "OK    " CUNIT_COLOR_DEFAULT
#define CUNIT_STRING_PASSED CUNIT_COLOR_GREEN "PASSED" CUNIT_COLOR_DEFAULT
#define CUNIT_STRING_FAILED CUNIT_COLOR_RED   "FAILED" CUNIT_COLOR_DEFAULT

typedef struct _CUnitTestResult {
	int ret;
	int asserts;
	int expects;
} CUnitTestResult;


typedef void (*CUnitTestRoutine)(CUnitTestResult *result);


typedef struct _CUnitTest {
	char             name[CUNIT_NAME_LENGTH_MAX + 1];
	CUnitTestRoutine routine;

	int            (*init)();
	void           (*term)();

	struct _CUnitTest *next;
} CUnitTest;


typedef struct _CUnitTestGroup {
	char name[CUNIT_NAME_LENGTH_MAX + 1];
	int  testCount;

	struct _CUnitTestGroup *next;
	struct _CUnitTest      *testHead;
	struct _CUnitTest      *testTail;
} CUnitTestGroup;


typedef struct _CUnitContext {
	CUnitTestGroup *groups;

	int totalTests;
	int executedTests;
	int groupCount;

	int passed;
	int failed;
	int skipped;
} CUnitContext;


#define PRINTF(__format...) fprintf(stdout, __format)

#define CUNIT_MSG(__format...) { fprintf(stdout, "MSG: "); fprintf(stdout, __format); fprintf(stdout, "\n"); }


#define CUNIT_TEST_BASIC(__cond, __val, __assert) { \
	if (__assert) { \
		_cunitResult->asserts++; \
	} else { \
		_cunitResult->expects++; \
	} \
	\
	if ((__cond) != __val) { \
		if (__assert) { \
			PRINTF("%s:%d: assertion failed!\n", __FILE__, __LINE__); \
		} else { \
			PRINTF("%s:%d: unexpected value!\n", __FILE__, __LINE__); \
		} \
		\
		PRINTF("   Value of '" #__cond "' is %s (expected: %s)\n", __val ? "FALSE" : "TRUE", __val ? "TRUE" : "FALSE"); \
		_cunitResult->ret = 1; \
		\
		if (__assert) { \
			return; \
		} \
	} \
}


#define CUNIT_ASSERT_TRUE(__cond)  CUNIT_TEST_BASIC(__cond, (1 == 1), 1)
#define CUNIT_ASSERT_FALSE(__cond) CUNIT_TEST_BASIC(__cond, (1 != 1), 1)

#define CUNIT_EXPECT_TRUE(__cond)  CUNIT_TEST_BASIC(__cond, (1 == 1), 0)
#define CUNIT_EXPECT_FALSE(__cond) CUNIT_TEST_BASIC(__cond, (1 != 1), 0)

#ifdef __CDT_PARSER__
	#define __type_print(_val)

#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 201112L)
	#define __type_print(_val) PRINTF(_Generic((_val), \
		int                : "%d", \
		long               : "%ld",  \
		long long          : "%lld", \
		unsigned           : "%u", \
		unsigned long      : "%lu", \
		unsigned long long : "%llu", \
		float              : "%f", \
		double             : "%f", \
		long double        : "%Lf", \
		void *             : "%p", \
		uint8_t            : "%" PRIu8, \
		default            : "undef"), \
		(_val))
#else
	void __type_print(int val) CUNIT_ATTR_WEAK;
	void __type_print(int val) { PRINTF("%d", val); }

	void __type_print(long val) CUNIT_ATTR_WEAK;
	void __type_print(long val) { PRINTF("%ld", val); }

	void __type_print(long long val) CUNIT_ATTR_WEAK;
	void __type_print(long long val) { PRINTF("%lld", val); }

	void __type_print(float val) CUNIT_ATTR_WEAK;
	void __type_print(float val) { PRINTF("%f", val); }

	void __type_print(double val) CUNIT_ATTR_WEAK;
	void __type_print(double val) { PRINTF("%f", val); }

	void __type_print(long double val) CUNIT_ATTR_WEAK;
	void __type_print(long double val) { PRINTF("%Lf", val); }

	void __type_print(uint32_t val) CUNIT_ATTR_WEAK;
	void __type_print(uint32_t val) { PRINTF("%" PRIu32, val); }

	void __type_print(size_t val) CUNIT_ATTR_WEAK;
	void __type_print(size_t val) { PRINTF("%zd", val); }

	void __type_print(const void *val) CUNIT_ATTR_WEAK;
	void __type_print(const void *val) { PRINTF("%p", val); }
#endif

#ifdef __cplusplus
	#if __cplusplus >= 201103L || (defined(_MSC_VER) && _MSC_VER >= 1900)
	#define __typeof_word decltype
#else
	#define __typeof_word typeof
#endif

#else
	#define __typeof_word typeof
#endif

#define CUNIT_TEST_BINARY(__val1, __val2, __op, __assert) { \
	if (__assert) { \
		_cunitResult->asserts++; \
	} else { \
		_cunitResult->expects++; \
	} \
	\
	if (! ((__val1) __op (__val2))) { \
		__typeof_word (__val1) v1 = (__val1); \
		__typeof_word (__val2) v2 = (__val2); \
		\
		if (__assert) { \
			PRINTF("%s:%d: assertion failed!\n", __FILE__, __LINE__); \
		} else { \
			PRINTF("%s:%d: unexpected value!\n", __FILE__, __LINE__); \
		} \
		\
		PRINTF("   Expected: " #__op " "); __type_print(v2); PRINTF("\n"); \
		PRINTF("   Have:     "); __type_print(v1); PRINTF("\n"); \
		_cunitResult->ret = 1; \
		\
		if (__assert) { \
			return; \
		} \
	} \
}

#define CUNIT_ASSERT_EQ(__val1, __val2) CUNIT_TEST_BINARY(__val1, __val2, ==, 1)
#define CUNIT_ASSERT_NE(__val1, __val2) CUNIT_TEST_BINARY(__val1, __val2, !=, 1)
#define CUNIT_ASSERT_LT(__val1, __val2) CUNIT_TEST_BINARY(__val1, __val2,  <, 1)
#define CUNIT_ASSERT_LE(__val1, __val2) CUNIT_TEST_BINARY(__val1, __val2, <=, 1)
#define CUNIT_ASSERT_GT(__val1, __val2) CUNIT_TEST_BINARY(__val1, __val2,  >, 1)
#define CUNIT_ASSERT_GE(__val1, __val2) CUNIT_TEST_BINARY(__val1, __val2, >=, 1)

#define CUNIT_EXPECT_EQ(__val1, __val2) CUNIT_TEST_BINARY(__val1, __val2, ==, 0)
#define CUNIT_EXPECT_NE(__val1, __val2) CUNIT_TEST_BINARY(__val1, __val2, !=, 0)
#define CUNIT_EXPECT_LT(__val1, __val2) CUNIT_TEST_BINARY(__val1, __val2,  <, 0)
#define CUNIT_EXPECT_LE(__val1, __val2) CUNIT_TEST_BINARY(__val1, __val2, <=, 0)
#define CUNIT_EXPECT_GT(__val1, __val2) CUNIT_TEST_BINARY(__val1, __val2,  >, 0)
#define CUNIT_EXPECT_GE(__val1, __val2) CUNIT_TEST_BINARY(__val1, __val2, >=, 0)

#define CUNIT_TEST_STRING(__str1, __str2, __comp, __op, __val, __assert) { \
	if (__assert) { \
		_cunitResult->asserts++; \
	} else { \
		_cunitResult->expects++; \
	} \
	\
	if (! (__comp((__str1), (__str2)) __op __val)) { \
		if (__assert) { \
			PRINTF("%s:%d: assertion failed!\n", __FILE__, __LINE__); \
		} else { \
			PRINTF("%s:%d: unexpected value!\n", __FILE__, __LINE__); \
		} \
		\
		PRINTF("   Expected: '%s'\n", (__str1)); \
		PRINTF("   Have:     '%s'\n", (__str2)); \
		_cunitResult->ret = 1; \
		\
		if (__assert) { \
			return; \
		} \
	} \
}

#define CUNIT_ASSERT_STREQ(__str1, __str2)     CUNIT_TEST_STRING(__str1, __str2, strcmp,     ==, 0, 1)
#define CUNIT_ASSERT_STRNE(__str1, __str2)     CUNIT_TEST_STRING(__str1, __str2, strcmp,     !=, 0, 1)
#define CUNIT_ASSERT_STRCASEEQ(__str1, __str2) CUNIT_TEST_STRING(__str1, __str2, strcasecmp, ==, 0, 1)
#define CUNIT_ASSERT_STRCASENE(__str1, __str2) CUNIT_TEST_STRING(__str1, __str2, strcasecmp, !=, 0, 1)

#define CUNIT_EXPECT_STREQ(__str1, __str2)     CUNIT_TEST_STRING(__str1, __str2, strcmp,     ==, 0, 0)
#define CUNIT_EXPECT_STRNE(__str1, __str2)     CUNIT_TEST_STRING(__str1, __str2, strcmp,     !=, 0, 0)
#define CUNIT_EXPECT_STRCASEEQ(__str1, __str2) CUNIT_TEST_STRING(__str1, __str2, strcasecmp, ==, 0, 0)
#define CUNIT_EXPECT_STRCASENE(__str1, __str2) CUNIT_TEST_STRING(__str1, __str2, strcasecmp, !=, 0, 0)

#define __CUNIT_TEST_FUNC_NAME(_group, _name, _type) cunit_test_ ## _group ## _ ## _name ## _ ## _type

#define CUNIT_TEST_EX(__group, __name, __init, __term) \
	CUNIT_CTX_EXT; \
	\
	static void __CUNIT_TEST_FUNC_NAME(__group, __name, main) (CUnitTestResult *_cunitResult); \
	static void __CUNIT_TEST_FUNC_NAME(__group, __name, init) () CUNIT_ATTR_CONSTRUCTOR; \
	static void __CUNIT_TEST_FUNC_NAME(__group, __name, init) () { \
		CUnitTestGroup *group = CUNIT_CTX_NAME.groups; \
		\
		while (NOT_NULL(group)) { \
			if (strcmp(group->name, #__group) == 0) { \
				break; \
			} \
			group = group->next; \
		} \
		\
		if (IS_NULL(group)) {\
			group = (CUnitTestGroup *) malloc(sizeof(*group));\
			if (NOT_NULL(group)) { \
				memset(group, 0, sizeof(*group));\
				\
				group->next = CUNIT_CTX_NAME.groups;\
				strncpy(group->name, #__group, CUNIT_NAME_LENGTH_MAX); \
				\
				CUNIT_CTX_NAME.groups = group;\
				CUNIT_CTX_NAME.groupCount++; \
			} \
		} \
		\
		if (NOT_NULL(group)) { \
			CUnitTest *test = (CUnitTest *) malloc(sizeof(*test)); \
			if (NOT_NULL(test)) { \
				memset(test, 0, sizeof(*test));\
				\
				test->next    = 0;\
				test->routine = __CUNIT_TEST_FUNC_NAME(__group, __name, main); \
				test->init    = __init; \
				test->term    = __term; \
				strncpy(test->name, #__name, CUNIT_NAME_LENGTH_MAX); \
				\
				if (IS_NULL(group->testHead)) { \
					group->testHead = group->testTail = test; \
					\
				} else { \
					group->testTail->next = test; \
					group->testTail       = test; \
				} \
				\
				group->testCount++; \
				\
				CUNIT_CTX_NAME.totalTests++; \
			} \
		} \
	}; \
\
	static void __CUNIT_TEST_FUNC_NAME(__group, __name, main) (CUnitTestResult *_cunitResult)

#define CUNIT_TEST(__group, __name) CUNIT_TEST_EX(__group, __name, NULL, NULL)

static void _cunit_execute_test(CUnitContext *_cunitCtx, CUnitTestGroup *_cunitGroup, CUnitTest *_cunitTest) {
	CUnitTestResult _cunitResult;

	memset(&_cunitResult, 0, sizeof(_cunitResult));

	PRINTF("[ RUNNING ] Test %s:%s\n", _cunitGroup->name, _cunitTest->name);

	{
		int initRet = 0;

		if (_cunitTest->init != NULL) {
			initRet = _cunitTest->init();
			if (initRet != 0) {
				CUNIT_MSG("Initialize method has failed!");
			}
		}

		if (initRet == 0) {
			_cunitTest->routine(&_cunitResult);
		}

		if (initRet == 0) {
			if (_cunitTest->term != NULL) {
				_cunitTest->term();
			}

		} else {
			_cunitResult.ret = initRet;
		}
	}

	if (_cunitResult.ret == 0) {
		_cunitCtx->passed++;

	} else {
		_cunitCtx->failed++;
	}

	_cunitCtx->executedTests++;

	PRINTF("[ %s  ] Test %s:%s, asserts: %d, expectations: %d\n",
		(_cunitResult.ret == 0) ? CUNIT_STRING_OK : CUNIT_STRING_FAILED, _cunitGroup->name, _cunitTest->name,
		_cunitResult.asserts, _cunitResult.expects
	);
}


static void _cunit_execute_group(CUnitContext *ctx, CUnitTestGroup *group) {
	CUnitTest *test = group->testHead;

	while (NOT_NULL(test)) {
		_cunit_execute_test(ctx, group, test);

		test = test->next;
	}

	PRINTF("[---------]\n");
}


#ifdef __cplusplus
int cunit_main(int argc, char *argv[]) CUNIT_ATTR_WEAK;
int cunit_main(int argc, char *argv[]) {
#else
int cunit_main(int argc, char *argv[]) CUNIT_ATTR_WEAK;
int cunit_main(int argc, char *argv[]) {
#endif
	CUNIT_CTX_EXT;

	if (argc > 1) {
		int opt;

		while ((opt = getopt(argc, argv, "t:g:")) != -1) {
			switch (opt) {
				case 't':
					{
						CUnitTestGroup *group = CUNIT_CTX_NAME.groups;

						while (NOT_NULL(group)) {
							CUnitTest *test = group->testHead;

							while (NOT_NULL(test)) {
								if (strcmp(test->name, optarg) == 0) {
									_cunit_execute_test(&CUNIT_CTX_NAME, group, test);
								}

								test = test->next;
							}

							group = group->next;
						}
					}
					break;

				case 'g':
					{
						CUnitTestGroup *group = CUNIT_CTX_NAME.groups;

						while (NOT_NULL(group)) {
							if (strcmp(group->name, optarg) == 0) {
								_cunit_execute_group(&CUNIT_CTX_NAME, group);
							}

							group = group->next;
						}
					}
					break;

				default: /* '?' */
					fprintf(stderr, "Usage: %s [-t test_name,test_name,...] [-g group_name,group_name,..]\n", argv[0]);

					exit(EXIT_FAILURE);
			}
		}

	} else {
		CUnitTestGroup *group = CUNIT_CTX_NAME.groups;

		PRINTF("[=========] Running %d test%c from %d group%c\n",
			CUNIT_CTX_NAME.totalTests,
			CUNIT_CTX_NAME.totalTests == 1 ? '\0' : 's', CUNIT_CTX_NAME.groupCount,
			CUNIT_CTX_NAME.groupCount == 1 ? '\0' : 's'
		);

		while (NOT_NULL(group)) {
			_cunit_execute_group(&CUNIT_CTX_NAME, group);

			group = group->next;
		}
	}

	PRINTF("[=========]\n");
	PRINTF("[ %s  ] Executed %d tests, passed: %d, failed: %d\n",
		CUNIT_CTX_NAME.failed > 0 ? CUNIT_STRING_FAILED : CUNIT_STRING_PASSED,
		CUNIT_CTX_NAME.executedTests, CUNIT_CTX_NAME.passed, CUNIT_CTX_NAME.failed
	);

	return CUNIT_CTX_NAME.failed > 0 ? 1 : 0;
}

#define CUNIT_GLOBALS CUnitContext CUNIT_CTX_NAME = { 0 };
#define CUNIT_MAIN() \
	int main(int argc, char *argv[]) { \
		return cunit_main(argc, argv); \
	} \


#endif /* CUNIT_H_ */