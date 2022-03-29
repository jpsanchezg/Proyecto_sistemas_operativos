/**************************************************************************
*   Sistemas Operativos Proyecto Entrega 2: Prestamo de libros
*   Miembros:
*       - Juan Sebastián Barreto Jimenéz
*       - Janet Chen He
*       - Juan Camilo Merchan Loza
*   File: 
*       receptor.c
*   Proposito: 
*       Programa que se encarga del receptor de peticiones.
**************************************************************************/

/* Inclusión de archivos */
#include "pipes.h"

/* Definición de constantes */
#define TAM_BUFFER 10

const char VACIO[20] = "\0";

/* Definición de estructuras */
struct Pipe
{
    int fd;
    char nombrePipe[TAM_STRING];
};

struct Ejemplar
{
    int id;
    char estado;
    struct tm fecha;
};

struct Libro
{
    char nombreLibro[TAM_NOMBRE];
    char isbn[TAM_ISBN];
    int numEjemplares;
    struct Ejemplar *ejemplares;
    struct Libro *siguiente;
};

/* Definición de variables globales */
struct Pipe pipesRespuesta[TAM_PIPES];
char buffer[TAM_BUFFER][TAM_STRING];
int indiceProductor = 0;
int indiceConsumidor = 0;
int pipeReceptor;
char nombrePipeReceptor[TAM_STRING];
char nombreArchivoDatos[TAM_STRING];
char nombreArchivoSalida[TAM_STRING];
char banderaArchivoSalida;
char banderaEspaciosBD;
struct Libro *listaLibros;
sem_t semProductor;
sem_t semConsumidor;

/**************************************************************************
*   Función:   inicializarPipes()
*   Proposito:  Rutina para inicializar la lista de pipes para el
*               manejo de varios procesos solicitantes.
*   Argumentos:
*       void
*   Retorno:
*       void
**************************************************************************/
void inicializarPipes()
{
    for (int i = 0; i < TAM_PIPES; i++)
    {
        pipesRespuesta[i].fd = -1;
        strcpy(pipesRespuesta[i].nombrePipe, VACIO);
    }
} // fin inicializarPipes

/**************************************************************************
*   Función:   agregarPipe()
*   Proposito:  Rutina para agregar un pipe con el nombre y el descriptor
*                que son pasados por parámetro en la lista de pipes.
*   Argumentos:
*       nombrePipe: Cadena de caracteres con el nombre del pipe a cerrar.
*       fd:         Descriptor del pipe.
*   Retorno:
*       int: Estado de confirmación si se pudo agregar el pipe o no.
*           0   -   No se pudo agregar.
*           1   -   Se pudo agregar.
**************************************************************************/
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
} // fin agregarPipe

/**************************************************************************
*   Función:   buscarPipe()
*   Proposito:  Rutina para buscar un pipe con el nombre que es pasado
*               por parámetro.
*   Argumentos:
*       nombrePipe: Cadena de caracteres con el nombre del pipe a cerrar.
*   Retorno:
*       Pipe: Puntero de una estructura Pipe con el pipe que se buscaba.
**************************************************************************/
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
} // fin buscarPipe

/**************************************************************************
*   Función:   cerrarPipe()
*   Proposito:  Rutina para cerrar el pipe con el nombre que es pasado
*               por parámetro, que se encuentra en la lista de los
*               pipes.
*   Argumentos:
*       nombrePipe: Cadena de caracteres con el nombre del pipe a cerrar.
*   Retorno:
*       void
**************************************************************************/
void cerrarPipe(char nombrePipe[])
{
    struct Pipe *pipe;
    pipe = buscarPipe(nombrePipe);
    close(pipe->fd);
    pipe->fd = -1;
    strcpy(pipe->nombrePipe, VACIO);
} // fin cerrarPipe

