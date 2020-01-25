package main

import (
	"compress/gzip"
	"fmt"
	"io/ioutil"
	"log"
	"os"
)

func compress(fn string, buf []byte) {
	f, _ := os.Create(fn)
	w := gzip.NewWriter(f)
	w.Write(buf)
	w.Close()
}
func decompress(fn string) ([]byte, error) {
	f, err := os.Open(fn)
	if err != nil {
		log.Fatal(err)
	}
	defer f.Close()
	r, err := gzip.NewReader(f)
	if err != nil {
		log.Fatal(err)
	}
	defer r.Close()
	return ioutil.ReadAll(r)
}

func main() {
	m := "Hello, GZip!\n"
	fmt.Println(m)
	if len(os.Args) <= 1 {
		return
	}
	fn := os.Args[1]
	compress(fn, []byte(m))
	buf, err := decompress(fn)
	fmt.Println(buf, err)
}
