#/bin/bash

sudo systemctl restart ssh
sudo /usr/bin/vmhgfs-fuse .host:/ ~/shares -o subtype=vmhgfs-fuse,allow_other
