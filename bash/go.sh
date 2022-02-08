#!/bin/bash
SUCCESS=0
FAIL=1
main() {
    local version 
    if ! get_latest_go_version version; then 
        printf "get_latest_go_version failed."
        return $FAIL 
    elif exists_go_version $version; then
        printf "Go %s  already installed.\n"  $version
        return $SUCCESS
    elif ! install_go $version; then
        printf "install_go %s failed.\n" $version
        return $FAIL 
    elif ! setup_go_path; then 
        printf "setup_go_path failed.\n"
        return $FAIL
    elif ! install_go_extra; then 
        printf "install_go_extra failed.\n"
        return $FAIL
    else 
        printf "successful!\n"
        return $SUCCESS
    fi
}

#########################################
# latest_go_version version
#########################################
get_latest_go_version(){
    local -n glgv_version=$1
    glgv_version=$(curl -s https://go.dev/VERSION?m=text)
    if (($? != $SUCCESS )); then 
        printf "curl -s https://go.dev/VERSION?m=text failed.\n" 
        return $FAIL
    elif [[ -z "$glgv_version" ]]; then
        printf "go version is empty.\n"
        return $FAIL
    fi 
    glgv_version=${glgv_version#go}
    printf "latest go version is %s\n" $glgv_version
    return $SUCCESS
}

#########################################
# install_go $version
#########################################
install_go() {
    local version=$1
    local tarball=go$version.linux-amd64.tar.gz
    cd $HOME
    if ! curl -O https://dl.google.com/go/$tarball; then 
        printf "curl -O https://dl.google.com/go/%s failed.\n" $tarball
        return $FAIL 
    fi 
    rm -rf $HOME/go
    if ! tar -xvf $tarball; then 
        return $FAIL 
    fi 
    rm $tarball
    return $SUCCESS
}

#########################################
# setup_go_path 
#########################################
setup_go_path(){
    echo "PATH=$HOME/go/bin:\$PATH" > /etc/profile.d/go_path.sh # setup for next login
    . /etc/profile.d/go_path.sh # set up for now.
}

#########################################
# install_go_extra
#   install extra tools to /root/go/bin
#########################################
install_go_extra(){
    #Install go-bindata
    go install github.com/go-bindata/go-bindata/...@latest
    #install delve
    go install github.com/go-delve/delve/cmd/dlv@latest
    #install go-imports
    go install golang.org/x/tools/cmd/goimports@latest
}

exists_go_version() {
    local version="$(go_version)"
    if [[ "$version" =~ "$1" ]]; then
        return $SUCCESS
    else
        return $FAIL
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
