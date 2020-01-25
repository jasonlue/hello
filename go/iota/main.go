package main

import "fmt"

func main() {
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
