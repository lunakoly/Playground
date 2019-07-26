@echo off

set SUCCEEDED=false

echo Compiling...
g++ --std=c++17 %1 -o "runnable.exe" && set SUCCEEDED=true


if "%SUCCEEDED%"=="true" (
	echo Running:
	runnable.exe
)