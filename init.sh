#/bin/bash

/usr/bin/vmhgfs-fuse .host:/ ~/shares -o subtype=vmhgfs-fuse,allow_other
