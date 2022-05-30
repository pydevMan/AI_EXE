package basicconvmake

/*
*|section one encoding for matrix size|
*|layout of data in (proj_name).ai.out.bytes|
|     matrix  	|    |  matrix |  |      |
|     encode  	| -> |	data   |->| small|->
|     space	  	| -> |	space! |->|	ident|->all compressed down for later reading
|size = 2*layers|	 |//encodes|  |      |
*/

import (
	"encoding/json"
	"io/ioutil"
)

// the struct that encodes for json
type ActionBuildFile struct {
	For        string    `json:"for"`
	Action     string    `json:"action"`
	Inputs     uint64    `json:"inputs"`
	Outputs    uint64    `json:"outputs"`
	Layers     uint64    `json:"layers"`
	Sup_Matrix [2]uint64 `json:"max-matrix"`
}

//make the basic convolutinal network, it is matrix based simple and elagant in design, truly a sight to be hold how linear algabra can combine matraces in such a way
func MakeBasicConv(actionFiles []string, name, folder string) {
	containsNeededFile := false

	//wish go was more declarative
	for _, file := range actionFiles {
		if file == "action.create.layout.json" {
			containsNeededFile = true
		}
	}

	if !containsNeededFile {
		panic("Needed a action.create.layout.json file did not have it")
	}

	createStruct := ActionBuildFile{}

	bytes, err := ioutil.ReadFile(folder + "/" + "action.create.layout.json")

	if err != nil {
		panic(err)
	}

	// we can finaly parse the json it only took like 12 lines thanks golang
	err = json.Unmarshal(bytes, &createStruct)

	if err != nil {
		panic(err)
	}

	// now that we have the json file we can focus on the layout of this kind of ai

	newfilename := name + "ai.out.binfile.mat_code"

	makeConvBinFile(newfilename, createStruct.Sup_Matrix, createStruct.Layers, createStruct.Inputs, createStruct.Outputs)

}

//after reachin a convolutinal ai make command this runs:
func makeConvBinFile(outName string, max_matrix [2]uint64, layers, outputs, inputs uint64) {
	//size := (2 + layers) * 8
	encodeData(max_matrix[0], max_matrix[1], layers, outputs, inputs, outName)
}

func toBytes(num uint64) []byte {
	var out []byte
	for i := 0; i < 8; i++ {
		out = append(out, byte(0xff&(num>>(i*8))))
	}

	return out
}

func encodeData(maxHeight, maxWidth, layers, outputs, inputs uint64, outName string) {
	//size gotten by (m x n) * (c x k) = (m x k)

	//gives the optin to ranspose the input vector as the first input
	//gives the optin to ranspose the output vector as the last output
	transposeRes := make([][2]byte, 0)
	prevSizeVec := [2]uint64{inputs, 1}
	for i := 0; i < int(layers); i++ {

	}

}
