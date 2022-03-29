/**************************************************************************
*   Sistemas Operativos Proyecto Entrega 2: Prestamo de libros
*   Miembros:
*       - Juan Sebastián Barreto Jimenéz
*       - Janet Chen He
*       - Juan Camilo Merchan Loza
*   File: 
*       pipes.h
*   Proposito: 
*       Header que contiene variables, estructuras y funciones necesarias
*       para el uso de pipes.
**************************************************************************/
#ifndef PIPES_H
#define PIPES_H

/* Inclusión de archivos */
#include "precompiler.h"

/* Definición de constantes */
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
    //printf("Se ha abierto el pipe %s\n", nombrePipe);
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

/**************************************************************************
*   Función:   leerPipe()
*   Proposito:  Rutina para leer el pipe que se pasa por parametro.
*   Argumentos:
*       fd: Descriptor del pipe que se va a leer.
*       mensaje: Cadena de caracteres donde se almacenara lo leído.
*   Retorno:
*       void
**************************************************************************/
void leerPipe(int fd, char mensaje[TAM_STRING])
{
    while (read(fd, mensaje, TAM_STRING) == 0);
} // fin leerPipe

#endif