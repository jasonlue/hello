brctl addbr vbr0
ip tuntap add tap0 mode tap
brctl addif vbr0 tap0