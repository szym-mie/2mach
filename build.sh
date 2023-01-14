CC=gcc
TARGETS=2mach 2machasm

for target in $TARGETS
do
	echo -e Building $target :\n
	$CC -o $target.out $target.c
	echo ---
done
