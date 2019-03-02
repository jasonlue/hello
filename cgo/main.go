package main

import "fmt"

type slice struct{
	addr uintptr
	len int
	cap int
}

func TestSlice(){
	
	sl := slice{}
}
func main() {
	fmt.Println("Hello, CGO!")
}
