
#include "pipes.h"

struct subscripcion
{
    char Noticia *noticias;
};

struct noticia
{
    char tipo;
    char titulo[TAM_STRING - 70];
};

struct Pipe pipesRespuesta[TAM_PIPES];
char buffer[TAM_BUFFER][TAM_STRING];
int indiceProductor = 0;
int indiceConsumidor = 0;
int pipeReceptor;
char nombrePipeSuscriptor[TAM_STRING];
char nombrePipePublicador[TAM_STRING];
struct noticia *listaNoticia;
sem_t semPublicador;
sem_t semSuscriptor;

void inicializarPipes()
{
    for (int i = 0; i < TAM_PIPES; i++)
    {
        pipesRespuesta[i].fd = -1;
        strcpy(pipesRespuesta[i].nombrePipe, VACIO);
    }
}

int agregarPipe(char nombrePipe[], int fd)
{
    for (int i = 0; i < TAM_PIPES; i++)
    {
        if (strcmp(pipesRespuesta[i].nombrePipe, VACIO) == 0 || access(nombrePipe, F_OK) != 0)
        {
            pipesRespuesta[i].fd = fd;
            strcpy(pipesRespuesta[i].nombrePipe, nombrePipe);
            return 1;
        }
    }
    return 0;
}

void agregarABuffer(char instruccion[TAM_STRING])
{
    strcpy(buffer[indiceProductor], instruccion);
    indiceProductor++;
    indiceProductor %= TAM_BUFFER;
}

void pedirABuffer(char respuesta[TAM_STRING])
{
    strcpy(respuesta, buffer[indiceConsumidor]);
    indiceConsumidor++;
    indiceConsumidor %= TAM_BUFFER;
}

void *atenderBuffer()
{
    char instruccion[TAM_STRING];
    char mensaje[TAM_STRING];
    char tipoInstruccion;
    char *token;
    char nombre[TAM_NOMBRE];
    char nombrePipe[TAM_NOMBRE];
    char numeroEjemplarCadena[TAM_EJEMPLAR];
    char isbn[TAM_ISBN];
    char tipoRespuesta;
    int numeroEjemplar;
    struct Pipe *pipeRespuesta;
    while (1)
    {
        sem_wait(&semConsumidor);
        pedirABuffer(instruccion);
        sem_post(&semProductor);
        token = strtok(instruccion, ",");
        tipoInstruccion = token[0];
        token = strtok(NULL, ",");
        strcpy(nombre, token);
        token = strtok(NULL, ",");
        strcpy(isbn, token);
        token = strtok(NULL, ",");
        strcpy(numeroEjemplarCadena, token);
        numeroEjemplar = atoi(numeroEjemplarCadena);
        token = strtok(NULL, ",");
        strcpy(nombrePipe, token);
        switch (tipoInstruccion)
        {
        case RENOVAR:
            tipoRespuesta = renovarLibro(nombre, isbn, numeroEjemplar);
            break;

        case DEVOLVER:
            tipoRespuesta = devolverLibro(nombre, isbn, numeroEjemplar);
        default:
            break;
        }
        if (tipoRespuesta == POSITIVO)
            escribirBaseDeDatos();
        pipeRespuesta = buscarPipe(nombrePipe);
        sprintf(mensaje, "%c,%s,%s", tipoRespuesta, nombre, isbn);
        write(pipeRespuesta->fd, mensaje, TAM_STRING);
    }
}

struct Pipe *buscarPipe(char nombrePipe[TAM_NOMBRE])
{
    for (int i = 0; i < TAM_PIPES; i++)
    {
        if (strcmp(pipesRespuesta[i].nombrePipe, nombrePipe) == 0)
        {
            return &pipesRespuesta[i];
        }
    }
    return NULL;
}

void cerrarPipe(char nombrePipe[])
{
    struct Pipe *pipe;
    pipe = buscarPipe(nombrePipe);
    close(pipe->fd);
    pipe->fd = -1;
    strcpy(pipe->nombrePipe, VACIO);
}

