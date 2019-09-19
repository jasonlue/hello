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

	"github.com/google/gopacket"
	"github.com/google/gopacket/layers"
	"github.com/google/gopacket/pcap"
	"github.com/google/gopacket/pcapgo"
)

func main() {
	splt := flag.Bool("s", false, "Split pcap file into series of files of the same session")
	filt := flag.Bool("f", false, "filter out tcp/udp from pcap file")
	flag.Usage = func() {
		fmt.Fprintf(os.Stderr, "Usage: %s [option...] pcap_file\n\nOptions:\n", os.Args[0])
		flag.PrintDefaults()
	}
	flag.Parse()
	if !*splt && !*filt || len(flag.Args()) != 1 {
		flag.Usage()
	} else {
		splitFile(flag.Args()[0], *filt)
	}
}

func splitFile(file string, filtOnly bool) {
	if fi, err := os.Stat(file); err != nil {
		fmt.Fprintf(os.Stderr, "%s: %s\n", file, err)
	} else if !fi.Mode().IsRegular() {
		fmt.Fprintf(os.Stderr, "%s is not a regular file\n", file)
	} else {
		if filtOnly {
			fmt.Printf("Filtering %s\n", file)
		} else {
			fmt.Printf("Splitting %s\n", file)
		}
		splitPcapFile(file, filtOnly)
	}
}

func splitPcapFile(file string, filtOnly bool) {
	if hpcap, err := pcap.OpenOffline(file); err != nil {
		fmt.Fprintln(os.Stderr, err)
	} else {
		defer hpcap.Close()
		type valType struct {
			w *pcapgo.Writer
			b *bytes.Buffer
		}
		m := map[string]valType{}

		//distribute each packet to each bucket.
		packetSource := gopacket.NewPacketSource(hpcap, hpcap.LinkType())
		for packet := range packetSource.Packets() {
			k := key(packet, filtOnly)
			if _, present := m[k]; !present {
				b := bytes.NewBuffer([]byte{})
				w := pcapgo.NewWriter(b)
				w.WriteFileHeader(1024 /*snapshotLen*/, layers.LinkTypeEthernet)
				m[k] = valType{w, b}
			}
			m[k].w.WritePacket(packet.Metadata().CaptureInfo, packet.Data())
		}
		//write bucket to each file. use go routine as io is slow.
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
}

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
func key(pkt gopacket.Packet, filtOnly bool) string {
	ipv4, ok := pkt.NetworkLayer().(*layers.IPv4)
	if !ok {
		return "etc"
	}
	if filtOnly {
		return "ip"
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
