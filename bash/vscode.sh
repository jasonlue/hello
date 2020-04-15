#!/bin/bash
#vscode
#code package is located at  https://packages.microsoft.com/repos 
#add repo link
add-apt-repository "deb [arch=amd64] https://packages.microsoft.com/repos/vscode stable main"
#Import repo key
wget -q https://packages.microsoft.com/keys/microsoft.asc -O- | sudo apt-key add -
#update apt cache
apt update
#install code
sudo apt install code



