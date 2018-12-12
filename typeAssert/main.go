package main

import (
	"fmt"
)

//IFoo detect if a type has interface's functions.
type IFoo interface {
	Foo()
}

//IBar is interface
type IBar interface {
	Bar()
}

type foo struct {
	IFoo
}

func (foo) Foo() {
	fmt.Println("foo::Foo()!")
}

type fooBar int

func (*fooBar) Foo() {
	fmt.Println("fooBar::Foo()")
}
func (*fooBar) Bar() {
	fmt.Println("footBar::Bar()")
}

func main() {

	f := foo{}
	var ifoo interface{} = f
	v, ok := ifoo.(IFoo)
	if ok {
		v.Foo()
	}
	fb := fooBar(10)
	var ifb interface{} = &fb
	v2, ok2 := ifb.(IFoo)
	if ok2 {
		v2.Foo()
	}
	v3, ok3 := ifb.(IBar)
	if ok3 {
		v3.Bar()
	}
}
