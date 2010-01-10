#!/bin/sh
mkdir cliphist2-1.0.0
./do_file_list.sh
tar -cf - -T file.lst | tar -xpf - -C cliphist2-1.0.0
tar -z -cf cliphist2-1.0.0.tar.gz cliphist2-1.0.0/*
rm -rf cliphist2-1.0.0/
#list: tar -tvf cliphist2-1.0.0.tar.gz