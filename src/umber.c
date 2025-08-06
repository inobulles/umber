// SPDX-License-Identifier: MIT
// Copyright (c) 2022-2025 Aymeric Wibo

#define __STDC_WANT_LIB_EXT2__ 1 // ISO/IEC TR 24731-2:2010 standard library extensions

#if __linux__
# define _GNU_SOURCE
#endif

#include "umber.h"

#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if !defined(PATH_TRUNCATION_LEN)
# define PATH_TRUNCATION_LEN 30
#endif

#define CLEAR "\033[0m"
#define REGULAR "\033[0;"
#define BOLD "\033[1;"

#define PURPLE "35m"
#define RED "31m"
#define YELLOW "33m"
#define GREEN "32m"
#define BLUE "34m"
#define GREY "37m"

struct umber_class_t {
	char name[UMBER_CLASS_NAME_MAX];
	char description[UMBER_CLASS_DESCRIPTION_MAX];
	umber_lvl_t default_lvl;
	umber_lvl_t lvl;
};

umber_class_t const* umber_class_new(
	char const name[UMBER_CLASS_NAME_MAX],
	umber_lvl_t const default_lvl,
	char const description[UMBER_CLASS_DESCRIPTION_MAX]
) {
	// Validate name.

	for (size_t i = 0; i < UMBER_CLASS_NAME_MAX - 1 && name[i] != '\0'; i++) {
		if (name[i] == ',' || name[i] == '=') {
			return NULL;
		}
	}

	// Create class.

	umber_class_t* const c = malloc(sizeof(umber_class_t));

	if (c == NULL) {
		return NULL;
	}

	strncpy(c->name, name, sizeof c->name - 1);
	strncpy(c->description, description, sizeof c->description - 1);
	c->default_lvl = default_lvl;

	// Check UMBER_LVL environment variable and set the log level accordingly.

	c->lvl = default_lvl;
	char* const lvl_env = getenv("UMBER_LVL");

	if (lvl_env == NULL) {
		return c;
	}

	char* dup_lvl_env = strdup(lvl_env);
	char* tok;

	while ((tok = strsep(&dup_lvl_env, ",")) != NULL) {
		char* const eq = strchr(tok, '=');
		*eq = '\0';

		char const* const tok_name = tok;
		char const* const tok_lvl = eq + 1;

		bool const globbed_end = *(eq - 1) == '*';

		if (!globbed_end && strcmp(tok_name, c->name) != 0) {
			continue;
		}

		if (globbed_end && strncmp(tok_name, c->name, strlen(tok_name) - 1) != 0) {
			continue;
		}

		// Past this point, we know that the class name matches.

		switch (*tok_lvl) {
		case '\0':
			c->lvl = c->default_lvl;
			break;
		case '0':
		case 'n':
			c->lvl = UMBER_LVL_NEVER;
		case '1':
		case 'f':
			c->lvl = UMBER_LVL_FATAL;
			break;
		case '2':
		case 'e':
			c->lvl = UMBER_LVL_ERROR;
			break;
		case '3':
		case 'w':
			c->lvl = UMBER_LVL_WARN;
			break;
		case '4':
		case 's':
			c->lvl = UMBER_LVL_SUCCESS;
			break;
		case '5':
		case 'i':
			c->lvl = UMBER_LVL_INFO;
			break;
		case '6':
		case 'v':
			c->lvl = UMBER_LVL_VERBOSE;
			break;
		default:
			continue;
		}
	}

	return c;
}

void umber_log(
	umber_class_t const* const cls,
	umber_lvl_t const lvl,
	char const* const path,
	uint32_t const line,
	char const* const msg
) {
	// Drop message if the log level is lower than the class's log level.

	if (lvl < cls->lvl) {
		return;
	}

	// Get information about the log level (colour, output stream, & level string).

	char* lvl_str;
	FILE* fp;
	char* colour;

	switch (lvl) {
#define LVL_CASE(name, _fp, _colour) \
	case UMBER_LVL_##name: {          \
		lvl_str = #name;               \
		fp = (_fp);                    \
		colour = (_colour);            \
                                     \
		break;                         \
	}

		LVL_CASE(FATAL, stderr, PURPLE)
		LVL_CASE(ERROR, stderr, RED)
		LVL_CASE(WARN, stderr, YELLOW)
		LVL_CASE(SUCCESS, stdout, GREEN)
		LVL_CASE(INFO, stdout, BLUE)
		LVL_CASE(VERBOSE, stdout, GREY)

#undef LVL_CASE

	default:
		return;
	}

	// Truncate file path.

	char const* truncated_path = path;
	size_t len = strlen(path);

	while (len > PATH_TRUNCATION_LEN) {
		char const* const new_path = strchr(truncated_path, '/');

		if (!new_path) {
			break;
		}

		truncated_path = new_path + 1;
		len = strlen(truncated_path);
	}

	// Actually log.

	char const* const ellipsis = truncated_path == path ? "" : ".../";

	fprintf(
		fp,
		BOLD "%s[%s %s -> %s%s:%d]" REGULAR "%s %s" CLEAR "\n",
		colour,
		lvl_str,
		cls->name,
		ellipsis,
		truncated_path,
		line,
		colour,
		msg
	);
}

void umber_vlog(umber_class_t const* cls, umber_lvl_t const lvl, char const* const path, uint32_t const line, char const* const fmt, ...) {
	va_list args;
	va_start(args, fmt);

	char* msg;
	vasprintf(&msg, fmt, args);

	if (msg == NULL) {
		umber_log(cls, UMBER_LVL_FATAL, __FILE__, __LINE__, "Failed to allocate space for log message");
		return;
	}

	va_end(args);

	umber_log(cls, lvl, path, line, msg);
	free(msg);
}
