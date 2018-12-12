package main

import (
	"encoding/json"
	"flag"
	"fmt"
	"io/ioutil"
	"os"
)

//json <test.json>
type indicators struct {
	UUID       string
	Indicators []struct {
		UUID       string
		Pattern    string
		Confidence int
		OutLabels  []struct {
			Name string
		}
	}
}

type fileTypes struct {
	UUID      string
	FileTypes []struct {
		PrimaryMIME         string
		AlternateMIMEs      []string
		PrimaryExtension    string
		AlternateExtensions []string
		Comment             string
	}
}

func main() {
	indicatorsFilePtr := flag.String("i", "", "indicators file")
	fileTypesFilePtr := flag.String("t", "", "file type file")
	flag.Parse()
	if flag.NArg() > 0 || len(*indicatorsFilePtr) == 0 || len(*fileTypesFilePtr) == 0 {
		fmt.Printf("%s usage:\n", os.Args[0])
		flag.PrintDefaults()
		return
	}

	bytes, err := ioutil.ReadFile(*indicatorsFilePtr)
	if err != nil {
		fmt.Printf("Failed to read file %s: %s\n", *indicatorsFilePtr, err)
		return
	}
	var inds indicators
	err = json.Unmarshal(bytes, &inds)
	if err != nil {
		fmt.Printf("Failed to parse json file %s: %s\n", *indicatorsFilePtr, err)
		return
	}

	fmt.Printf("Indicators file UUID: %s\n", inds.UUID)
	for _, v := range inds.Indicators {
		fmt.Printf("UUID: %s, Pattern: %s, Confidence: %d, OutLabels: %+v\n",
			v.UUID, v.Pattern, v.Confidence, v.OutLabels)
	}

	bytes, err = ioutil.ReadFile(*fileTypesFilePtr)
	if err != nil {
		fmt.Printf("Failed to read file %s: %s\n", *fileTypesFilePtr, err)
	}
	var ft fileTypes
	err = json.Unmarshal(bytes, &ft)
	if err != nil {
		fmt.Printf("failed to parse json file %s: %s\n", *fileTypesFilePtr, err)
	}
	fmt.Printf("FileTypes file UUID: %s\n", ft.UUID)
	for _, v := range ft.FileTypes{
		fmt.Printf("\n%s", v.PrimaryMIME)
		if len(v.AlternateMIMEs) != 0{
			fmt.Printf("%v", v.AlternateMIMEs)
		}
		fmt.Printf("\n\t%s", v.PrimaryExtension)
		if len(v.AlternateExtensions) != 0 {
			fmt.Printf("%v", v.AlternateExtensions)
		}
	}
}
