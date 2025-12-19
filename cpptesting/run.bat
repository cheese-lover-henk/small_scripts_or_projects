@echo off
echo compiling... (g++)
echo ------------------
g++ -o main.exe -I . -I ./src *.cpp src/*.cpp
echo compiling done, running...
echo ------------------
main.exe
echo ------------------
echo done.