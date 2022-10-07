#undef UMBER_COMPONENT // force user to define this after header inclusion

#if !defined(__UMBER__)
#define __UMBER__

#include <stdint.h>

typedef enum {
	UMBER_LVL_FATAL,
	UMBER_LVL_ERROR,
	UMBER_LVL_WARN,
	UMBER_LVL_SUCCESS,
	UMBER_LVL_INFO,
	UMBER_LVL_VERBOSE,
} umber_lvl_t;

void umber_log(umber_lvl_t lvl, char const* component, char const* path, char const* func, uint32_t line, char const* msg);

__attribute__((__format__(__printf__, 6, 0)))
void umber_vlog(umber_lvl_t lvl, char const* component, char const* path, char const* func, uint32_t line, char const* fmt, ...);

// helper macros

#define LOG_FATAL(...)   umber_vlog(UMBER_LVL_FATAL,   UMBER_COMPONENT, __FILE__, __func__, __LINE__, __VA_ARGS__);
#define LOG_ERROR(...)   umber_vlog(UMBER_LVL_ERROR,   UMBER_COMPONENT, __FILE__, __func__, __LINE__, __VA_ARGS__);
#define LOG_WARN(...)    umber_vlog(UMBER_LVL_WARN,    UMBER_COMPONENT, __FILE__, __func__, __LINE__, __VA_ARGS__);
#define LOG_SUCCESS(...) umber_vlog(UMBER_LVL_SUCCESS, UMBER_COMPONENT, __FILE__, __func__, __LINE__, __VA_ARGS__);
#define LOG_INFO(...)    umber_vlog(UMBER_LVL_INFO,    UMBER_COMPONENT, __FILE__, __func__, __LINE__, __VA_ARGS__);
#define LOG_VERBOSE(...) umber_vlog(UMBER_LVL_VERBOSE, UMBER_COMPONENT, __FILE__, __func__, __LINE__, __VA_ARGS__);

#endif
