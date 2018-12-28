package main

import (
	"bytes"
	"flag"
	"fmt"
	"io/ioutil"
	"net"
	"os"
	"path/filepath"
	"sync"

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

func doSplitFile(file string, wg *sync.WaitGroup) {
	defer wg.Done()
	splitFile(file)
}

func splitFile2(file string) {
	fmt.Printf("Splitting %s\n", file)
	fi, err := os.Stat(file)
	if err != nil {
		fmt.Fprintf(os.Stderr, "%s: %s", file, err)
		return
	}
	if !fi.Mode().IsRegular() {
		fmt.Fprintf(os.Stderr, "%s: is not a regular file.\n", file)
		return
	}
	hpcap, err := pcap.OpenOffline(file)
	if err != nil {
		fmt.Fprintln(os.Stderr, err)
		return
	}
	defer hpcap.Close()
	m := map[string]*pcapgo.Writer{}

	packetSource := gopacket.NewPacketSource(hpcap, hpcap.LinkType())
	base := filepath.Base(file)
	i := len(base) - 1
	for ; i >= 0 && base[i] != '.'; i-- {
	}
	for packet := range packetSource.Packets() {
		k := key(packet)
		_, present := m[k]
		if !present {
			output := base[:i] + "-" + k + base[i:]
			f, err := os.Create(output)
			if err != nil {
				fmt.Fprintln(os.Stderr, err)
				return
			}
			defer f.Close()
			w := pcapgo.NewWriter(f)
			w.WriteFileHeader(1024 /*snapshotLen*/, layers.LinkTypeEthernet)
			m[k] = w
		}
		m[k].WritePacket(packet.Metadata().CaptureInfo, packet.Data())
	}
}

// splitFile split pcap file into a set of pcap files based on src/dsk ip and port tuple.
//to avoid too many open files error (1024 limit, can be set by ulimit -Sn 4096)
//write to the memory first and write the files at once.
func splitFile(file string) {
	fi, err := os.Stat(file)
	if err != nil {
		fmt.Fprintf(os.Stderr, "%s: %s", file, err)
		return
	}
	if !fi.Mode().IsRegular() {
		//		fmt.Fprintf(os.Stderr, "%s: is not a regular file.\n", file)
		return
	}
	fmt.Printf("Splitting %s\n", file)
	hpcap, err := pcap.OpenOffline(file)
	if err != nil {
		fmt.Fprintln(os.Stderr, err)
		return
	}
	defer hpcap.Close()
	type valType struct {
		w *pcapgo.Writer
		b *bytes.Buffer
	}
	m := map[string]valType{}

	packetSource := gopacket.NewPacketSource(hpcap, hpcap.LinkType())
	for packet := range packetSource.Packets() {
		k := key(packet)
		_, present := m[k]
		if !present {
			b := bytes.NewBuffer([]byte{})
			w := pcapgo.NewWriter(b)
			w.WriteFileHeader(1024 /*snapshotLen*/, layers.LinkTypeEthernet)
			m[k] = valType{w, b}
		}
		m[k].w.WritePacket(packet.Metadata().CaptureInfo, packet.Data())
	}
	base := filepath.Base(file)
	i := len(base) - 1
	for ; i >= 0 && base[i] != '.'; i-- {
	}
	wg := sync.WaitGroup{}
	for k, v := range m {
		output := base[:i] + "-" + k + base[i:]
		wg.Add(1)
		go func(o string, b []byte) {
			err := ioutil.WriteFile(o, b, 0644)
			if err != nil {
				fmt.Fprintln(os.Stderr, err)
			}
			wg.Done()
		}(output, v.b.Bytes())
	}
	wg.Wait()
}

//splitFile2 not use pcap lib. straight gopacket.
func splitFile3(file string) error {
	return nil
}
func doSplit(files []string) {
	wg := sync.WaitGroup{}
	for _, file := range files {
		wg.Add(1)
		go doSplitFile(file, &wg)
	}
	wg.Wait()
	fmt.Printf("Done.\n")
}

func split(files []string) {
	for _, file := range files {
		splitFile(file)
	}
	fmt.Printf("Done.\n")
}
func main() {
	help := flag.Bool("h", false, "help")
	splt := flag.Bool("s", false, "Split pcap file into series of files of the same session")
	flag.Parse()
	flag.Usage = func() {
		fmt.Fprintf(os.Stderr, "Usage: %s [option...] [file...]\n\nOptions:\n", os.Args[0])
		flag.PrintDefaults()
	}
	switch {
	case *splt == true:
		doSplit(flag.Args())
	case *help == true:
		flag.Usage()
	default:
		flag.Usage()
	}
}
