scp -i ~/.ssh/id_rsa_provisioning -o ProxyCommand="ssh -W %h:%p -i ~/.ssh/id_rsa jlu@10.11.2.126" $1 $2
