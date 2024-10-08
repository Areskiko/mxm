#!/usr/bin/env sh

mkdir -p /tmp/data
mkdir -p /tmp/workdir

pushd $(pwd)

for dir in $(git worktree list --porcelain | tr  "\n " ";;" | sed "s/;;/\n/g" | awk -F ";" '!/bare$/{print $2}')
do
	ln -s /tmp/data $dir/data
	ln -s /tmp/workdir $dir/workdir
	pushd $dir
	make benchmark
	clean_dir=$($dir | sed -E 's-.*/(.*)$-\1-')
	mv result.txt workdir/$clean_dir.txt
	popd
done

popd
mv /tmp/workdir/*.txt

