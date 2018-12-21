package main

import (
	"fmt"
	"math/rand"
	"os"
	"os/signal"
	"sync"
	"time"
)

//ideas to explore
//Key:
//	1. channel is the core mechanism. others have to convert to channel to combine together with either WaitForAll() or WaitForAny()
//  2. select is to wait for any channels to signal
//  3. channel should always be closed by sender. channel will exist until the data are all retrieved. then receiver returns immediately on reading channel
//  4. reading nil channel will block forever

//problems to solve
//1. wait for all channels to signal
//		for c1 != nil || c2 != nil{
//			select{
//				case c1: c1 = nil
//				case c2: c2 = nil
//		}
//}
//2. wait for any channel to signal
//		select{
//			case m1 <- c1:
//			case m2 <- c2:
//}
//3. one signal to triger all waiting routines.
//		close(c)
//4. wait for a mix of channel and waiting group and other sync mechanisms.
//	use a routine to wait on the wg and close a channel.
//	this channel are in waiting select outside.

func tchan() {
	for { //time.After creates a time tick channel and beeps periodically into this
		fmt.Println(<-time.After(time.Second))
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
func oneWriterOneReader() {
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
			time.Sleep(time.Second * time.Duration(rand.Intn(5)))
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

func oneWriterNReader() {
	jobs := make(chan int, 2)
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
		time.Sleep(time.Second * time.Duration(rand.Intn(5)))
		fmt.Printf("dispatching job #%d\n", i)
		jobs <- i
	}
	close(jobs)
	fmt.Println("dispatched all the jobs. waiting for them to be processed.")
	<-done
	<-done
	<-done
	fmt.Println("all jobs processed.")
	close(done)
}

func oneWriterNReaderRange() {
	jobs := make(chan int, 2)
	done := make(chan bool)
	for i := 0; i < 3; i++ {
		go func(id int) { //reader
			for j := range jobs { //range ends with channel is closed, not when channel is empty. block when channel is empty instead.
				fmt.Printf("routine #%d: processing job #%d\n", id, j)
				time.Sleep(time.Second * time.Duration(rand.Intn(5)))
				fmt.Printf("routine #%d: Finished job #%d\n", id, j)
			}
			fmt.Printf("routine #%d: done all jobs.\n", id)
			done <- true //writer closes done channel after first reader detects and writes to done channel.
			return
		}(i)
	}
	for i := 0; i < 30; i++ {
		time.Sleep(time.Second * time.Duration(rand.Intn(5)))
		fmt.Printf("dispatching job #%d\n", i)
		jobs <- i
	}
	close(jobs) //close channel can still keep data for readerd to read.
	fmt.Println("dispatched all the jobs. waiting for them to be processed.")
	<-done
	<-done
	<-done
	close(done) //can still write to closed channel? it should panic but doesn't
	fmt.Println("all jobs processed.")
}
func oneWriterNReaderRangeWG() {
	jobs := make(chan int, 2)
	var done sync.WaitGroup
	done.Add(3)
	for i := 0; i < 3; i++ {
		go func(id int) { //reader
			for j := range jobs { //range ends with channel is closed, not when channel is empty. block when channel is empty instead.
				fmt.Printf("routine #%d: processing job #%d\n", id, j)
				time.Sleep(time.Second * time.Duration(rand.Intn(5)))
				fmt.Printf("routine #%d: Finished job #%d\n", id, j)
			}
			fmt.Printf("routine #%d: done all jobs.\n", id)
			done.Done()
			return
		}(i)
	}
	for i := 0; i < 30; i++ {
		time.Sleep(time.Second * time.Duration(rand.Intn(5)))
		fmt.Printf("dispatching job #%d\n", i)
		jobs <- i
	}
	close(jobs) //close channel can still keep data for readerd to read.
	fmt.Println("dispatched all the jobs. waiting for them to be processed.")
	done.Wait()
	fmt.Println("all jobs processed.")
}

func nWriterNReader() {
	jobs := make(chan int, 3)
	nWriters := 2
	var writing sync.WaitGroup
	writing.Add(nWriters)
	nReaders := 3
	var reading sync.WaitGroup
	reading.Add(nReaders)
	for i := 0; i < nReaders; i++ {
		go func(id int) { //reader
			for j := range jobs {
				switch id {
				case 0:
					fmt.Printf("\x1B[31m")
				case 1:
					fmt.Printf("\x1B[32m")
				case 2:
					fmt.Printf("\x1B[33m")
				}
				fmt.Printf("\tRd#%d: received job #%d\x1B[0m\n", id, j)
				time.Sleep(time.Second * time.Duration(rand.Intn(3)))
			}
			fmt.Printf("\tRd#%d: no more jobs!\n", id)
			reading.Done()
		}(i)
	}
	for i := 0; i < nWriters; i++ {
		go func(id int) { //writers.
			for i := 0; i < 30; i++ {
				time.Sleep(time.Second * time.Duration(rand.Intn(3)))
				if i%nWriters == id {
					switch id {
					case 0:
						fmt.Printf("\x1B[35m")
					case 1:
						fmt.Printf("\x1B[36m")
					}
					fmt.Printf("Wr#%d: generated job#%d\x1B[0m\n", id, i)
					jobs <- i
				}
			}
			fmt.Printf("Wr#%d: done generating all jobs.\n", id)
			writing.Done()
		}(i)
	}
	writing.Wait()
	close(jobs) //signal readers to end gracefully. this property of channel is like broadcast events of condition variable.
	fmt.Printf("main: all writers done\n")
	reading.Wait()
	fmt.Printf("main: all readers done\n")
}
func nWriterNReaderQuit() {
	jobs := make(chan int, 3)
	quitWriting := make(chan bool)
	nWriters := 2
	writing := sync.WaitGroup{}
	writing.Add(nWriters)
	nReaders := 3
	reading := sync.WaitGroup{}
	reading.Add(nReaders)
	for i := 0; i < nReaders; i++ {
		go func(id int) { //reader
			for j := range jobs {
				switch id {
				case 0:
					fmt.Printf("\x1B[31m")
				case 1:
					fmt.Printf("\x1B[32m")
				case 2:
					fmt.Printf("\x1B[33m")
				}
				fmt.Printf("\tRd#%d: received job #%d\x1B[0m\n", id, j)
				time.Sleep(time.Second * time.Duration(rand.Intn(3)))
			}
			fmt.Printf("\tRd#%d: no more jobs!\n", id)
			reading.Done()
		}(i)
	}
	for i := 0; i < nWriters; i++ {
		go func(id int) { //writers.
		outerLoop:
			for i := 0; ; i++ {
				time.Sleep(time.Second * time.Duration(rand.Intn(3))) //this can convert to timeout channel too.
				if i%nWriters == id {
					switch id {
					case 0:
						fmt.Printf("\x1B[35m")
					case 1:
						fmt.Printf("\x1B[36m")
					}
					fmt.Printf("Wr#%d: generated job#%d\x1B[0m\n", id, i)

					select {
					case jobs <- i:
						//continue for loop
					case <-quitWriting:
						break outerLoop //out of for loop.
					}
				}
			}
			fmt.Printf("Wr#%d: done generating all jobs.\n", id)
			writing.Done()
		}(i)
	}
	fmt.Println("sleeping for a minute.")
	time.Sleep(time.Minute)
	fmt.Println("tell writers to quit")
	close(quitWriting)
	writing.Wait()
	close(jobs) //signal readers
	fmt.Printf("main: all writers done\n")
	reading.Wait()
	fmt.Printf("main: all readers done\n")
}

func main() {
	fmt.Println("Hello, Go Routines!")
	//oneReaderOneWriter()
	nWriterNReaderQuit()
	//simpleGo()
	//go tchan()
	//fibchan()
	//var s string
	//fmt.Scanln(&s)

	//graceful exit for the program.
	c := make(chan os.Signal)
	signal.Notify(c, os.Interrupt)

}
