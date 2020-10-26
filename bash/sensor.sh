function main() {
    FetchPackages
    InstallPackages
}

function FetchPackages() {
    local packageDir="$(PackageDir)"
    local repo="$(PackageRepo)"
    local packages="$(Packages)"
    mkdir -p $packageDir

    pushd $packageDir
    for p in ${packages[@]}; do
        if [[ "${p}" ]]; then
            FetchPackage "${repo}/${p}"
        fi
    done
    popd
}

# Fetches a package from our upstream repository and drops it, named as-is
# into the current working directory
# add --fail so that curl returns error when url doesn't exist instead of a html doc.
# $1 package url
function FetchPackage() {
    local url="${1}"
    echo fetching package: "${url}"
    curl --fail -sSLO "${url}"
}

#$1 - package dir
function InstallPackages() {
    echo "Installing packages"
    local packageDir=$(PackageDir)
    local packages="$(Packages)"
    for package in ${packages[@]}; do
        echo "Installing ${packageDir}/${package}"
        apt install -y "${packageDir}/${package}"
    done
}

# app package repo to download
function PackageRepo() {
    echo "http://pkg.uswest2.icebrg.io"
}

function PackageDir() {
    echo "/opt/packages"
}

# array in the first param.
function Packages() {
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

main
