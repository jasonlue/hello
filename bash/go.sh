#!/bin/bash
GO_VERSION="1.15.10"
USER=jlu
main() {
    if exists_go_version ${GO_VERSION}; then
        echo Go version ${GO_VERSION} already installed.
    else
        install_go
        install_go_extra
    fi
}
install_go() {
    cd ~
    curl -O https://dl.google.com/go/go${GO_VERSION}.linux-amd64.tar.gz
    tar xvf go${GO_VERSION}.linux-amd64.tar.gz
    sudo ln -s /home/${USER}/go/bin/go /usr/bin/go
}

install_go_extra(){
    #Install go-bindata
    GOPATH=/home/${USER}/go-bindata go get -u github.com/go-bindata/go-bindata/...
    
    #install delve
    GOPATH=/home/${USER}/delve go get -u github.com/go-delve/delve/cmd/dlv
    
    #install go-imports
    GOPATH=/home/${USER}/goimports go get -u golang.org/x/tools/cmd/goimports
}
exists_go_version() {
    version="$(go_version)"
    if [[ "${version}" =~ "$1" ]]; then
        return 0
    else
        return 1
    fi
}
go_version() {
    if ! which go > /etc/null; then
        echo ""
    else
        echo $(go version)
    fi
}

if (( $# == 0 )); then
    main
else
    eval $1
fi
