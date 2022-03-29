/**************************************************************************
*   Sistemas Operativos Proyecto Entrega 2: Prestamo de libros
*   Miembros:
*       - Juan Sebastián Barreto Jimenéz
*       - Janet Chen He
*       - Juan Camilo Merchan Loza
*   File: 
*       precompiler.h
*   Proposito: 
*       Header que contiene las inclusiones de librerias y constantes
*       necesarias para los códigos.
**************************************************************************/
#ifndef PRECOMPILER_H
#define PRECOMPILER_H

/* Inclusión de archivos */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <time.h>
#include <semaphore.h>

/* Definición de constantes */
#define TAM_STRING 100
#define TAM_NOMBRE 20
#define MAX_EJEMPLARES 10
#define TAM_ISBN 5
#define TAM_EJEMPLAR 5
#define APERTURA_PIPE 'A'
#define CIERRE_PIPE 'C'
#define DEVOLVER 'D'
#define RENOVAR 'R'
#define SOLICITAR 'P'
#define POSITIVO 'T'
#define NEGATIVO 'F'
#define DISPONIBLE 'D'
#define PRESTADO 'P'
#define DELIM ","

const char ANIM_CARGA[] = {'/', '-', '\\', '|', '\0'};

#endif