

main : main.o init.o xyzout.o xyzin.o histogram.o

	cc -o main main.o init.o xyzout.o xyzin.o histogram.o  -lm

main.o : main.c init.c
	cc -c main.c
init.o : init.c
	cc -c init.c
xyzout.o : xyzout.c
	cc -c xyzout.c
xyzin.o : xyzin.c
	cc -c xyzin.c
histogram.o : histogram.c
	cc -c histogram.c




clean :
	rm main main.o init.o  \
