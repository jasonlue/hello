package main

/*
#include "_obj/_cgo_export.h"
void doSomething(int *p) {
  *p = 123;
}

//extern void goHello();
void callGo(){
	goHello();
}
*/
import "C"

import "fmt"

func testCgo() {
	var n C.int
	C.doSomething(&n)
	fmt.Printf("n=%v\n", n)
	C.callGo()
}
func main() {
	fmt.Println("Hello, CGO!")
	testCgo()
	fmt.Println("Bye, CGO!")
}
