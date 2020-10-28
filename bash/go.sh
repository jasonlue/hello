#!/bin/bash
GO_VERSION="1.15.3"

main() {
    if exists_go_version ${GO_VERSION}; then
        echo Go version ${GO_VERSION} already installed.
    else
        install_go
    fi
}
install_go() {
    curl -O https://dl.google.com/go/go${GO_VERSION}.linux-amd64.tar.gz
    tar xvf go${GO_VERSION}.linux-amd64.tar.gz
    mv go /lib/go-${GO_VERSION}
    ln -fs /lib/go-${GO_VERSION}/bin/go /usr/bin/go

    #Install go-bindata
    GOPATH=/home/jasonlu/go-bindata go get -u github.com/go-bindata/go-bindata/...
    mv go-bindata /lib/go-bindata
    ln -fs /lib/go-bindata/bin/go-bindata /usr/bin/go-bindata

    #install delve
    GOPATH=/home/jasonlu/delve go get -u github.com/go-delve/delve/cmd/dlv
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
    if go version; then
        echo $(go version)
    else
        echo ""
    fi
}
main
