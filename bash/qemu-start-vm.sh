#!/bin/bash

QEMU=qemu-system-x86_64
MACHINE=q35
GIGA_MEMORY="4096"
CPU="Nehalem"
CORES="4"
MACHINE="q35,accel=hvf"
DISPLAY="display default,show-cursor=on"
IMAGE="${1:-debian.qcow2}"
ISO="${2:-}"

# Set to true to enable mounting the shared virtfs folder
ENABLE_VIRTFS="false"

# management network
MANAGEMENT_NETWORK_MAC=00:1D:AC:43:00:01

# capture network
CAPTURE_NETWORK_MAC=00:1D:AC:44:00:01

QEMU_CMD=""
append_qemu_argument(){
    QEMU_CMD+="${*} "
}

append_qemu_argument "${QEMU}"
append_qemu_argument "-usb -device usb-tablet"
append_qemu_argument "-machine ${MACHINE} -cpu ${CPU} -smp ${CORES} -m ${GIGA_MEMORY}"
append_qemu_argument "-netdev user,id=management_network,hostfwd=tcp::2222-:22,hostfwd=tcp::8765-:8765"
append_qemu_argument "-device driver=virtio-net,netdev=management_network,mac=${MANAGEMENT_NETWORK_MAC}"
#append_qemu_argument "-netdev tap,id=capture_network,ifname=tap0,script=no,downscript=no"
#append_qemu_argument "-device driver=virtio-net,netdev=capture_network,mac=${CAPTURE_NETWORK_MAC}"
append_qemu_argument "-drive format=qcow2,file=${IMAGE}"

if [[ "${ISO}" ]]; then
    append_qemu_argument "-cdrom ${ISO}"
fi

printf "${QEMU_CMD}\n"
eval ${QEMU_CMD}