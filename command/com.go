package command

import (
	"flag"
)

var (
	New        = flag.Bool("new", false, "create a new ai project")
	Run        = flag.Bool("run", false, "run ai project")
	FileNameA  = flag.String("nameA", "", "enter a file name, kinda like a argument of a function")
	FileNameB  = flag.String("nameB", "", "enter a file name, kinda like a argument of a function")
	CheckBatch = flag.Bool("checkBatch", false, "check batch of a evolutinary ai")
	Learn      = flag.Bool("convlearn", false, "theach a convolutional neural network")
	//speeds up the function ParseCommands
	FastFuncArray = [4]func(){HandleNew, HandleRun, HandleCheckBatch, HandleLearn}
	FuncStack     []func()
)

//run all my commands
func RunCommands() {
	boolOptions := [4]bool{*New, *Run, *CheckBatch, *Learn}
	for i, option := range boolOptions {
		if option {
			FuncStack = append(FuncStack, FastFuncArray[i])
		}
	}
}
