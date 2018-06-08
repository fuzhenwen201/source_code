#! /bin/sh

for i in `find ~/ -name '*.*cpp'`
do
	echo $i
    ctags -a -f ~/tags $i	
done


for i in `find ~/ -name '*.*h'`
do
	echo $i
	ctags -a -f ~/tags $i
done

