package main

import (
	"fmt"
	"sort"
	"unsafe"
)

func removeDup(A []int) []int { //new array, with map
	//array by value, slices are references
	T := make([]int, 0)
	M := make(map[int]int, 0)
	for _, a := range A {
		if b, present := M[a]; present {
			T[b] = a
		} else {
			M[a] = len(T)
			T = append(T, a)
		}
	}
	A = T //this makes no effect. as A pass by value.
	return T
}

func removeDup2(B []int) []int { //inplace with map
	M := make(map[int]bool) //o(1) for map. space for speed.
	for i := 0; i < len(B); i++ {
		if M[B[i]] {
			//swap A[i] with A[len(A)-1] and drop last element.
			B[i] = B[len(B)-1]
			B = B[:len(B)-1]
			//i may still be a dup! retest it by backtrack by 1.
			i--
		} else {
			M[B[i]] = true
		}
	}
	return B
}
func removeDup3(B *[]int) []int { //modify the slice input, looks ugly, not the go way. return is replacement for ref in go
	M := make(map[int]bool)
	for i := 0; i < len(*B); i++ {
		if M[(*B)[i]] {
			//swap A[i] with A[len(A)-1] and drop last element.
			(*B)[i] = (*B)[len(*B)-1]
			*B = (*B)[:len(*B)-1]
			//i may still be a dup! retest it by backtrack by 1.
			i--
		} else {
			M[(*B)[i]] = true
		}
	}
	return *B
}
func removeDup4(B []int) []int { //inplace, without map, sort first then replace.
	sort.Ints(B) //sort first. use 2 pointers i,j, nlgn for sort. when n is small, it's reasonable compromise of space and speed.
	j := 0
	for i := 1; i < len(B); i++ {
		if B[i] != B[j] { //unique
			j++
			B[j] = B[i]
		}
	}
	return B[:j+1]
}

func main() {
	A := []int{1, 2, 3, 1, 2, 3}
	B := A[:] //slice is like pointer.
	T := removeDup(B)
	fmt.Printf("%v,%v,%v,%v,%v,%v\n", A, B, T, unsafe.Pointer(&A), unsafe.Pointer(&B), unsafe.Pointer(&T))
	A = []int{1, 2, 3, 1, 2, 3}
	B = A[:] //slice is like pointer.
	T = removeDup2(B)
	fmt.Printf("%v,%v,%v,%v,%v,%v\n", A, B, T, unsafe.Pointer(&A), unsafe.Pointer(&B), unsafe.Pointer(&T))
	A = []int{1, 2, 3, 1, 2, 3}
	B = A[:] //slice is like pointer.
	T = removeDup3(&B)
	fmt.Printf("%v,%v,%v,%v,%v,%v\n", A, B, T, unsafe.Pointer(&A), unsafe.Pointer(&B), unsafe.Pointer(&T))
	A = []int{1, 2, 3, 1, 2, 3}
	B = A[:] //slice is like pointer.
	T = removeDup4(B)
	fmt.Printf("%v,%v,%v,%v,%v,%v\n", A, B, T, unsafe.Pointer(&A), unsafe.Pointer(&B), unsafe.Pointer(&T))

	Y := [2]int{}
	X := Y[0:0:1] //when cap is specified, slice grow out of array when cap of slice reached, not cap of array reached.
	//	X := make([]int, 1,1)
	fmt.Println(X, len(X), cap(X))
	for i := 0; i < 30; i++ {
		X = append(X, i+1)
		X[0] = i + 1 //when X grow out of Y, they part ways. Y[x] doesn't point to X[0] anymore
		fmt.Printf("Y: %v, %v, X: %v, %v, %v, %v\n", Y, unsafe.Pointer(&Y), X, unsafe.Pointer(&X), len(X), cap(X))
	}
	var Z []int
	for len(X) != 0 { //drop first, middle and last. keep the order, lot of element copy. from cap, X is new everytime.
		Z = append(Z, X[0], X[len(X)/2], X[len(X)-1])
		sort.Ints(Z)
		X = append(X[1:len(X)/2], X[len(X)/2+1:len(X)-1]...)
		fmt.Println(Z, len(Z), cap(Z))
		fmt.Println(X, len(X), cap(X))
	}
	fmt.Println("==========================================================")
	for len(Z) != 0 { //insert the last one in the middle
		//X = append(X[:len(X)/2], append([]int{Z[len(Z)-1]}, X[len(X)/2:]...)...)//not efficient.
		X = append(X, 0)
		copy(X[(len(X)-1)/2+1:], X[(len(X)-1)/2:])
		X[(len(X)-1)/2] = Z[len(Z)-1]
		Z = Z[:len(Z)-1]
		fmt.Println("Z:", Z, len(Z), cap(Z))
		fmt.Println("X:", X, len(X), cap(X))
	}

}
