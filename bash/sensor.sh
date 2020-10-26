#!/bin/bash

function main() {
    Bootstrap
    InstallDevPackages
    FetchSensorPackages
    InstallSensorPackages
}
function Bootstrap() {
    apt install -y curl git gcc g++ pkg-config tree htop jq ssh
    git clone https://github.com/jasonlue/hello
    echo "PATH=$(HomeDir)/hello/bash:$PATH" >>$(HomeDir)/.bashrc
    echo export GOSUMDB=off >>$(HomeDir)/.bashrc
    source $(HomeDir)/.bashrc
}
function InstallDevPackages() {
    "$(ScriptDir)"/git.sh
    "$(ScriptDir)"/go.sh
    "$(ScriptDir)"/vscode.sh
}
function FetchSensorPackages() {
    local packageDir="$(SensorPackageDir)"
    local repo="$(SensorPackageRepo)"
    local packages="$(SensorPackages)"
    mkdir -p $packageDir

    pushd $packageDir
    for p in ${packages[@]}; do
        if [[ "${p}" ]]; then
            FetchSensorPackage "${repo}/${p}"
        fi
    done
    popd
}
# Fetches a package from our upstream repository and drops it, named as-is
# into the current working directory
# add --fail so that curl returns error when url doesn't exist instead of a html doc.
# $1 package url
function FetchSensorPackage() {
    local url="${1}"
    echo fetching package: "${url}"
    curl --fail -sSLO "${url}"
}

#$1 - package dir
function InstallSensorPackages() {
    echo "Installing packages"
    local packageDir=$(SensorPackageDir)
    local packages="$(SensorPackages)"
    for package in ${packages[@]}; do
        echo "Installing ${packageDir}/${package}"
        apt install -y "${packageDir}/${package}"
    done
}

# app package repo to download
function SensorPackageRepo() {
    echo "http://pkg.uswest2.icebrg.io"
}
function SensorPackageDir() {
    echo "/opt/packages"
}
# array in the first param.
function SensorPackages() {
    echo '
    ndpi_3.2.0-2358_amd64.deb
    pfring_7.6.0-2900_amd64.deb
    pfring-modules-4.19.0-9-amd64_7.6.0_amd64.deb
    hyperscan-5.0.0-Linux.deb
    libfiletyper1-0.2.5.deb
    icebrg-suricata-4.0.5-49.deb
    libssl1.0.2_1.0.2u-1_deb9u1_amd64.deb
    bro-2.5.2-0-Linux-x86_64.deb
    '
}
function HomeDir() {
    echo "/home/jasonlu"
}
function ScriptDir() {
    dir="$(
        cd "$(dirname "$0")" >/dev/nul 2>&1
        pwd -P
    )"
    echo $dir
}
#main has to be last for reference dependencies
main
