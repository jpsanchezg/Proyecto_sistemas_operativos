
#include "pipes.h"
#include <stdio.h>

struct ArgSistem entryArguments;

struct Noti notA[100]; // 1
int numA = 0;
struct Noti notE[100]; // 2
int numE = 0;
struct Noti notC[100]; // 3
int numC = 0;
struct Noti notP[100]; // 4
int numP = 0;
struct Noti notS[100]; // 5
int numS = 0;

int temporizador = 0;

struct suscriptor sus[100]; // vector de suscriptores
int numSuscriptores = 0;    // numero de suscriptores

void comprobadorTiempo()
{
    while (true)
    {
        if (temporizador == entradaArgumentos.timeF)
        {
            for (int i = 0; i < numSuscriptores; i++)
            {
                kill(sus[i].id, SIGKILL);
            }
            kill(getpid(), SIGKILL);
        }
        sleep(1);
        temporizador++;
    }
}

void mostrarPublicacionNueva()
{
    printf("Numero de suscriptores: %d \n", numSuscriptores);
    printf("_________________________________________________________________________________________\n");
    for (int i = 0; i < numSuscriptores; i++)
    {
        printf("_________________________________________________________________________________________\n");
        printf("-------Sus id# %d \n", sus[i].id);
        printf("Categorias: \n");
        for (int x = 0; x < sus[i].cantidadSeleccionadas; x++)
        {
            if (sus[i].seleccionadas[x] == 1)
            {
                printf("Noticias de Arte: \n");
                for (int i = 0; i < numA; i++)
                {
                    printf("%s \t %s \n", notA[i].categoria, notA[i].noticia);
                }
            }
            if (sus[i].seleccionadas[x] == 2)
            {
                printf("Noticias de Espectaculo: \n");
                for (int i = 0; i < numE; i++)
                {
                    printf("%s \t %s \n", notE[i].categoria, notE[i].noticia);
                }
            }
            if (sus[i].seleccionadas[x] == 3)
            {
                printf("Noticias de Ciencia: \n");
                for (int i = 0; i < numC; i++)
                {
                    printf("%s \t %s \n", notC[i].categoria, notC[i].noticia);
                }
            }
            if (sus[i].seleccionadas[x] == 4)
            {
                printf("Noticias de Politica: \n");
                for (int i = 0; i < numP; i++)
                {
                    printf("%s \t %s \n", notP[i].categoria, notP[i].noticia);
                }
            }
            if (sus[i].seleccionadas[x] == 5)
            {
                printf("Noticias de Sucesos: \n");
                for (int i = 0; i < numS; i++)
                {
                    printf("%s \t %s \n", notS[i].categoria, notS[i].noticia);
                }
            }
        }
        printf("_________________________________________________________________________________________\n");
    }
}

void mostrarPubliNuevoSus()
{
    printf("_________________________________________________________________________________________\n");
    int i = numSuscriptores - 1;
    printf("-------Sus id# %d \n", sus[i].id);
    printf("Categorias: \n");
    for (int x = 0; x < sus[i].cantidadSeleccionadas; x++)
    {
        if (sus[i].seleccionadas[x] == 1)
        {
            printf("Noticias de Arte: \n");
            for (int i = 0; i < numA; i++)
            {
                printf("%s \t %s \n", notA[i].categoria, notA[i].noticia);
            }
        }
        if (sus[i].seleccionadas[x] == 2)
        {
            printf("Noticias de Espectaculo: \n");
            for (int i = 0; i < numE; i++)
            {
                printf("%s \t %s \n", notE[i].categoria, notE[i].noticia);
            }
        }
        if (sus[i].seleccionadas[x] == 3)
        {
            printf("Noticias de Ciencia: \n");
            for (int i = 0; i < numC; i++)
            {
                printf("%s \t %s \n", notC[i].categoria, notC[i].noticia);
            }
        }
        if (sus[i].seleccionadas[x] == 4)
        {
            printf("Noticias de Politica: \n");
            for (int i = 0; i < numP; i++)
            {
                printf("%s \t %s \n", notP[i].categoria, notP[i].noticia);
            }
        }
        if (sus[i].seleccionadas[x] == 5)
        {
            printf("Noticias de Sucesos: \n");
            for (int i = 0; i < numS; i++)
            {
                printf("%s \t %s \n", notS[i].categoria, notS[i].noticia);
            }
        }
    }
    printf("_________________________________________________________________________________________\n");
}

