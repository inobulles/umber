// C compilation

var cc = CC.new()

cc.add_opt("-I/usr/local/include")
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

var linker = Linker.new(cc)

linker.archive(src.toList, "libumber.a")
linker.link(src.toList, [], "libumber.so", true)

// copy over headers

File.list("src")
	.where { |path| path.endsWith(".h") }
	.each  { |path| Resources.install(path) }

// installation map

var prefix = "/usr/local" // TODO way to discriminate between OS' - on Linux distros, this would usually be simply "/usr" instead

var install = {
	"libumber.a":  "%(prefix)/lib/libumber.a",
	"libumber.so": "%(prefix)/lib/libumber.so",
	"umber.h":     "%(prefix)/include/umber.h",
}

// TODO testing

class Tests {
	static levels {
		return File.exec("test.sh")
	}
}

var tests = ["levels"]
