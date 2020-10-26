#install git
apt install git

#config git
git config --global url.git@github.icebrg.io:.insteadOf https://github.icebrg.io/
git config --global url.git@github.com:.insteadOf https://github.com/
git config --global user.name "Jason Lu"
git config --global user.email jason.lu@gigamon.com
git config --global push.default simple
git config --list