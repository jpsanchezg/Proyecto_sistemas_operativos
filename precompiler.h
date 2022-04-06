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

/* Definición de constantes */
#define TAM_STRING 100
#define TAM_NOTICIA 80
#define MAXCARACT 640
#define LIMCHAR 50

const char ANIM_CARGA[] = {'/', '-', '\\', '|', '\0'};

struct News
{
    char category[TAM_STRING];
    char title[MAXCARACT];
};

struct ArgPublisher
{
    char pipePSC[LIMCHAR];
    char file[LIMCHAR];
    double timeN;
};

struct ArgSistem
{
    char pipePSC[LIMCHAR];
    char pipeSSC[LIMCHAR];
    double timeF;
};

struct ArgSubscriber
{
    char pipeSSC[LIMCHAR];
};

struct subscriber
{
    int id, selected;
    int categoryNews[5];
};

#endif