/**************************************************************************
*   Función:   devolverLibro()
*   Proposito:  Rutina para devolver el libro, verificando con la BD.
*   Argumentos:
*       nombre: Cadena de caracteres con el nombre del libro a solicitar.
*       isbn: Cadena de caracteres con el isbn del libro a solicitar.
*       numeroEjemplar: Número del ejemplar a solicitar.
*   Retorno:
*       char:   Se devuelve respuesta POSITIVA o NEGATIVA al pipe,
*               dependiendo de estado de la solicitud.
**************************************************************************/
char devolverLibro(char nombre[TAM_NOMBRE], char isbn[TAM_ISBN], int numeroEjemplar)
{
    struct Libro *actual;
    actual = listaLibros;
    char tipoRespuesta = NEGATIVO;
    time_t unixTime;
    struct tm *fecha;
    while (actual != NULL)
    {
        if (strcmp(actual->nombreLibro, nombre) == 0 && strcmp(actual->isbn, isbn) == 0)
        {
            if(numeroEjemplar <= actual->numEjemplares)
            {
                if (actual->ejemplares[numeroEjemplar-1].estado == PRESTADO)
                {
                    tipoRespuesta = POSITIVO;
                    actual->ejemplares[numeroEjemplar-1].estado = DISPONIBLE;
                    unixTime = time(0);
                    fecha = localtime(&unixTime);
                    actual->ejemplares[numeroEjemplar-1].fecha = *fecha;
                    actual = NULL;
                    break;
                }
            }else
            {
                tipoRespuesta = NEGATIVO;
            }
            break;
        }
        else
        {
            actual = actual->siguiente;
        }
    }
    return tipoRespuesta;
} // fin devolverLibro

/**************************************************************************
*   Función:   renovarLibro()
*   Proposito:  Rutina para renovar el libro, verificando con la BD.
*   Argumentos:
*       nombre: Cadena de caracteres con el nombre del libro a solicitar.
*       isbn: Cadena de caracteres con el isbn del libro a solicitar.
*       numeroEjemplar: Número del ejemplar a solicitar.
*   Retorno:
*       char:   Se devuelve respuesta POSITIVA o NEGATIVA al pipe,
*               dependiendo de estado de la solicitud.
**************************************************************************/
char renovarLibro(char nombre[TAM_NOMBRE], char isbn[TAM_ISBN], int numeroEjemplar)
{
    struct Libro *actual;
    actual = listaLibros;
    char tipoRespuesta = NEGATIVO;
    time_t unixTime;
    struct tm *fecha;
    while (actual != NULL)
    {
        if (strcmp(actual->nombreLibro, nombre) == 0 && strcmp(actual->isbn, isbn) == 0)
        {
            if(numeroEjemplar <= actual->numEjemplares)
            {
                if (actual->ejemplares[numeroEjemplar-1].estado == PRESTADO)
                {
                    tipoRespuesta = POSITIVO;
                    unixTime = time(0);
                    unixTime += 86400 * 7;
                    fecha = localtime(&unixTime);
                    actual->ejemplares[numeroEjemplar-1].fecha = *fecha;
                    actual = NULL;
                }
            }else
            {
                tipoRespuesta = NEGATIVO;
            }
            break;
        }
        else
        {
            actual = actual->siguiente;
        }
    }
    return tipoRespuesta;
} // fin renovarLibro

/**************************************************************************
*   Función:   solicitarLibro()
*   Proposito:  Rutina para solicitar el libro, verificando con la BD.
*   Argumentos:
*       nombre: Cadena de caracteres con el nombre del libro a solicitar.
*       isbn: Cadena de caracteres con el isbn del libro a solicitar.
*       numeroEjemplar: Número del ejemplar a solicitar.
*   Retorno:
*       char:   Se devuelve respuesta POSITIVA o NEGATIVA al pipe,
*               dependiendo de estado de la solicitud.
**************************************************************************/
char solicitarLibro(char nombre[TAM_NOMBRE], char isbn[TAM_ISBN], int numeroEjemplar)
{
    struct Libro *actual;
    actual = listaLibros;
    char tipoRespuesta = NEGATIVO;
    time_t unixTime;
    struct tm *fecha;
    while (actual != NULL)
    {
        if (strcmp(actual->nombreLibro, nombre) == 0 && strcmp(actual->isbn, isbn) == 0)
        {
            if(numeroEjemplar <= actual->numEjemplares)
            {
                if (actual->ejemplares[numeroEjemplar-1].estado == DISPONIBLE)
                {
                    tipoRespuesta = POSITIVO;
                    actual->ejemplares[numeroEjemplar-1].estado = PRESTADO;
                    unixTime = time(0);
                    unixTime += 86400 * 7;
                    fecha = localtime(&unixTime);
                    actual->ejemplares[numeroEjemplar-1].fecha = *fecha;
                    actual = NULL;
                }
            }else
            {
                tipoRespuesta = NEGATIVO;
            }
            break;
        }
        else
        {
            actual = actual->siguiente;
        }
    }
    return tipoRespuesta;
} // fin solicitarLibro

