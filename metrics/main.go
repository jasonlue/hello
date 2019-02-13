package main

import (
	"fmt"
	"time"

	humanize "github.com/dustin/go-humanize"
	"github.com/shirou/gopsutil/cpu"
	"github.com/shirou/gopsutil/disk"
	"github.com/shirou/gopsutil/mem"
)

func main() {

	//	numPhyProcessors, _ := cpu.Counts(true)
	//	numLogProcessors, _ := cpu.Counts(true)
	//fmt.Printf("physical CPUs: %v\n", numPhyProcessors)
	//fmt.Printf("logical CPUs: %v\n", numLogProcessors)

	cores, _ := cpu.Info()
	for _, c := range cores {
		fmt.Println(c)
	}
	cpuStat, _ := cpu.Times(false)
	fmt.Println(cpuStat)
	cpuStats, _ := cpu.Times(true)
	for _, cs := range cpuStats {
		fmt.Println(cs)
	}

	percent, _ := cpu.Percent(time.Second, false)
	fmt.Println(percent)
	for _, per := range percent {
		fmt.Println(per)
	}

	percents, _ := cpu.Percent(time.Second, true)
	for _, per := range percents {
		fmt.Println(per)
	}

	v, _ := mem.VirtualMemory()
	fmt.Printf("Total: %v, Free:%v, UsedPercent:%f%%\n", humanize.Comma(int64(v.Total)), humanize.Comma(int64(v.Free)), v.UsedPercent)
	fmt.Println(v)

	swap, _ := mem.SwapMemory()
	fmt.Println(swap)

	partitions, _ := disk.Partitions(false)
	for _, part := range partitions {
		fmt.Println(part)

	}

	u, _ := disk.Usage("/")
	fmt.Println(u)

}