struct argumentosS leerArgumentos(int cantidadArgumentos, char *arregloArgumentos[])
{
    struct argumentosS tempArgumentos;
    if (cantidadArgumentos != 7)
    {
        errno = 8;
        perror("ERROR: Cantidad equivocada de argumentos");
    }
    else
    {
        for (int i = 0; i < cantidadArgumentos; i++)
        {
            if (strcmp(arregloArgumentos[i], "-p") == 0)
            {
                strcpy(tempArgumentos.pipePSC, arregloArgumentos[i + 1]);
            }

            if (strcmp(arregloArgumentos[i], "-s") == 0)
            {
                strcpy(tempArgumentos.pipeSSC, arregloArgumentos[i + 1]);
            }

            if (strcmp(arregloArgumentos[i], "-t") == 0)
            {
                char *temp;
                tempArgumentos.timeF = strtod(arregloArgumentos[i + 1], &temp);
            }
        }
        return tempArgumentos;
    }
}

void ordenarNoticias(struct Noti noticias[100], int tam)
{
    int intento = 0;
    for (int i = 0; i < tam; i++)
    {
        if (!strcmp(noticias[i].categoria, "A"))
        {
            notA[numA] = noticias[i];
            numA++;
        }

        if (!strcmp(noticias[i].categoria, "E"))
        {
            notE[numE] = noticias[i];
            numE++;
        }

        if (!strcmp(noticias[i].categoria, "C"))
        {
            notC[numC] = noticias[i];
            numC++;
        }

        if (!strcmp(noticias[i].categoria, "P"))
        {
            notP[numP] = noticias[i];
            numP++;
        }

        if (!strcmp(noticias[i].categoria, "S"))
        {
            notS[numS] = noticias[i];
            numS++;
        }
        intento++;
    }
}

void imprimirNoticias()
{
    printf("Noticias A: \n");
    for (int i = 0; i < numA; i++)
    {
        printf("%s \t %s \n", notA[i].categoria, notA[i].noticia);
    }
    printf("Noticias E: \n");
    for (int i = 0; i < numE; i++)
    {
        printf("%s \t %s \n", notE[i].categoria, notE[i].noticia);
    }
    printf("Noticias C: \n");
    for (int i = 0; i < numC; i++)
    {
        printf("%s \t %s \n", notC[i].categoria, notC[i].noticia);
    }
    printf("Noticias P: \n");
    for (int i = 0; i < numP; i++)
    {
        printf("%s \t %s \n", notP[i].categoria, notP[i].noticia);
    }
    printf("Noticias S: \n");
    for (int i = 0; i < numS; i++)
    {
        printf("%s \t %s \n", notS[i].categoria, notS[i].noticia);
    }
}

int main(int argc, char *args[])
{
    entradaArgumentos = leerArgumentos(argc, args);

    // abrir pipe Publicador
    mode_t fifo_mode1 = S_IRUSR | S_IWUSR;
    unlink(entradaArgumentos.pipePSC);
    if (mkfifo(entradaArgumentos.pipePSC, fifo_mode1) == -1)
    {
        perror("mkfifo Pub");
        exit(1);
    }
    // abrir pipe suscriptor
    mode_t fifo_mode2 = S_IRUSR | S_IWUSR;

    unlink(entradaArgumentos.pipeSSC);
    if (mkfifo(entradaArgumentos.pipeSSC, fifo_mode2) == -1)
    {
        perror("mkfifo SUS");
        exit(1);
    }

    pthread_t thread_id1, thread_id2, thread_id3;
    int rc1, rc2, rc3, *salida1, *salida2, *salida3;
    rc1 = pthread_create(&thread_id1, NULL, pipePubli, entradaArgumentos.pipePSC);
    if (rc1)
    {
        printf("Error: %d: %s\n", rc1, strerror(rc1));
        exit(-1);
    }
    /*
    while(true){
        //leer ambos pipes
        // if(bytes==0){
        //    sleep(timef);
        //    read de ambos
        //    if(bytes==0) break
        //}
        //
    }
    */

    rc2 = pthread_create(&thread_id2, NULL, pipeSus, entradaArgumentos.pipeSSC);
    if (rc2)
    {
        printf("Error: %d: %s\n", rc2, strerror(rc2));
        exit(-1);
    }
    rc3 = pthread_create(&thread_id3, NULL, comprobadorTiempo, NULL);
    if (rc3)
    {
        printf("Error: %d: %s\n", rc3, strerror(rc3));
        exit(-1);
    }

    pthread_join(thread_id1, (void **)&salida1);
    pthread_join(thread_id2, (void **)&salida2);

    pthread_exit(NULL);
    pthread_exit(NULL);
    return 0;
}