/**************************************************************************
*   Función:   escribirBaseDeDatos()
*   Proposito:  Rutina para escribir la base de datos con el nombre
*               pasado por parametro en la compilación al receptor.
*   Argumentos:
*       void
*   Retorno:
*       void
**************************************************************************/
void escribirBaseDeDatos()
{
    FILE *fp;
    fp = fopen(nombreArchivoDatos, "w");
    struct Libro *actual;
    actual = listaLibros;
    char *number[5];
    while (actual != NULL)
    {
        fprintf(fp, "%s, %s, %d\n", actual->nombreLibro, actual->isbn, actual->numEjemplares);
        for (int i = 0; i < actual->numEjemplares; i++)
        {
            if(banderaEspaciosBD){
                fprintf(fp, "%d, %c, ", actual->ejemplares[i].id, actual->ejemplares[i].estado);
                fprintf(fp, "%d-%d-%d\n", actual->ejemplares[i].fecha.tm_mday, actual->ejemplares[i].fecha.tm_mon + 1, actual->ejemplares[i].fecha.tm_year + 1900);
            }else{
                fprintf(fp, "%d,%c,", actual->ejemplares[i].id, actual->ejemplares[i].estado);
                fprintf(fp, "%d-%d-%d\n", actual->ejemplares[i].fecha.tm_mday, actual->ejemplares[i].fecha.tm_mon + 1, actual->ejemplares[i].fecha.tm_year + 1900);
            }
        }
        actual = actual->siguiente;
    }
    fclose(fp);
} // fin escribirBaseDeDatos

/**************************************************************************
*   Función:   leerBaseDeDatos()
*   Proposito:  Rutina para leer la base de datos con el nombre
*               pasado por parametro en la compilación al receptor.
*   Argumentos:
*       void
*   Retorno:
*       int: Bandera si se pudo abrir el archivo o no.
**************************************************************************/
int leerBaseDeDatos()
{
    struct Libro **newLibro = NULL;
    struct Ejemplar newEjemplar;
    FILE *fp;
    char buff[100];
    char *token;
    char *tokenFecha;
    char nombreArchivoDatosReducido[TAM_STRING];
    newLibro = &listaLibros;
    banderaEspaciosBD = 0;
    fp = fopen(nombreArchivoDatos, "r");
    if(fp){
        while (fgets(buff, sizeof(buff), fp ))
        {
            if (strcmp(buff, "") != 0 && strcmp(buff, "\n") != 0)
            {
                *newLibro = malloc(sizeof(struct Libro));
                (*newLibro)->siguiente = NULL;
                token = strtok(buff, DELIM);
                strcpy((*newLibro)->nombreLibro, token);
                token = strtok(NULL, DELIM);
                if (token[0] == ' '){
                    banderaEspaciosBD = 1;
                    token = token + 1;
                }
                strcpy((*newLibro)->isbn, token);
                token = strtok(NULL, DELIM);
                if (token[0] == ' ')
                    token = token + 1;
                (*newLibro)->numEjemplares = atoi(token);
                (*newLibro)->ejemplares = malloc(sizeof(struct Ejemplar) * (*newLibro)->numEjemplares);
                for (int i = 0; i < (*newLibro)->numEjemplares; i++)
                {
                    fgets(buff, sizeof(buff), fp);
                    memset(&newEjemplar, 0, sizeof(newEjemplar));
                    token = strtok(buff, DELIM);
                    newEjemplar.id = atoi(token);
                    token = strtok(NULL, DELIM);
                    if (token[0] == ' ')
                        newEjemplar.estado = token[1];
                    else
                        newEjemplar.estado = token[0];
                    token = strtok(NULL, DELIM);
                    if (token[0] == ' ')
                        token++;
                    tokenFecha = strtok(token, "-");
                    newEjemplar.fecha.tm_mday = atoi(tokenFecha);
                    tokenFecha = strtok(NULL, "-");
                    newEjemplar.fecha.tm_mon = atoi(tokenFecha) - 1;
                    tokenFecha = strtok(NULL, "-");
                    newEjemplar.fecha.tm_year = atoi(tokenFecha) - 1900;
                    mktime(&(newEjemplar.fecha));
                    (*newLibro)->ejemplares[i] = newEjemplar;
                }
                newLibro = &((*newLibro)->siguiente);
            }
        }
        fclose(fp);
    }
    else
    {
        strcpy(nombreArchivoDatosReducido, nombreArchivoDatos+2);
        printf("El archivo con nombre de la base de datos %s, pasada con -f no fue posible abrirlo, posiblemente se deba a que no existe\n", nombreArchivoDatosReducido);
        return 0;
    }
    return 1;
} // fin  leerBaseDeDatos

