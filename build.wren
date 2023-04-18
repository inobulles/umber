// C compilation

var cc = CC.new()

cc.add_opt("-isystem=/usr/local/include")
cc.add_opt("-Isrc")
cc.add_opt("-fPIC")
cc.add_opt("-std=c99")
cc.add_opt("-Wall")
cc.add_opt("-Wextra")
cc.add_opt("-Werror")

var src = File.list("src")
	.where { |path| path.endsWith(".c") }

src
	.each { |path| cc.compile(path) }

// create static & dynamic libraries

var linker = Linker.new()

linker.archive(src.toList, "libumber.a")
linker.link(src.toList, [], "libumber.so", true)

// copy over headers

File.list("src")
	.where { |path| path.endsWith(".h") }
	.each  { |path| Resources.install(path) }

// installation map

var install = {
	"libumber.a":  "lib/libumber.a",
	"libumber.so": "lib/libumber.so",
	"umber.h":     "include/umber.h",
}

// testing

class Tests {
	static filter {
		return File.exec("test.sh")
	}

	static levels {
		return File.exec("test.sh")
	}
}

var tests = ["filter", "levels"]
