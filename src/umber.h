// SPDX-License-Identifier: MIT
// Copyright (c) 2022-2025 Aymeric Wibo

#pragma once

#include <stdint.h>

/**
 * Umber log levels.
 *
 * RFC 5424 (syslog) maps surjectively on to these levels.
 */
typedef enum {
	/**
	 * Never log.
	 *
	 * This is used to disable logging for a class.
	 */
	UMBER_LVL_NEVER,
	/**
	 * Fatal error.
	 *
	 * An error occurred which prevents the component from continuing operation.
	 * This would be the equivalent to RFC 5424's "emergency", "alert", and "critical" levels (0, 1, and 2).
	 */
	UMBER_LVL_FATAL,
	/**
	 * Error.
	 *
	 * An error occurred which may impair some of the component's functionality.
	 * This would be the equivalent to RFC 5424's "error" level (3).
	 */
	UMBER_LVL_ERROR,
	/**
	 * Warning.
	 *
	 * Something is not necessarily wrong, but is a bit unexpected.
	 * This would be the equivalent to RFC 5424's "warning" level (4).
	 */
	UMBER_LVL_WARN,
	/**
	 * Informational message.
	 *
	 * The component is doing something.
	 * This would be the equivalent to RFC 5424's "notice" and "info" levels (5 and 6).
	 */
	UMBER_LVL_INFO,
	/**
	 * Verbose message.
	 *
	 * Extremely detailed information about what the component is currently doing.
	 * This information is to be used for tracing.
	 * This would be the equivalent to RFC 5424's "debug" level (7).
	 */
	UMBER_LVL_VERBOSE,
} umber_lvl_t;

/**
 * Maximum size for logging class names.
 */
#define UMBER_CLASS_NAME_MAX 32

/**
 * Maximum size for logging class descriptions.
 */
#define UMBER_CLASS_DESCRIPTION_MAX 128

/**
 * Logging class struct.
 *
 * A logging class groups logs together by common theme, e.g. so a user can filter out noisy verbose logs from a main loop but can keep them for initialization code.
 */
typedef struct umber_class_t umber_class_t;

/**
 * Create a new logging class.
 *
 * The name is meant to be the class' fully-qualified name, e.g. aqua.gvd.elp.
 * This name may not contain any ',', '=', or '*' characters.
 * The description is meant to be a short description of the class, e.g. "ELP sender.".
 * The default log level is the class' log level before user filtering, i.e. logs with levels lower or equal to this associated to this class will be logged by default.
 *
 * As long as you don't introduce a memory leak, you don't have to free this memory.
 * If you want/need to anyway, you can just use {@link free} on the returned pointer.
 *
 * @param name The class' fully-qualified name. Must be {@link UMBER_CLASS_NAME_MAX} characters or less, NULL-terminator included.
 * @param default_lvl The class' default log level.
 * @param description The class' description. Must be {@link UMBER_CLASS_DESCRIPTION_MAX} characters or less, NULL-terminator included.
 * @return A pointer to the new logging class or NULL if the class could not be created.
 */
umber_class_t const* umber_class_new(
	char const* name,
	umber_lvl_t const default_lvl,
	char const* description
);

/**
 * Log a message.
 *
 * You should use the LOG_* macros instead of using this function directly.
 *
 * @param lvl Log level.
 * @param component Component name.
 * @param path Path to the source file the log message is coming from.
 * @param line Line number the log message is coming from.
 * @param msg Message to log.
 */
void umber_log(
	umber_class_t const* const cls,
	umber_lvl_t const lvl,
	char const* const path,
	uint32_t const line,
	char const* const msg
);

/**
 * Log a message with a format string.
 *
 * You should use the LOG_* macros instead of using this function directly.
 * This wraps {@link umber_log}.
 *
 * @param lvl Log level.
 * @param component Component name.
 * @param path Path to the source file the log message is coming from.
 * @param line Line number the log message is coming from.
 * @param fmt Format string.
 * @param ... Arguments to the format string.
 */
__attribute__((__format__(__printf__, 5, 0))) void umber_vlog(
	umber_class_t const* const cls,
	umber_lvl_t const lvl,
	char const* const path,
	uint32_t const line,
	char const* const fmt,
	...
);

/**
 * Log a fatal error message.
 *
 * An error occurred which prevents the component from continuing operation.
 * You may pass a format string and arguments to this as you would {@link printf}.
 *
 * @param cls Logging class to use.
 * @param ... Message to log.
 */
#define LOG_F(cls, ...) umber_vlog((cls), UMBER_LVL_FATAL, __FILE__, __LINE__, __VA_ARGS__)

/**
 * Log an error message.
 *
 * An error occurred which may impair some of the component's functionality.
 * You may pass a format string and arguments to this as you would {@link printf}.
 *
 * @param cls Logging class to use.
 * @param ... Message to log.
 */
#define LOG_E(cls, ...) umber_vlog((cls), UMBER_LVL_ERROR, __FILE__, __LINE__, __VA_ARGS__)

/**
 * Log a warning message.
 *
 * Something is not necessarily wrong, but is a bit unexpected.
 * You may pass a format string and arguments to this as you would {@link printf}.
 *
 * @param cls Logging class to use.
 * @param ... Message to log.
 */
#define LOG_W(cls, ...) umber_vlog((cls), UMBER_LVL_WARN, __FILE__, __LINE__, __VA_ARGS__)

/**
 * Log an informational message.
 *
 * The component is doing something.
 * You may pass a format string and arguments to this as you would {@link printf}.
 *
 * @param cls Logging class to use.
 * @param ... Message to log.
 */
#define LOG_I(cls, ...) umber_vlog((cls), UMBER_LVL_INFO, __FILE__, __LINE__, __VA_ARGS__)

/**
 * Log a verbose message.
 *
 * Extremely detailed information about what the component is currently doing.
 * This information is to be used for tracing.
 * You may pass a format string and arguments to this as you would {@link printf}.
 *
 * @param cls Logging class to use.
 * @param ... Message to log.
 */
#define LOG_V(cls, ...) umber_vlog((cls), UMBER_LVL_VERBOSE, __FILE__, __LINE__, __VA_ARGS__)