/**************************************************************************
*   Función:   agregarABuffer()
*   Proposito:  Rutina para agregar al buffer una instrucción que se
*               recibe por el pipe receptor.
*   Argumentos:
*       instruccion: Instrucción que es recibida desde el pipe receptor.
*   Retorno:
*       void
**************************************************************************/
void agregarABuffer(char instruccion[TAM_STRING])
{
    strcpy(buffer[indiceProductor], instruccion);
    indiceProductor++;
    indiceProductor %= TAM_BUFFER;
} // fin agregarABuffer

/**************************************************************************
*   Función:   pedirABuffer()
*   Proposito:  Rutina para solicitar la instrucción que se encuentra
*               en la cabeza del buffer.
*   Argumentos:
*       void
*   Retorno:
*       void
**************************************************************************/
void pedirABuffer(char respuesta[TAM_STRING])
{
    strcpy(respuesta, buffer[indiceConsumidor]);
    indiceConsumidor++;
    indiceConsumidor %= TAM_BUFFER;
} // fin pedirABuffer

/**************************************************************************
*   Función:   atenderBuffer()
*   Proposito:  Rutina para atender el buffer.
*   Argumentos:
*       void
*   Retorno:
*       void * 
*       Nota:   Se retorna un puntero de void dado que así se necesita
*               para poder usar la función en un hilo.
**************************************************************************/
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
        if(tipoRespuesta == POSITIVO)
            escribirBaseDeDatos();
        pipeRespuesta = buscarPipe(nombrePipe);
        sprintf(mensaje, "%c,%s,%s", tipoRespuesta, nombre, isbn);
        write(pipeRespuesta->fd, mensaje, TAM_STRING);
    }
} // fin atenderBuffer

/**************************************************************************
*   Función:   atenderPipeReceptor()
*   Proposito:  Rutina para atender el pipe receptor que recibe los
*               mensajes del proceso solicitante.
*   Argumentos:
*       void
*   Retorno:
*       void
**************************************************************************/
void atenderPipeReceptor()
{
    char mensaje[TAM_STRING];
    char copiaMensaje[TAM_STRING];
    char *token;
    char *token2;
    char instruccion;
    int fd;
    struct Pipe *pipeRespuesta;
    char nombre[TAM_NOMBRE];
    char nombrePipe[TAM_NOMBRE];
    char numeroEjemplarCadena[TAM_EJEMPLAR];
    char isbn[TAM_ISBN];
    char tipoRespuesta;
    int numeroEjemplar;
    pthread_t hiloBuffer;

    sem_init(&semProductor, 0, TAM_BUFFER);
    sem_init(&semConsumidor, 0, 0);
    pthread_create(&hiloBuffer, NULL, atenderBuffer, NULL);

    while (1)
    {
        if (read(pipeReceptor, mensaje, TAM_STRING) > 0)
        {
            strcpy(copiaMensaje, mensaje);
            //printf("%s\n", mensaje);
            token = strtok(mensaje, ",");
            if (token != NULL)
            {
                instruccion = token[0];
                switch (instruccion)
                {
                case APERTURA_PIPE:
                    token = strtok(NULL, ",");
                    fd = abrirPipe(token);
                    if (agregarPipe(token, fd))
                    {
                        sprintf(mensaje, "%c", POSITIVO); // Confirmacion de apertura del pipe de respuesta
                        printf("=========================================================================\n");
                        printf("              Menu Receptor\n");
                        printf("=========================================================================\n");
                        printf("               r. Generar reporte\n");
                        printf("               s. Salir\n");
                        printf("=========================================================================\n");
                    }
                    else
                    {
                        sprintf(mensaje, "%c,%s", NEGATIVO, "401"); // Ya existen demasiados pipes
                    }
                    write(fd, mensaje, TAM_STRING);
                    break;

                case CIERRE_PIPE:
                    token = strtok(NULL, ",");
                    cerrarPipe(token);
                    break;

                case DEVOLVER:
                    sem_wait(&semProductor);
                    agregarABuffer(copiaMensaje);
                    sem_post(&semConsumidor);
                    break;

                case RENOVAR:
                    sem_wait(&semProductor);
                    agregarABuffer(copiaMensaje);
                    sem_post(&semConsumidor);
                    break;

                case SOLICITAR:
                    token = strtok(NULL, ",");
                    strcpy(nombre, token);
                    token = strtok(NULL, ",");
                    strcpy(isbn, token);
                    token = strtok(NULL, ",");
                    strcpy(numeroEjemplarCadena, token);
                    numeroEjemplar = atoi(numeroEjemplarCadena);
                    token = strtok(NULL, ",");
                    strcpy(nombrePipe, token);
                    tipoRespuesta = solicitarLibro(nombre, isbn, numeroEjemplar);
                    if(tipoRespuesta == POSITIVO)
                        escribirBaseDeDatos();
                    sprintf(mensaje, "%c,%s,%s", tipoRespuesta, nombre, isbn);
                    pipeRespuesta = buscarPipe(nombrePipe);
                    write(pipeRespuesta->fd, mensaje, TAM_STRING);
                    break;

                default:
                    break;
                }
            }
        }
    }
    pthread_join(hiloBuffer, NULL);
} // fin atenderPipeReceptor

