package main

import (
	"flag"
	"fmt"
	"net"
	"os"

	"github.com/google/gopacket/pcapgo"

	"github.com/google/gopacket"

	"github.com/google/gopacket/layers"
	"github.com/google/gopacket/pcap"
)

func lessThan(ip1, ip2 net.IP, port1, port2 uint16) bool {
	for i := 0; i < 4; i++ {
		if ip1[i] < ip2[i] {
			return true
		} else if ip1[i] > ip2[i] {
			return false
		}
	}
	return port1 < port2
}
func key(pkt gopacket.Packet) string {
	ipv4, ok := pkt.NetworkLayer().(*layers.IPv4)
	if !ok {
		return "etc"
	}
	proto := ""
	var srcPort, dstPort uint16
	switch t := pkt.TransportLayer().(type) {
	case *layers.TCP:
		proto = "tcp"
		srcPort = uint16(t.SrcPort)
		dstPort = uint16(t.DstPort)
	case *layers.UDP:
		proto = "udp"
		srcPort = uint16(t.SrcPort)
		dstPort = uint16(t.DstPort)
	}
	//since reverse src dst still same session. we put smaller ones first.
	if lessThan(ipv4.SrcIP, ipv4.DstIP, srcPort, dstPort) {
		return fmt.Sprintf("%s:%d-%s-%s:%d", ipv4.SrcIP, srcPort, proto, ipv4.DstIP, dstPort)
	}
	return fmt.Sprintf("%s:%d-%s-%s:%d", ipv4.DstIP, dstPort, proto, ipv4.SrcIP, srcPort)
}
func doSplitFile(file string) error {
	hpcap, err := pcap.OpenOffline(file)
	if err != nil {
		return fmt.Errorf("failed to open %s: %s", file, err)
	}
	defer hpcap.Close()
	m := map[string]*pcapgo.Writer{}

	packetSource := gopacket.NewPacketSource(hpcap, hpcap.LinkType())
	for packet := range packetSource.Packets() {
		k := key(packet)
		_, present := m[k]
		if !present {
			i := len(file) - 1
			for ; i >= 0 && file[i] != '.'; i-- {
			}

			output := file[:i] + "-" + k + file[i:]
			f, _ := os.Create(output) //assert success?
			w := pcapgo.NewWriter(f)
			w.WriteFileHeader(1024 /*snapshotLen*/, layers.LinkTypeEthernet)
			defer f.Close()
			m[k] = w
		}
		m[k].WritePacket(packet.Metadata().CaptureInfo, packet.Data())
	}
	return nil
}
func doSplit(files []string) {
	for _, file := range files {
		if err := doSplitFile(file); err != nil {
			fmt.Println(err) //print out error. but continue next file.
		}
	}
}

func main() {
	help := flag.Bool("h", false, "help")
	split := flag.Bool("s", false, "Split pcap file into series of files of the same session")
	flag.Parse()
	switch {
	case *split == true:
		doSplit(flag.Args())
	case *help == true:
		flag.Usage()
	default:
		flag.Usage()
	}
}