void recibirnoticia(struct Noticia *noticia, struct Pipe *pipe)
{
    read(pipe->fd, noticia, sizeof(struct Noticia));
}
//

void registrarsuscripcion(struct subscripcion *subscripcion, struct Pipe *pipe)
{
    write(pipe->fd, subscripcion, sizeof(struct subscripcion));
}
//

void imprimirsuscripciones(struct subscripcion *subscripcion, struct Pipe *pipe)
{
    read(pipe->fd, subscripcion, sizeof(struct subscripcion));
}
//

int main(int argc, char *args[])
{
    int seleccion;
    char selector_modo_suscriptor;
    char tipo_noticia;
    char titulo_noticia[TAM_NOTICIA];

    atexit(salidaSuscriptor);

    selector_modo_suscriptor = atenderArgumentosSuscriptor(argc, args);

    if (selector_modo_suscriptor)
    {
        manejoPipes();
    }
}

unsigned char atenderArgumentosSistemacomunicacion(int argc, char *args[])
{
    unsigned char correcto = 0;
    char bandera_p = 0, bandera_f = 0;
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(args[i], "-p") == 0)
        {
            bandera_p = 1;
            i++;
            if (i < argc)
            {
                strcpy(nombrePipePublicador, args[i]);
                correcto = 1;
            }
            else
            {
                printf("Argumento incompleto: \n");
                printf("Falta el nombre del pipe publicador, solo se puso -p\n");
                printf("./sistema -p pipePSC -s pipeSSC [-t timeF]\n");
                correcto = 0;
                break;
            }
        }
        else if (strcmp(args[i], "-s") == 0)
        {
            bandera_f = 1;
            i++;
            if (i < argc)
            {
                strcpy(nombrePipeSuscriptor, args[i]);
                correcto = 1;
            }
            else
            {
                printf("Argumento incompleto: \n");
                printf("Falta el nombre del pipe suscriptor, solo se puso -s\n");
                printf("./sistema -p pipePSC -s pipeSSC [-t timeF]\n");
                correcto = 0;
                break;
            }
        }
        else if(strcmp(args[i], "-t") == 0)
        {
            i++;
            if (i < argc)
            {
                timeF = atoi(args[i]);
                correcto = 1;
            }
            else
            {
                printf("Argumento incompleto: \n");
                printf("Falta el tiempo de espera, solo se puso -t\n");
                printf("./sistema -p pipePSC -s pipeSSC [-t timeF]\n");
                correcto = 0;
                break;
            }
        }
        else
        {
            printf("Argumento incorrecto: \n");
            printf("./sistema -p pipePSC -s pipeSSC [-t timeF]\n");
            correcto = 0;
            break;
        }
    }
    if (!bandera_p)
    {
        printf("Argumentos incompletos: \n");
        printf("Falta el nombre del pipe receptor\n");
        printf("./receptor -p pipeReceptor -f filedatos [-s filesalida]\n");
        correcto = 0;
    }
    if (!bandera_f)
    {
        printf("Argumentos incompletos: \n");
        printf("Falta el nombre del archivo de la BD\n");
        printf("./receptor -p pipeReceptor -f filedatos [-s filesalida]\n");
        correcto = 0;
    }
    return correcto;
}

int main(int argc, char *args[])
{
    banderaArchivoSalida = 0;
    if (atenderArgumentosSistemacomunicacion(argc, args))
    {
        if (leerBaseDeDatos())
        {
            atexit(salidaReceptor);
            printf("=========================================================================\n");
            printf("              Proceso sistema de comunicaciones\n");
            printf("=========================================================================\n");
            printf("               r. Generar reporte\n");
            printf("               s. Salir\n");
            printf("=========================================================================\n");
            pthread_t hiloMenu;
            pthread_create(&hiloMenu, NULL, atenderEntradaConsola, NULL);
            inicializarPipes();
            pipeReceptor = crearPipe(nombrePipeReceptor);
            atenderPipeReceptor();
            pthread_join(hiloMenu, NULL);
        }
    }
    return 0;
} // fin main