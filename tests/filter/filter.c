#include <umber.h>

void FUNC_NAME(void) {
#define UMBER_COMPONENT COMP_A
	LOG_VERBOSE("%s", SPEC_A); // line 5
#undef UMBER_COMPONENT

#define UMBER_COMPONENT COMP_B
	LOG_VERBOSE("%s", SPEC_B); // line 9
#undef UMBER_COMPONENT

#define UMBER_COMPONENT COMP_C
	LOG_VERBOSE("%s", SPEC_C); // line 13
}

int main(void) {
	FUNC_NAME();
	return 0;
}
