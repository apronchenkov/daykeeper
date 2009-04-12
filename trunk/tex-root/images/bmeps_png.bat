@echo off

for %%i in (*.png) do (
	.\bmeps\bmeps.exe -p3 -c -e8f -tpng "%%i" "%%~Ni.eps"
)