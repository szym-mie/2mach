@echo off

set CC=tcc
set TARGETS=2mach 2machasm

for %%T in (%TARGETS%) do (
	echo Building %%T :
	echo.
	%CC% -o %%T.exe %%T.c
	echo ---
)

