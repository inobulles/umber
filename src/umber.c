#include <umber.h>

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#define CLEAR   "\033[0m"
#define REGULAR "\033[0;"
#define BOLD    "\033[1;"

#define PURPLE  "35m"
#define RED     "31m"
#define YELLOW  "33m"
#define GREEN   "32m"
#define BLUE    "34m"
#define GREY    "37m"

void umber_log(umber_lvl_t lvl, const char* component, const char* path, const char* func, uint32_t line, char* msg) {
	char* lvl_env = getenv("UMBER_LVL");
	umber_lvl_t max_lvl = UMBER_LVL_SUCCESS;

	if (
		lvl_env &&
		*lvl_env >= '0' + UMBER_LVL_FATAL &&
		*lvl_env <= '0' + UMBER_LVL_VERBOSE
	) {
		max_lvl = *lvl_env - '0';
	}

	if (lvl > max_lvl) {
		return;
	}

	char* lvl_str;
	FILE* fp;
	char* colour;

	switch (lvl) {
		#define LVL_CASE(name, _fp, _colour) \
			case UMBER_LVL_##name: { \
				lvl_str = #name; \
				fp = (_fp); \
				colour = (_colour); \
				\
				break; \
			}

		default:

		LVL_CASE(FATAL,   stderr, PURPLE)
		LVL_CASE(ERROR,   stderr, RED   )
		LVL_CASE(WARN,    stderr, YELLOW)
		LVL_CASE(SUCCESS, stdout, GREEN )
		LVL_CASE(INFO,    stdout, BLUE  )
		LVL_CASE(VERBOSE, stdout, GREY  )

		#undef LVL_CASE
	}

	fprintf(fp, BOLD "%s[%s %s -> %s:%d -> %s]" REGULAR "%s %s" CLEAR "\n",
		colour, lvl_str, component, path, line, func, colour, msg);
}

__attribute__((__format__(__printf__, 6, 0)))
void umber_vlog(umber_lvl_t lvl, const char* component, const char* path, const char* func, uint32_t line, char* fmt, ...) {
	va_list args;
	va_start(args, fmt);

	char* msg;
	vasprintf(&msg, fmt, args);

	va_end(args);

	umber_log(lvl, component, path, func, line, msg);
	free(msg);
}