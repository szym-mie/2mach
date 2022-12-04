@echo off

set CC=gcc
set TARGETS=2mach 2machasm

for %%T in (%TARGETS%) do 
	%CC% -o %%T %%T.c

