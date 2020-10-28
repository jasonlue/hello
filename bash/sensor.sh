#!/bin/bash

#configuration
SENSOR_PACKAGE_REPO="http://pkg.uswest2.icebrg.io"
SENSOR_PACKAGE_DIR="/opt/packages"
HOME_DIR="/home/jasonlu"
SENSOR_PACKAGES=(
    "ndpi_3.2.0-2358_amd64.deb"
    "pfring_7.6.0-2900_amd64.deb"
    "pfring-modules-4.19.0-9-amd64_7.6.0_amd64.deb"
    "hyperscan-5.0.0-Linux.deb"
    "libfiletyper1-0.2.5.deb"
    "icebrg-suricata-4.0.5-49.deb"
    "libssl1.0.2_1.0.2u-1_deb9u1_amd64.deb"
    "bro-2.5.2-0-Linux-x86_64.deb"
)
main() {
    bootstrap
    install_dev_packages
    fetchSensor_packages
    install_sensor_packages
}
bootstrap() {
    apt install -y curl git gcc g++ pkg-config tree htop jq ssh
    if [[ -d ${HOME_DIR}/hello ]]; then
        return
    fi
    git clone "https://github.com/jasonlue/hello"
    echo "PATH=${HOME_DIR}/hello/bash:${PATH}" >>${HOME_DIR}/.bashrc
    echo export GOSUMDB=off >>"${HOME_DIR}/.bashrc"
    source "${HOME_DIR}/.bashrc"
}
install_dev_packages() {
    "$(script_dir)"/git.sh
    "$(script_dir)"/go.sh
    "$(script_dir)"/vscode.sh
}
fetch_sensor_packages() {
    mkdir -p "${SENSOR_PACKAGE_DIR}"
    pushd "${SENSOR_PACKAGE_DIR}"
    for p in ${SENSOR_PACKAGES[@]}; do
        if [[ "${p}" ]]; then
            fetch_sensor_package "${SENSOR_PACKAGE_REPO}/${p}"
        fi
    done
    popd
}
# Fetches a package from our upstream repository and drops it, named as-is
# into the current working directory
# add --fail so that curl returns error when url doesn't exist instead of a html doc.
# $1 package url
fetch_sensor_package() {
    local url="${1}"
    echo fetching package: "${url}"
    curl --fail -sSLO "${url}"
}

#1: package dir
install_sensor_packages() {
    echo "Installing packages"
    for package in ${SENSOR_PACKAGES[@]}; do
        echo "Installing ${SENSOR_PACKAGE_DIR}/${package}"
        apt install -y "${SENSOR_PACKAGE_DIR}/${package}"
    done
}

function script_dir() {
    dir="$(
        cd "$(dirname "$0")" >/dev/nul 2>&1
        pwd -P
    )"
    echo $dir
}
#main has to be last for reference dependencies
main
