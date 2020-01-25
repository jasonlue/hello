package main

import "fmt"

func fibGen() func() int {
	f1, f2 := 0, 1
	return func() int {
		f1, f2 = f2, f1+f2
		return f2
	}
}

func main() {
	fib := fibGen()
	for i := 1; i < 30; i++ {
		fmt.Println(fib())
	}
}
