CC=gcc
TARGETS=2mach 2machasm

for target in $TARGETS
do
	$CC -o $target $target.c
done
