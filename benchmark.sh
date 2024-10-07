#!/usr/bin/env sh

mkdir -p ../data
mkdir -p ../workdir

for dir in $(git worktree list --porcelain | tr  "\n " ";;" | sed "s/;;/\n/g" | awk -F ";" '!/bare$/{print $2}')
do
	ln -s ../data $dir/data
	ln -s ../workdir $dir/workdir
	pushd $dir
	make benchmark
	mv result.txt workdir/$dir.log
	popd
done

