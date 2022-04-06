programa:
	@gcc -o publicador.o publicador.c
	@gcc -o sistemadecomunicacion.o sistemadecomunicacion.c
	@gcc -o suscriptor.o suscriptor.c

suscriptor: suscriptor.o sistemadecomunicacion.o
	@gcc -o suscriptor.exe suscriptor.o sistemadecomunicacion.o -lpthread

sistemadecomunicacion: sistemadecomunicacion.o
	@gcc -o sistemadecomunicacion.exe sistemadecomunicacion.o -lpthread

publicador: publicador.o sistemadecomunicacion.o
	@gcc -o publicador.exe publicador.o sistemadecomunicacion.o -lpthread


main: publicador.exe suscriptor.exe sistemadecomunicacion.exe
	@./publicador.exe & ./suscriptor.exe & ./sistemadecomunicacion.exe

clean:
	@rm -f publicador.exe suscriptor.exe sistemadecomunicacion.exe publicador.o suscriptor.o sistemadecomunicacion.o

	build:
	@gcc -pthread -o publicador.o publicador.c
	@gcc -pthread -o sistema.o sistema.c
	@gcc -pthread -o suscriptor.o suscriptor.c
