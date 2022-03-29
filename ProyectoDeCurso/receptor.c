#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>

#define otherSize 100
#define bufferSize 10
#define INS_DEV 'D'
#define INS_SOL 'P'
#define INS_REN 'R'
#define CONF 'C'
#define NEGA 'N'
#define SEP ","

struct Pipe
{
    int fd;
    char nombre[otherSize];
};

int contIns;
int solicitud;
int respuesta;
char nombrePR[otherSize];
char buffer[bufferSize][otherSize];
struct Pipe pipes[100];

void removerInicio()
{
    for (int i = 0; i < contIns - 1; i++)
    {
        strcpy(buffer[i], buffer[i + 1]);
    }
    contIns--;
}

void relleno(){
    for(int i=0;i<100;i++){
        pipes[i].fd=-1;
        strcpy(pipes[i].nombre , "-1");
    }
}

void obtenerComando(char *respuesta)
{
    if (contIns != 0)
    {
        strcpy(respuesta, buffer[0]);
        removerInicio();
    }
}

void agregar(char *comando)
{
    strcpy(buffer[contIns], comando);
    contIns++;
}

void openPipe(char *nombre)
{
    int fd;

    unlink(nombre);
    mkfifo(nombre, 0);
    chmod(nombre, 460);
    fd = open(nombre, O_RDONLY);
    solicitud = (fd);
}

void writer(char *nombre)
{
    int fd;
    do
    {
        fd = open(nombre, O_WRONLY | O_NONBLOCK);
        if (fd == -1)
        {
            printf("\e[1;1H\e[2J");
            printf("Abriendo pipe.\n");
            printf("\e[1;1H\e[2J");
            sleep(1);
            printf("Abriendo pipe..\n");
            printf("\e[1;1H\e[2J");
            sleep(1);
            printf("Abriendo pipe...\n");
            printf("\e[1;1H\e[2J");
            sleep(1);
        }

    } while (fd == -1);
    respuesta = fd;
}

void crearPipe(char *datos)
{
    char mensaje[otherSize];

    for (int i = 0; i < 100; i++)
    {
        if (strcmp(pipes[i].nombre,"-1") == 0 || access(pipes[i].nombre,F_OK)!=0)
        {
            writer(datos);
            pipes[i].fd = respuesta;
            strcpy(pipes[i].nombre,datos);
            break;
        }
    }

    strcpy(mensaje, "Se ha creado el pipe exitosamente");
    write(respuesta, mensaje, otherSize);
}

void manejoInfo()
{
    char informacion[otherSize];
    char *token;
    char *numeritoRaro;

    char mensaje[otherSize];
    char servicio;

    int num = 0;

    while (1)
    {
        respuesta = -1;
        if (read(solicitud, informacion, otherSize) > 0)
        {
            printf("%s\n",informacion);
            token = strtok(informacion, SEP);

            if (token != NULL)
            {

                for (int i = 0; i < 100; i++)
                {
                    if (strcmp(pipes[i].nombre, token) == 0)
                    {
                        respuesta = pipes[i].fd;
                        break;
                    }
                }
                if (respuesta == -1)
                {
                    crearPipe(token);
                }
                else
                {
                    token = strtok(NULL, SEP);
                    servicio = token[1];
                    switch (servicio)
                    {
                    case INS_DEV:
                        token = strtok(NULL, SEP);
                        //printf("%s\n", token); con espacio al inicio
                        //printf("%s\n", &token[1]); sin espacio al inicio
                        numeritoRaro = strtok(NULL, SEP);

                        sprintf(mensaje, "%s, %s", "Se ha devuelto exitosamente el libro", &token[1]);
                        write(respuesta, mensaje, otherSize);
                        break;

                    case INS_SOL:
                        token = strtok(NULL, SEP);
                        numeritoRaro = strtok(NULL, SEP);
                        sprintf(mensaje, "%s, %s", "Se ha solicitado exitosamente el libro", &token[1]);
                        write(respuesta, mensaje, otherSize);
                        break;

                    case INS_REN:
                        token = strtok(NULL, SEP);
                        numeritoRaro = strtok(NULL, SEP);
                        sprintf(mensaje, "%s, %s", "Se ha renovado exitosamente el libro", &token[1]);
                        write(respuesta, mensaje, otherSize);
                        break;

                    default:
                        strcpy(mensaje, "Servicio solicitado no existe.");
                        write(respuesta, mensaje, otherSize);
                        break;
                    }
                }
            }
        }
        sleep(1);
    }
    
    /*
    printf("%s\n", message);
    agregar(message);
    num++;
    sleep(5);
    close(fd);*/
}

void fin()
{
    close(solicitud);
    close(respuesta);
    unlink(nombrePR);
}

int main(int argc, char *argv[])
{
    int p = -1;
    int f = -1;
    int s = -1;
    char nombreP[otherSize - 5] = "";
    char message[otherSize] = "";

    atexit(fin);

    for (int i = 0; i < argc; i++)
    {

        if ((strcmp(argv[i], "-p")) == 0)
        {
            p = i;
        }
        if ((strcmp(argv[i], "-f")) == 0)
        {
            f = i;
        }
        if ((strcmp(argv[i], "-s")) == 0)
        {
            s = i;
        }
    }

    if (p > 0)
    {
        relleno();
        strcpy(nombrePR,argv[p+1]);
        openPipe(argv[p + 1]);
        manejoInfo();
    }
    else
    {
        printf("El formato de este comando es: ./receptor –p pipeReceptor –f filedatos [ –s filesalida ]");
    }

    //char *respuesta = malloc(150);
    /*
    if (argc > 1)
    {
        for (int i = 1; i < argc; i++)
        {
            agregar(argv[i]);
        }
    }
 */

    return 0;
}
//./receptor –p pipeReceptor –f filedatos [ –s filesalida ]
