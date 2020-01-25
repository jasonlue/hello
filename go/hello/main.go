package main

import (
	"fmt"
	"net"

	"github.com/google/gopacket/pcap"
	"github.com/vadv/ostent/getifaddrs"
)

func main() {

	fmt.Println(struct{ i int }{1})
	fmt.Println("ifconfig!")
	pcapDevices()
	netInterfaces()
	ifAddrs()
}

func pcapDevices() {
	fmt.Printf("pcap devices:\n")
	devices, err := pcap.FindAllDevs()
	if err != nil {
		return
	}
	for i, device := range devices {
		fmt.Printf("#%d: name=%s, desciption=%s, addresses:\n", i, device.Name, device.Description)
		for j, addr := range device.Addresses {
			fmt.Printf("\t%d: %v, %v\n", j, addr.IP, addr.Netmask)
		}
	}
}

func netInterfaces() {
	fmt.Printf("go net.Interfaces:\n")
	ifaces, err := net.Interfaces()
	if err != nil {
		return
	}
	for _, iface := range ifaces {
		fmt.Printf("%v\n", iface)
	}
}

func ifAddrs() { //stats on interface.
	fmt.Printf("getifaddrs:\n")
	ifdatas, err := getifaddrs.Getifaddrs()
	if err != nil {
		return
	}
	for _, ifd := range ifdatas {
		fmt.Printf("%v\n", ifd)
	}

}
