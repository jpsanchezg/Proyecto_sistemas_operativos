#include "pipes.h"

int Seleccionadas[5];
int contador = 0;

struct argumentosSus leerArgumentos(int cantidadArgumentos, char *arregloArgumentos[])
{
    struct argumentosSus tempArgumentos;
    if (cantidadArgumentos != 3)
    {
        errno = 8;
        perror("ERROR: Cantidad equivocada de argumentos");
    }
    else
    {
        for (int i = 0; i < cantidadArgumentos; i++)
        {
            if (strcmp(arregloArgumentos[i], "-s") == 0)
            {
                strcpy(tempArgumentos.pipeSSC, arregloArgumentos[i + 1]);
            }
        }
        return tempArgumentos;
    }
}

void SeleccionCat()
{

    char opcion;
    char Continuar;
    int salir = 1;

    while (salir == 1)
    {
        printf("Indique el numero segun la noticia que desea ver \n");
        printf("1. Arte \n");
        printf("2. Espectaculo \n");
        printf("3. ciencia \n");
        printf("4. Politica \n");
        printf("5. Sucesos \n");

        scanf("%s", &opcion);

        if (isdigit(opcion))
        {
            if ((atoi(&opcion)) > 0 && (atoi(&opcion)) <= 5)
            {
                int validador = 0;
                for (int i = 0; i < contador; i++)
                {
                    if (Seleccionadas[i] == atoi(&opcion))
                    {
                        validador = 1;
                        errno = 22;
                        perror("Opcion seleccionada anteriormente");
                    }
                }
                if (validador == 0)
                {
                    Seleccionadas[contador] = atoi(&opcion);
                    contador++;
                }
            }
            else
            {
                errno = 22;

                perror("Numero invalido");
            }
        }
        else
        {
            errno = 22;
            perror("Valor ingresado no es un numero");
        }

        int validador2 = 0;
        while (validador2 == 0)
        {

            printf("Si desea agregar otra categoria Introduzca 1 sino introduzca 2: ");
            scanf("%s", &Continuar);

            if (isdigit(Continuar))
            {
                if ((atoi(&Continuar)) == 1 || (atoi(&Continuar)) == 2)
                {
                    validador2 = 1;
                }
                else
                {
                    errno = 22;
                    perror("Numero invalido");
                }
            }
            else
            {
                errno = 22;
                perror("Valor ingresado no es un numero");
            }
        }
        if (atoi(&Continuar) == 2)
        {
            salir = 2;
        }
        else
        {
            salir = 1;
        }
    }

    for (int i = 0; i < contador; i++)
    {
        printf("Opcion %d:%d \n", i + 1, Seleccionadas[i]);
    }
}

void EnviarPipe(char pipe[])
{
    int creado, fd;
    do
    {
        fd = open(pipe, O_WRONLY | O_NONBLOCK);
        if (fd == -1)
            sleep(1);
    } while (fd == -1);
    int pid = getpid();
    write(fd, &pid, sizeof(int));
    write(fd, &contador, sizeof(int));
    write(fd, Seleccionadas, sizeof(Seleccionadas) * contador);
    close(fd);
}

int main(int argc, char const *argv[])
{
    SeleccionCat();
    struct argumentosSus entradaArgumentos;
    entradaArgumentos = leerArgumentos(argc, argv);
    EnviarPipe(entradaArgumentos.pipeSSC);
    while (true)
    {
        // simulacion de suscriptor esperando
    }
    return 0;
}
