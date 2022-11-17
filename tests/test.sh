#!/bin/bash
cd ..
cd build/lab2
#tests=$(find /root/projects/os_lab_2-main/tests -name "*.txt")
tests=$(find -name "*.txt")

make
count=1
for i in ${tests}
do
echo $i
    echo "-------------------------test $count-------------------------"
    ./lab2 < "$i"
    echo "\n"
    let "count += 1"
done
