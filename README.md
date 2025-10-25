# Umber

Logging library for everything in the AQUA ecosystem.

## Building

With [Bob the Builder](https://github.com/inobulles/bob) installed:

```console
bob install
```

You can also depend on this library in another Bob project without having to install it by adding it to your dependencies list:

```py
deps = [
	Dep.git("https://github.com/inobulles/umber", "v0.3.0"),
]
```

## Basic usage

To use Umber in your project, include the `umber.h` header:

```c
#include <umber.h>
```

When compiling your program, you can link Umber as such:

```console
cc the-fitness-gram-pacer-test.c -lumber
```

(Notice the subtle play on words, which is difficult to notice at first; indeed, "Lumber" refers to processed wood, wood which is also what logs are made of.)

To start logging, you must first create a logging class:

```c
umber_class_t const* const cls = umber_class_new(
	"fully.qualified.class.name", // Fully-qualified class name.
	UMBER_LVL_VERBOSE, // Default log level.
	"The Fitness Gram™ Pacer Test", // Description.
);
```

You should only be creating these logging classes once, so you don't need to free them.
If you do want to you can just `free(cls);`.

The header provides the following macros you can use for various log levels:

|Macro  |Level|POSIX stream|Description|
|-------|-----|------------|-----------|
|`LOG_F`|1    |`stderr`    |An error occurred which prevents the component from continuing operation.|
|`LOG_E`|2    |`stderr`    |An error occurred which may impair some of the component's functionality.|
|`LOG_W`|3    |`stderr`    |Something is not necessarily wrong, but is a bit unexpected.|
|`LOG_I`|4    |`stdout`    |The component is doing something. Or is printing out relevant info.|
|`LOG_V`|5    |`stdout`    |Extremely detailed information about what the component is currently doing.|

These definitions are intentionally left a bit vague; the way you use these macros is mostly up to your own common sense.
The logging macros may be called as such:

```c
#include <umber.h>

int main(void) {
	umber_class_t const* const cls = umber_class_new(
		"fully.qualified.class.name",
		UMBER_LVL_VERBOSE,
		"The Fitness Gram™ Pacer Test",
	);

	LOG_F(cls, "Oh the humanity.");
	LOG_E(cls, "Oh the misery.");
	LOG_W(cls, "Oh the shame.");
	LOG_I(cls, "Oh there's my wine toaster.");
	LOG_V(cls, "My favourite number is %d.", *(volatile int*) 0);

	return 0;
}
```

Logging classes are meant to cover parts of your program which you might want to set different rules for logging for.
For example, you might want a logging class for your programs initialization which defaults to being very verbose, but then use a different logging class for your programs main loop which only shows errors and warnings by default to avoid overwhelming the user with information.

Then, if you needed to debug an issue in your loop code, you could set that logging class to be verbose temporarily.

## Filtering

You can override the default log level of a logging class by setting the `UMBER_LVL` environment variable.

For example, if you wanted to only print warnings and errors in the above example, you could do one of these:

```console
UMBER_LVL=fully.qualified.class.name=warn
UMBER_LVL=fully.qualified.class.name=w
UMBER_LVL=fully.qualified.class.name=3
```

You can even glob the end of class names to capture children:

```console
UMBER_LVL=fully.*=w
```

Or, you could implement a whitelist by first setting the log level to all classes to never, and then selecting a specific class to enable:

```console
UMBER_LVL=*=never,fully.qualified.class.name=verbose
```

These assignments are evaluated left-to-right, so latter ones will take precedence.

## Enabling line buffering

If you need to enable line buffering (e.g. you are reading the output of a program using Umber through a pipe let's say and you need `getline()` to return immediately when a new line is produced), you can use the `UMBER_LINEBUF` environment variable:

```console
UMBER_LINEBUF=true # This can be set to whatever except "", "0", or "false".
```

This will enable line buffering on both `stdout` and `stderr`.

Thank you for attending my TED Talk.
