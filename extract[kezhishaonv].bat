@echo off
for %%i in (out/*.dat) do extract out/%%i
cd out
del *.dat
pause