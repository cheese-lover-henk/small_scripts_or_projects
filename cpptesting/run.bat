@echo off
del main.exe
echo compiling... (g++)
echo ------------------
g++ -o main.exe -Iinclude main.cpp src\*.cpp -lgdi32
echo compiling done, running...
echo ------------------
main.exe
echo ------------------
echo done.