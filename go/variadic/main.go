package main

import "fmt"

func print(s ...string) { //pass string list, treat internal as string slice []string
	fmt.Printf("%#v\n", s)
}

func main() {
	fmt.Printf("Hello, Variadic!\n")
	print("1", "2", "3")
	s := []string{"11", "22", "33"}
	print(s...)//parameter is string variable list. pass string slice. use ... to flatten the string slice.
}
