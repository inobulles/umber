// SPDX-License-Identifier: MIT
// Copyright (c) 2022-2025 Aymeric Wibo

#undef UMBER_COMPONENT // Force user to define this after header inclusion.
#pragma once

#include <stdint.h>

/**
 * Umber log levels.
 */
typedef enum {
	/**
	 * Fatal error.
	 *
	 * An error occurred which prevents the component from continuing operation.
	 */
	UMBER_LVL_FATAL,
	/**
	 * Error.
	 *
	 * An error occurred which may impair some of the component's functionality.
	 */
	UMBER_LVL_ERROR,
	/**
	 * Warning.
	 *
	 * Something is not necessarily wrong, but is a bit unexpected.
	 */
	UMBER_LVL_WARN,
	/**
	 * Success message.
	 *
	 * The component executed a large operation successfully.
	 */
	UMBER_LVL_SUCCESS,
	/**
	 * Informational message.
	 *
	 * The component is doing something.
	 */
	UMBER_LVL_INFO,
	/**
	 * Verbose message.
	 *
	 * Extremely detailed information about what the component is currently doing.
	 * This information is to be used for tracing.
	 */
	UMBER_LVL_VERBOSE,
} umber_lvl_t;

/**
 * Log a message.
 *
 * You should probably use the LOG_* macros instead of using this function directly.
 *
 * @param lvl Log level.
 * @param component Component name.
 * @param path Path to the source file the log message is coming from.
 * @param func Function name the log message is coming from.
 * @param line Line number the log message is coming from.
 * @param msg Message to log.
 */
void umber_log(umber_lvl_t const lvl, char const* const component, char const* const path, char const* const func, uint32_t const line, char const* const msg);

/**
 * Log a message with a format string.
 *
 * You should probably use the LOG_* macros instead of using this function directly.
 * This wraps {@link umber_log}.
 *
 * @param lvl Log level.
 * @param component Component name.
 * @param path Path to the source file the log message is coming from.
 * @param func Function name the log message is coming from.
 * @param line Line number the log message is coming from.
 * @param fmt Format string.
 * @param ... Arguments to the format string.
 */
__attribute__((__format__(__printf__, 6, 0))) void umber_vlog(umber_lvl_t const lvl, char const* const component, char const* const path, char const* const func, uint32_t const line, char const* const fmt, ...);

// Helper macros.

/**
 * Log a fatal error message.
 *
 * An error occurred which prevents the component from continuing operation.
 * You may pass a format string and arguments to this as you would {@link printf}.
 * {@link UMBER_COMPONENT} must be set for the component name to be set correctly.
 *
 * @param ... Message to log.
 */
#define LOG_FATAL(...) umber_vlog(UMBER_LVL_FATAL, UMBER_COMPONENT, __FILE__, __func__, __LINE__, __VA_ARGS__)

/**
 * Log an error message.
 *
 * An error occurred which may impair some of the component's functionality.
 * You may pass a format string and arguments to this as you would {@link printf}.
 * {@link UMBER_COMPONENT} must be set for the component name to be set correctly.
 *
 * @param ... Message to log.
 */
#define LOG_ERROR(...) umber_vlog(UMBER_LVL_ERROR, UMBER_COMPONENT, __FILE__, __func__, __LINE__, __VA_ARGS__)

/**
 * Log a warning message.
 *
 * Something is not necessarily wrong, but is a bit unexpected.
 * You may pass a format string and arguments to this as you would {@link printf}.
 * {@link UMBER_COMPONENT} must be set for the component name to be set correctly.
 *
 * @param ... Message to log.
 */
#define LOG_WARN(...) umber_vlog(UMBER_LVL_WARN, UMBER_COMPONENT, __FILE__, __func__, __LINE__, __VA_ARGS__)

/**
 * Log a success message.
 *
 * The component executed a large operation successfully.
 * You may pass a format string and arguments to this as you would {@link printf}.
 * {@link UMBER_COMPONENT} must be set for the component name to be set correctly.
 *
 * @param ... Message to log.
 */
#define LOG_SUCCESS(...) umber_vlog(UMBER_LVL_SUCCESS, UMBER_COMPONENT, __FILE__, __func__, __LINE__, __VA_ARGS__)

/**
 * Log an informational message.
 *
 * The component is doing something.
 * You may pass a format string and arguments to this as you would {@link printf}.
 * {@link UMBER_COMPONENT} must be set for the component name to be set correctly.
 *
 * @param ... Message to log.
 */
#define LOG_INFO(...) umber_vlog(UMBER_LVL_INFO, UMBER_COMPONENT, __FILE__, __func__, __LINE__, __VA_ARGS__)

/**
 * Log a verbose message.
 *
 * Extremely detailed information about what the component is currently doing.
 * This information is to be used for tracing.
 * You may pass a format string and arguments to this as you would {@link printf}.
 * {@link UMBER_COMPONENT} must be set for the component name to be set correctly.
 *
 * @param ... Message to log.
 */
#define LOG_VERBOSE(...) umber_vlog(UMBER_LVL_VERBOSE, UMBER_COMPONENT, __FILE__, __func__, __LINE__, __VA_ARGS__)
