#include <umber.h>
#include <stdio.h>

int main(void) {
	umber_class_t const* const cls = umber_class_new(CLASS_NAME, UMBER_LVL_VERBOSE, "");

	fprintf(stderr, "\n"); // Dummy space so we can use head -n1 to get no output.

	LOG_F(cls, TEST_FMT, TEST_ARG); // line 9
	LOG_E(cls, TEST_FMT, TEST_ARG); // line 10
	LOG_W(cls, TEST_FMT, TEST_ARG); // line 11
	LOG_I(cls, TEST_FMT, TEST_ARG); // line 12
	LOG_V(cls, TEST_FMT, TEST_ARG); // line 13

	return 0;
}
