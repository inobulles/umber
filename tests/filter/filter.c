#include <umber.h>

int main(void) {
	umber_class_t const* const cls_a = umber_class_new(CLASS_A, UMBER_LVL_VERBOSE, "Class A");
	umber_class_t const* const cls_b = umber_class_new(CLASS_B, UMBER_LVL_VERBOSE, "Class B");
	umber_class_t const* const cls_c = umber_class_new(CLASS_C, UMBER_LVL_VERBOSE, "Class C");

	LOG_V(cls_a, "%s", SPEC_A); // Line 8.
	LOG_V(cls_b, "%s", SPEC_B); // Line 9.
	LOG_V(cls_c, "%s", SPEC_C); // Line 10.

	return 0;
}
