 #!/bin/bash

sudo modprobe nbd
sudo umount /mnt
sudo qemu-nbd -d /dev/nbd0