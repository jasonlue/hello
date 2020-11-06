#!/bin/bash
# 1 - image_name
# 2 - size_g

main(){
    local image=${1:-debian.qcow2}
    local size="${2:-10}G"

    if [[ -f $image ]]; then 
        printf "image %s already exists.\n" $image 
    else 
        qemu-img create -f qcow2 $image $size
    fi
}

main $@