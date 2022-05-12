#!/bin/bash
qemu-system-x86_64 -m 64 -fda ./Disk.img -rtc base=localtime -M pc # -S -s

