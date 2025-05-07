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

#if !defined(LIST_DELIM)
# define LIST_DELIM ":"
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

// 'list' is a colon-separated (or otherwise if 'LIST_DELIM' is set) list of component names
// returns true if 'component' is in said list, false otherwise

static bool component_in_list(char const* list, char const* component) {
	char* dup_list = strdup(list);

	if (!dup_list) {
		return false;
	}

	char* tok;

	while ((tok = strsep(&dup_list, LIST_DELIM))) {
		if (!strcmp(tok, component)) {
			return true;
		}
	}

	free(dup_list);

	return false;
}

void umber_log(umber_lvl_t const lvl, char const* const component, char const* const path, char const* const func, uint32_t const line, char const* const msg) {
	// check log level and compare it to 'UMBER_LVL' envvar

	char* const lvl_env = getenv("UMBER_LVL");
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

	// check component filter (blacklist/whitelist/nothing)
	// these different filter types are mutually exclusive; you can't have a blacklist & a whitelist at the same time, that would be preposterous, even silly!

	char* const whitelist = getenv("UMBER_WHITELIST");
	char* const blacklist = getenv("UMBER_BLACKLIST");

	if (whitelist && !component_in_list(whitelist, component)) {
		return;
	}

	else if (blacklist && component_in_list(blacklist, component)) {
		return;
	}

	// get information about the log level (colour, output stream, & level string)

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

	default:

		LVL_CASE(FATAL, stderr, PURPLE)
		LVL_CASE(ERROR, stderr, RED)
		LVL_CASE(WARN, stderr, YELLOW)
		LVL_CASE(SUCCESS, stdout, GREEN)
		LVL_CASE(INFO, stdout, BLUE)
		LVL_CASE(VERBOSE, stdout, GREY)

#undef LVL_CASE
	}

	// truncate file path

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

	// actually log

	char const* const elipsis = truncated_path == path ? "" : ".../";

	fprintf(fp, BOLD "%s[%s %s -> %s%s:%d -> %s]" REGULAR "%s %s" CLEAR "\n", colour, lvl_str, component, elipsis, truncated_path, line, func, colour, msg);
}

void umber_vlog(umber_lvl_t const lvl, char const* const component, char const* const path, char const* const func, uint32_t const line, char const* const fmt, ...) {
	va_list args;
	va_start(args, fmt);

	char* msg;
	vasprintf(&msg, fmt, args);

	if (!msg) {
		umber_log(UMBER_LVL_FATAL, "umber", __FILE__, __func__, __LINE__, "Failed to allocate space for log message");
		return;
	}

	va_end(args);

	umber_log(lvl, component, path, func, line, msg);
	free(msg);
}
