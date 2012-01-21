#!/bin/sh
mkdir cliphist2-1.0.1
#./do_file_list.sh
tar -cf - -T src_files.lst | tar -xpf - -C cliphist2-1.0.1
tar -z -cf cliphist2-1.0.1.tar.gz cliphist2-1.0.1/*
rm -rf cliphist2-1.0.1/
#list: tar -tvf cliphist2-1.0.1.tar.gz