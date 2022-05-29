package main

import (
	"ai_exe/command"
	"flag"
)

func main() {
	flag.Parse()
	command.RunCommands()

	//run the commandline stuff
	for _, function := range command.FuncStack {
		function()
	}
}
