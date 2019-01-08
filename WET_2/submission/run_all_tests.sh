#!/bin/bash

cd tests
for x in *.in; do
	dos2unix $x
done
cd ..
cd test_results
for x in *.out; do
	dos2unix $x
done
cd ..

echo "running valgrind"
valgrind --log-file="val_result" --leak-check=full ./a.out

k=0
for x in test_results/randTest*Result.out; do
	echo "running diff ${k}"
	diff test_results/randTest${k}Result.out test_results/randTest${k}.out
	let k++
done

cat "val_result"