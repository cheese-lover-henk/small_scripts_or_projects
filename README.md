## cpptesting:
a very flawed attempt at creating my first selfmade 2d game engine + software renderer.
some parts i made too complicated, and other parts i didnt think about or understand well enough.
it was a solid learning experience though. might try again later.

right now, it compiles and runs, but very poorly. only around 30fps.
and thats without any game or ui calculations and very little rendering related code.
its not capable of safely swapping between image buffers yet, so it cant display anything rendered either.
the communication between threads is a bit messy, and apparently windowsapi with WM_PAINT on
purely the cpu (without using a graphics api) is very limited. 