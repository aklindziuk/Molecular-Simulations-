

mdlj : mdlj.o  init.o xyz_out.o

	cc -o mdlj mdlj.o  init.o xyz_out.o  -L ~/gsl/lib -I ~/gsl/include -lgsl -lgslcblas -lm

mdlj.o : mdlj.c
	cc -c mdlj.c

init.o : init.c
	cc -c init.c
xyz_out.o: xyz_out.c
	cc -c xyz_out.c



clean :
	rm main main.o init.o  \
