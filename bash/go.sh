
apt install curl

#install go
VERSION=1.12.17
curl -O https://dl.google.com/go/go${VERSION}.linux-amd64.tar.gz
tar xvf go${VERSION}.linux-amd64.tar.gz
mv go /lib/go-${VERSION}
ln -fs /lib/go-${VERSION}/bin/go /usr/bin/go

#Install go-bindata
GOPATH=/home/jasonlu/go-bindata go get -u github.com/go-bindata/go-bindata/...
mv go-bindata /lib/go-bindata
ln -fs /lib/go-bindata/bin/go-bindata /usr/bin/go-bindata
