package command

import (
	"ai_exe/command/basicconvmake"
	"ai_exe/command/neatmake"
	"encoding/json"
	"io/ioutil"
)

type initFileJson struct {
	ProjectName        string   `json:"ProjectName"`
	ProjectKind        string   `json:"ProjectKind"`
	ProjectActionFiles []string `json:"ProjectActionFiles"`
	ProjectFolder      string   `json:"ProjectFolder"`
}

func handleJsonNew(filename string) {
	//read the file
	file, err := ioutil.ReadFile(filename)
	if err != nil {
		panic(err)
	}

	initStruct := initFileJson{}

	err = json.Unmarshal(file, &initStruct)

	if err != nil {
		panic(err)
	}

	switch initStruct.ProjectKind {
	case "Basic-Conv":
		basicconvmake.MakeBasicConv(initStruct.ProjectActionFiles, initStruct.ProjectName, initStruct.ProjectFolder)
	case "NEAT_EXE":
		neatmake.MakeNeat(initStruct.ProjectActionFiles, initStruct.ProjectName, initStruct.ProjectFolder)
	default:
		return
	}
}

func HandleNew() {
	if FileNameA != nil || FileNameB != nil {
		handleJsonNew(*FileNameA)
		return
	}
}
