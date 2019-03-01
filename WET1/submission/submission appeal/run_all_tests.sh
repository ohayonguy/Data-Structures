#!/bin/bash

for x in *.txt; do
	dos2unix $x
done

i=1
for x in in*.txt; do
	./a.out < in${i}.txt > out_result${i}.txt
	let i++
done

k=1
for x in out_result*.txt; do
	echo "running diff ${k}"
	diff out${k}.txt out_result${k}.txt
	let k++
done

k=1
for x in in*.txt; do
	echo "running valgrind ${k}"
	valgrind --log-file="val_result${k}.txt" --leak-check=full ./a.out < in${k}.txt > val_std_out${k}.txt
	let k++
done

k=1
for x in val_result*.txt; do
	cat $x
done