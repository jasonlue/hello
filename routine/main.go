package main

import (
	"fmt"
	"math/rand"
	"time"
)

func f(i int) {
	time.Sleep(time.Millisecond * time.Duration(rand.Intn(10)))
	fmt.Printf("%d ", i)
}

func simpleGo() {
	for i := 0; i < 100; i++ {
		go f(i)
	}
}
func tchan() {
	for { //time.After creates a time tick channel and beeps periodically into this
		fmt.Println(<-time.After(time.Second))
	}
}
func fibchan() {
	c := make(chan int, 2)
	c <- 0
	c <- 1
	for {
		i := <-c
		j := <-c
		fmt.Println(i+j, float64(j)/(float64(i+j)))
		c <- j
		c <- i + j
		time.Sleep(time.Second)
	}
}
func fchan(c <-chan int) {
	for {
		f(<-c)
	}
}
func chanGo() {
	c := make(chan int)
	go fchan(c)
	for i := 0; i < 100; i++ {
		c <- i
	}
}
func fchan2ears(c1 <-chan int, c2 <-chan int) {
	for {
		select {
		case i1 := <-c1:
			fmt.Printf("%dL ", i1)
		case i2 := <-c2:
			fmt.Printf("%dR ", i2)
		}
	}
}
func chan2earsGo() {
	c1 := make(chan int, 10)
	c2 := make(chan int, 10)
	go fchan2ears(c1, c2)
	for i := 0; i < 100; i++ {
		if i%2 == 0 {
			c2 <- i
		} else {
			c1 <- i
		}
	}
}

//Try a few things.
//channel can only be closed by one. can't be closed for one direction.
//one side close the channel, the other side can detect by
//x,more := <-jobs.  when jobs close, more is false.
//range over channel is like for loop.
//multiple routines read from the same channel?

//one worker, one feeder. unbuffered channel. in locksteps.
func oneReaderOneWriter() {
	jobs := make(chan int)
	done := make(chan bool)
	go func() { //reader
		for {
			j, more := <-jobs
			if !more {
				fmt.Print("done all jobs.\n")
				done <- true
				return
			}
			fmt.Printf("processing job #%d\n", j)
			time.Sleep(time.Second * time.Duration(rand.Intn(10)))
			fmt.Printf("Finished job #%d\n", j)
		}
	}()
	for i := 0; i < 10; i++ {
		fmt.Printf("dispatching job #%d\n", i)
		jobs <- i
	}
	close(jobs)
	fmt.Println("dispatched all the jobs. waiting for them to be processed.")
	<-done
	fmt.Println("all jobs processed.")
	close(done)
}

func multiReaderOneWriter() {
	jobs := make(chan int, 6)
	done := make(chan bool)
	for i := 0; i < 3; i++ {
		go func(id int) { //reader
			for {
				j, more := <-jobs
				if !more {
					fmt.Printf("routine #%d: done all jobs.\n", id)
					done <- true
					return
				}
				fmt.Printf("routine #%d: processing job #%d\n", id, j)
				time.Sleep(time.Second * time.Duration(rand.Intn(10)))
				fmt.Printf("routine #%d: Finished job #%d\n", id, j)
			}
		}(i)
	}
	for i := 0; i < 30; i++ {
		fmt.Printf("dispatching job #%d\n", i)
		jobs <- i
	}
	close(jobs)
	fmt.Println("dispatched all the jobs. waiting for them to be processed.")
	<-done
	fmt.Println("all jobs processed.")
	close(done)
}
func main() {
	fmt.Println("Hello, Go Routines!")
	//oneReaderOneWriter()
	multiReaderOneWriter()
	//simpleGo()
	//go tchan()
	//fibchan()
	//var s string
	//fmt.Scanln(&s)
}
