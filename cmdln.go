package main

import (
	"flag"
	"fmt"
	"os"
	"path"
)

func enum() {
	//use iota expression to replace enum.
	type weekDay int
	const (
		Sunday    weekDay = iota //0
		Monday                   //1
		Tuesday                  //2
		Wednesday                //3
		Thursday                 //4
		Friday                   //5
		Saturday                 //6
	)

	fmt.Printf("Sunday:%d, Monday:%d, Tuesday:%d, Wednesday:%d, Thursday:%d, Friday:%d, Saturday:%d\n",
		Sunday, Monday, Tuesday, Wednesday, Thursday, Friday, Saturday)

	const ( //const can be untyped at the definition and inferred when used.
		AddFlag = 1 << iota //bit 0.
		ModFlag             //bit 1
		_
		DelFlag //bit 3
	)
	fmt.Printf("AddFlag: 0x%x, ModFlag: 0x%x, DelFlag: 0x%x\n", AddFlag, ModFlag, DelFlag)

	const (
		Bit0, Mask0 = iota, 1 << iota //iota keeps the value within expression.
		Bit1, Mask1
		Bit2, Mask2
	)
	fmt.Printf("Bit0:%d,0x%x; Bit2:%d,0x%x\n", Bit0, Mask0, Bit2, Mask2)
}

func cmdln() {

	var strV string
	var intV int
	var boolV bool
	fmt.Println(os.Args)

	strP := flag.String("str", "default", "string option")
	intP := flag.Int("int", 0, "int option")
	boolP := flag.Bool("bool", false, "bool option")

	flag.StringVar(&strV, "strv", "default strv", "string variable")
	flag.IntVar(&intV, "intv", 0, "intv option")
	flag.BoolVar(&boolV, "boolv", false, "boolv option")
	flag.Parse()
	fmt.Println("str:  ", *strP)
	fmt.Println("int:  ", *intP)
	fmt.Println("bool: ", *boolP)
	fmt.Println("tail: ", flag.NArg(), flag.Args())
	for i:=0; i<flag.NArg(); i++{
		fmt.Println(flag.Arg(i))
	}

	fmt.Printf("\nstrv:  %s\nintv:  %d\nboolv: %t\n", strV, intV, boolV)

	fmt.Printf("Envs:\nGOPATH=%s\n", os.Getenv("GOPATH"))
	exe, _ := os.Executable()
	fmt.Printf("Executable: %s\n", exe)

}

func sum(n ...int) int { //vari basically is slice, in this case n is []int
	s := 0
	for _, v := range n {
		s += v
	}
	return s
}

func filePath() {
	fmt.Printf("Full:%s\nDir: %s\nBase: %12s\nExt:%s\n",
		os.Args[0], path.Dir(os.Args[0]), path.Base(os.Args[0]), path.Ext(os.Args[0]))
}

func textInColor() {
	//ESC sequence: 0x1B = ESC ESC[<c>m c specifies color. 30-37 foreground, 40-47 background, 0 to reset
	//foreground color: 30-37  Black, Red, Green, Yellow, Blue, Magenta, Cyan, White
	//background color: 40-47
	fmt.Println("NormalText\x1B[30m\x1B[47mTextInColor\x1B[0mNormalText")

	if fi, err := os.Stdout.Stat(); err == nil {
		fmt.Printf("%+#v\n\n", fi)
	}

}
func main() {
	textInColor()
	enum()
	cmdln()
	//	s := sum(1, 2, 3, 4, 5)
	//	fmt.Printf("%d, %T, %bb, 0x%X\n", s, s, s, s)
	//	filePath()
}
