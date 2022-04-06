#ifndef PIPES_H
#define PIPES_H

/* Inclusión de archivos */
#include "precompiler.h"
#include <errno.h>
#define TAM_PIPES 100

/* Funciones */
/**************************************************************************
 *   Función:   limpiarTerminal()
 *   Proposito:  Rutina para limpiar el terminal
 *   Argumentos:
 *       void
 *   Retorno:
 *       void
 **************************************************************************/
void limpiarTerminal()
{
    printf("\033[2J");
    printf("\033[H");
}

/**************************************************************************
 *   Función:   abrirPipe()
 *   Proposito:  Rutina para abrir el pipe que tiene el nombre pasado
 *               por parametro, en caso de no estar creado se pone a
 *               esperar a que sea abierto.
 *   Argumentos:
 *       nombrePipe: Cadena de caracteres que tiene el nombre del pipe
 *                   que se va a abrir.
 *   Retorno:
 *       int: Se retorna descriptor del pipe que es abierto.
 **************************************************************************/
int abrirPipe(char nombrePipe[TAM_STRING])
{
    int fd;
    int counter = 0;
    int tamAnim = strlen(ANIM_CARGA);
    limpiarTerminal();
    do
    {
        fd = open(nombrePipe, O_WRONLY | O_NONBLOCK);
        if (fd == -1)
        {
            printf("\rEsperando creacion del pipe %c", ANIM_CARGA[counter]);
            fflush(stdout);
            counter++;
            counter %= tamAnim;
            sleep(1);
            printf("\r                                \r");
            fflush(stdout);
        }
    } while (fd == -1);
    limpiarTerminal();
    // printf("Se ha abierto el pipe %s\n", nombrePipe);
    sleep(1);
    return fd;
} // fin abrirPipe

/**************************************************************************
 *   Función:   crearPipe()
 *   Proposito:  Rutina para crear y abrir el pipe que tiene el nombre
 *                pasado por parametro.
 *   Argumentos:
 *       nombrePipe: Cadena de caracteres que tiene el nombre del pipe
 *                   que se va a abrir.
 *   Retorno:
 *       int: Se retorna descriptor del pipe que es creado y abierto.
 **************************************************************************/
int crearPipe(char nombrePipe[])
{
    int fd;
    unlink(nombrePipe);
    mkfifo(nombrePipe, 0);
    chmod(nombrePipe, 460);
    fd = open(nombrePipe, O_RDONLY);
    return fd;
} // fin crearPipe

void sendPipe(char pipe[], int countNews, struct News publication[TAM_STRING])
{
    int creado, fd;
    do
    {
        fd = open(pipe, O_WRONLY | O_NONBLOCK);
        if (fd == -1)
            sleep(1);
    } while (fd == -1);
    write(fd, &countNews, sizeof(int));
    write(fd, publication, sizeof(publication) * countNews);
    close(fd);
}

/**************************************************************************
 *   Función:   leerPipe()
 *   Proposito:  Rutina para leer el pipe que se pasa por parametro.
 *   Argumentos:
 *       fd: Descriptor del pipe que se va a leer.
 *       mensaje: Cadena de caracteres donde se almacenara lo leído.
 *   Retorno:
 *       void
 **************************************************************************/

void leerPipePubli(char *pipe, int fd)
{
    struct Noti noticias[100];

    int n, bytes, bytes2;

    bytes = read(fd, &n, sizeof(int));

    bytes2 = read(fd, noticias, sizeof(struct Noti) * n);

    ordenarNoticias(noticias, n);
}

void pipePubli(char *pipe)
{
    while (true)
    {
        int fd = open(pipe, O_RDONLY);
        leerPipePubli(pipe, fd);
        printf("----Publicacion agregada----\n");
        temporizador = 0;
        mostrarPublicacionNueva();
        close(fd);
    }
}

int leerPipeSus(char *pipe, int fd)
{
    int Seleccionadas[5];

    int pid, n, bytes, bytes2, bytepid;

    bytepid = read(fd, &pid, sizeof(int));
    bytes = read(fd, &n, sizeof(int));
    bytes2 = read(fd, Seleccionadas, sizeof(int) * n);

    sus[numSuscriptores].id = pid;                  // Se guarda el ID del suscriptor
    sus[numSuscriptores].cantidadSeleccionadas = n; // Se guarda la cantidad de noticias que selecciono el suscriptor
    for (int i = 0; i < n; i++)
    {
        sus[numSuscriptores].seleccionadas[i] = Seleccionadas[i]; // Se guarda las noticias que selecciono el suscriptor
    }
    numSuscriptores++; // Se aumenta el numero de suscriptores

    return bytes2;
}

void pipeSus(char *pipe)
{
    while (true)
    {
        int fd = open(pipe, O_RDONLY);
        leerPipeSus(pipe, fd);
        printf("---Suscriptor agregado---\n");
        mostrarPubliNuevoSus();
        close(fd);
    }
}

#endif
