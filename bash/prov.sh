#!/bin/bash

function main() {
    Provision $@
}
function Provision() {
    curl -X PUT --insecure -H "Content-Type: application/json" -d '{"registration_code":"$1"}' https://localhost:8765/v1/provision
}
main
