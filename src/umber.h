#undef UMBER_COMPONENT

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

void umber_log(umber_lvl_t lvl, const char* component, const char* path, const char* func, uint32_t line, char* msg);

__attribute__((__format__(__printf__, 6, 0)))
void umber_vlog(umber_lvl_t lvl, const char* component, const char* path, const char* func, uint32_t line, char* fmt, ...);

// helper macros

#define LOG_FATAL(...)   umber_vlog(UMBER_LVL_FATAL,   UMBER_COMPONENT, __FILE__, __func__, __LINE__, __VA_ARGS__);
#define LOG_ERROR(...)   umber_vlog(UMBER_LVL_ERROR,   UMBER_COMPONENT, __FILE__, __func__, __LINE__, __VA_ARGS__);
#define LOG_WARN(...)    umber_vlog(UMBER_LVL_WARN,    UMBER_COMPONENT, __FILE__, __func__, __LINE__, __VA_ARGS__);
#define LOG_SUCCESS(...) umber_vlog(UMBER_LVL_SUCCESS, UMBER_COMPONENT, __FILE__, __func__, __LINE__, __VA_ARGS__);
#define LOG_INFO(...)    umber_vlog(UMBER_LVL_INFO,    UMBER_COMPONENT, __FILE__, __func__, __LINE__, __VA_ARGS__);
#define LOG_VERBOSE(...) umber_vlog(UMBER_LVL_VERBOSE, UMBER_COMPONENT, __FILE__, __func__, __LINE__, __VA_ARGS__);

#endif