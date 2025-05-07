#include <umber.h>
#define UMBER_COMPONENT COMPONENT

void FUNC_NAME(void) {
	LOG_FATAL(TEST_FMT, TEST_ARG)   // line 5
	LOG_ERROR(TEST_FMT, TEST_ARG)   // line 6
	LOG_WARN(TEST_FMT, TEST_ARG)    // line 7
	LOG_SUCCESS(TEST_FMT, TEST_ARG) // line 8
	LOG_INFO(TEST_FMT, TEST_ARG)    // line 9
	LOG_VERBOSE(TEST_FMT, TEST_ARG) // line 10
}

int main(void) {
	FUNC_NAME();
	return 0;
}
