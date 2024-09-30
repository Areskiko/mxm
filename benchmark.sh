#!/usr/bin/env sh

mkdir -p ../data
make ../data/2.dat

for dir in $(git worktree list --porcelain | tr  "\n " ";;" | sed "s/;;/\n/g" | awk -F ";" '!/bare$/{print $2}')
do
	ln -s ../data $dir/data
	pushd $dir
	make benchmark
	popd
done

