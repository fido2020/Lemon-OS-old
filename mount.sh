 #!/bin/bash

sudo modprobe nbd
sudo qemu-nbd -c /dev/nbd0 Disks/Lemon.vhd
sudo mount /dev/nbd0p1 /mnt