/**************************************************************************
*   Función:   atenderArgumentosReceptor()
*   Proposito:  Rutina para leer los argumentos pasados por el usuario
*               en el momento de ejecución.
*   Argumentos:
*       argc:   Cantidad de argumentos pasados por el usuario en el 
*               momento de ejecución.
*       args:   Vector de argumentos pasados por el usuario en el 
*               momento de ejecución.
*   Retorno:
*       unsigned char: Número que define el estado de los argumentos.
*           0   -   Argumentos invalidos.
*           1   -   Argumentos validos.
**************************************************************************/
unsigned char atenderArgumentosReceptor(int argc, char *args[])
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
                strcpy(nombrePipeReceptor, args[i]);
                correcto = 1;
            }else
            {
                printf("Argumento incompleto: \n");
                printf("Falta el nombre del pipe receptor, solo se puso -p\n");
                printf("./receptor -p pipeReceptor -f filedatos [-s filesalida]\n");
                correcto = 0;
                break;
            }
        }
        else if (strcmp(args[i], "-f") == 0)
        {
            bandera_f = 1;
            i++;
            if (i < argc)
            { 
                nombreArchivoDatos[0] = '.';
                nombreArchivoDatos[1] = '/';
                strcat(nombreArchivoDatos, args[i]);
                correcto = 1;
            }else
            {
                printf("Argumento incompleto: \n");
                printf("Falta el nombre del nombre del archivo de la BD, solo se puso -f\n");
                printf("./receptor -p pipeReceptor -f filedatos [-s filesalida]\n");
                correcto = 0;
                break;
            }
        }
        else if (strcmp(args[i], "-s") == 0)
        {
            banderaArchivoSalida = 1;
            i++;
            if (i < argc)
            {
                strcpy(nombreArchivoSalida, args[i]);
                correcto = 1;
            }else
            {
                printf("Argumento incompleto: \n");
                printf("Falta el nombre del nombre del archivo de salida, solo se puso -s\n");
                printf("./receptor -p pipeReceptor -f filedatos [-s filesalida]\n");
                correcto = 0;
                break;
            }
        }
    }
    if(!bandera_p){
        printf("Argumentos incompletos: \n");
        printf("Falta el nombre del pipe receptor\n");
        printf("./receptor -p pipeReceptor -f filedatos [-s filesalida]\n");
        correcto = 0;
    }
    if(!bandera_f){
        printf("Argumentos incompletos: \n");
        printf("Falta el nombre del archivo de la BD\n");
        printf("./receptor -p pipeReceptor -f filedatos [-s filesalida]\n");
        correcto = 0;
    }
    return correcto;
} // fin atenderArgumentosReceptor

