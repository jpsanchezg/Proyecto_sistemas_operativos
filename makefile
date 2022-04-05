programa:
	@gcc -o publicador.o publicador.c
	@gcc -o sistemadecomunicacion.o sistemadecomunicacion.c
	@gcc -o suscriptor.o suscriptor.c

suscriptor: suscriptor.o sistemadecomunicacion.o
	@gcc -o suscriptor.exe suscriptor.o sistemadecomunicacion.o -lpthread

sistemadecomunicacion: sistemadecomunicacion.o
	@gcc -o sistemadecomunicacion.exe sistemadecomunicacion.o -lpthread

