#!/bin/bash
function main() {
    if ExistsGoVersion $(ExpectedGoVersion); then
        echo Go version $(ExpectedGoVersion) already installed.
    else
        InstallGo
    fi
}
function InstallGo() {
    version=$(ExpectedGoVersion)
    curl -O https://dl.google.com/go/go${version}.linux-amd64.tar.gz
    tar xvf go${version}.linux-amd64.tar.gz
    mv go /lib/go-${version}
    ln -fs /lib/go-${version}/bin/go /usr/bin/go

    #Install go-bindata
    GOPATH=/home/jasonlu/go-bindata go get -u github.com/go-bindata/go-bindata/...
    mv go-bindata /lib/go-bindata
    ln -fs /lib/go-bindata/bin/go-bindata /usr/bin/go-bindata

    #install delve
    GOPATH=/home/jasonlu/delve go get -u github.com/go-delve/delve/cmd/dlv
}
function ExistsGoVersion() {
    version="$(GoVersion)"
    if [[ "$version" =~ "$1" ]]; then
        return 0
    else
        return 1
    fi
}
function GoVersion() {
    if go version; then
        echo $(go version)
    else
        echo ""
    fi
}
function ExpectedGoVersion() {
    echo "1.15.3"
}
main