/**************************************************************************
*   Función:   borrarLibros()
*   Proposito:  Rutina para borrar los libros cargados en el receptor
*               de la BD.
*   Argumentos:
*       void
*   Retorno:
*       void
**************************************************************************/
void borrarLibros()
{
    struct Libro *actual;
    struct Libro *siguiente;
    actual = listaLibros;
    siguiente = NULL;
    while (actual != NULL)
    {
        siguiente = actual->siguiente;
        free(actual->ejemplares);
        free(actual);
        actual = siguiente;
    }
} // fin borrarLibros

/**************************************************************************
*   Función:   imprimirReporte()
*   Proposito:  Rutina para imprimir el reporte desde el hilo
*               auxiliar 2.
*   Argumentos:
*       void
*   Retorno:
*       void
**************************************************************************/
void imprimirReporte()
{
    struct Libro *actual;
    actual = listaLibros;
    limpiarTerminal();
    while (actual != NULL)
    {
        for (int i = 0; i < actual->numEjemplares; i++)
        {
            printf("%c, %s, %s, %d, ", actual->ejemplares[i].estado, actual->nombreLibro, actual->isbn, actual->ejemplares[i].id);
            printf("%d-%d-%d\n", actual->ejemplares[i].fecha.tm_mday, actual->ejemplares[i].fecha.tm_mon + 1, actual->ejemplares[i].fecha.tm_year + 1900);
        }
        actual = actual->siguiente;
    }
} // fin imprimirReporte

/**************************************************************************
*   Función:   atenderEntradaConsola()
*   Proposito:  Rutina para atender entrada por consola desde el hilo
*               auxiliar 2. R - Generar Reporte. S - Salir Receptor.
*   Argumentos:
*       void
*   Retorno:
*       void*: Puntero para uso desde un hilo.
**************************************************************************/
void *atenderEntradaConsola()
{
    int fd;
    char commando;
    while (1)
    {
        scanf("%c", &commando);
        switch (commando)
        {
        case 's':
            exit(0);
            break;

        case 'r':
            imprimirReporte();
        default:
            break;
        }
    }
} // fin atenderEntradaConsola

/**************************************************************************
*   Función:   escribirArchivoSalida()
*   Proposito:  
*   Argumentos:
*       void
*   Retorno:
*       void
**************************************************************************/
void escribirArchivoSalida(){
    FILE *fp;
    int contDisponibles = 0;
    fp = fopen(nombreArchivoSalida, "w");
    struct Libro *actual;
    actual = listaLibros;
    char *number[5];
    while (actual != NULL)
    {
        for (int i = 0; i < actual->numEjemplares; i++)
            if(actual->ejemplares[i].estado == DISPONIBLE)
                contDisponibles++;
        fprintf(fp, "Nombre: %s, ISBN: %s, Ejemplares: %d Ejemplares Disponibles: %d\n", actual->nombreLibro, actual->isbn, actual->numEjemplares, contDisponibles);
        contDisponibles = 0;
        for (int i = 0; i < actual->numEjemplares; i++)
        {
            fprintf(fp, "Ejemplar: %d, Estado: %c, ", actual->ejemplares[i].id, actual->ejemplares[i].estado);
            fprintf(fp, " Fecha: %d-%d-%d\n", actual->ejemplares[i].fecha.tm_mday, actual->ejemplares[i].fecha.tm_mon + 1, actual->ejemplares[i].fecha.tm_year + 1900);
        }
        actual = actual->siguiente;
    }
    fclose(fp);
} // end escribirArchivoSalida

/**************************************************************************
*   Función:   salidaReceptor()
*   Proposito:  Rutina para la salida del proceso receptor.
*   Argumentos:
*       void
*   Retorno:
*       void
**************************************************************************/
void salidaReceptor()
{
    if(banderaArchivoSalida)
        escribirArchivoSalida();
    escribirBaseDeDatos();
    borrarLibros();
    for (int i = 0; i < TAM_PIPES; i++)
    {
        if (pipesRespuesta[i].fd != -1)
        {
            close(pipesRespuesta[i].fd);
        }
    }
    close(pipeReceptor);
    unlink(nombrePipeReceptor);
} // fin salidaReceptor

int main(int argc, char *args[])
{
    banderaArchivoSalida = 0;
    if (atenderArgumentosReceptor(argc, args))
    {
        if(leerBaseDeDatos()){
            atexit(salidaReceptor);
            printf("=========================================================================\n");
            printf("              Proceso receptor\n");
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