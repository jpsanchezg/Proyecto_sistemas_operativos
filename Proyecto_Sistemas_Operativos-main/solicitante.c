/**************************************************************************
*   Sistemas Operativos Proyecto Entrega 2: Prestamo de libros
*   Miembros:
*       - Juan Sebastián Barreto Jimenéz
*       - Janet Chen He
*       - Juan Camilo Merchan Loza
*   File: 
*       solicitante.c
*   Proposito: 
*       Programa que se encarga de los procesos solicitantes.
**************************************************************************/

/* Inclusión de archivos */
#include "pipes.h"

char salir = 0;
char errorArgumentos = 0;
char nombrePipeRespuesta[TAM_STRING - 70];
char nombrePipeReceptor[TAM_STRING - 70];
char nombreArchivoInstrucciones[TAM_STRING - 70];
int pipeReceptor;
int pipeRespuesta;

/**************************************************************************
*   Función:   menu()
*   Proposito:  Rutina para imprimir el menu y leer la selección del
*               usuario.
*   Argumentos:
*       void
*   Retorno:
*       int: Selección realizada por el usuario.
**************************************************************************/
int menu()
{
    int seleccion;
    //limpiarTerminal();
    printf("=========================================================================\n");
    printf("                  Prestamo de libros\n");
    printf("=========================================================================\n");
    printf("               1. Devolver un Libro\n");
    printf("               2. Renovar un Libro\n");
    printf("               3. Solicitar un Libro\n");
    printf("               0. salir\n");
    printf("=========================================================================\n");
    scanf("%d", &seleccion);
    return seleccion;
} // fin menu

/**************************************************************************
*   Función:   leerArchivoIntrucciones()
*   Proposito:  Rutina para leer una línea de las instrucciones
*               de solicitudes desde el archivo.
*   Argumentos:
*       nombreLibro:    Cadena de caracteres donde se almacenara el
*                       nombre del libro.
*       isbnLibro:    Cadena de caracteres donde se almacenara el
*                       ISBN del libro.
*   Retorno:
*       int: Selección de instrucción leída del archivo.
**************************************************************************/
int leerArchivoIntrucciones(char *nombreLibro, char *isbnLibro, int *numeroEjemplar)
{
    static char estado = 0; // 0 - abre archivo 1 - esta leyendo archivo
    int seleccion;
    static FILE *fp;
    char buff[100];
    char *token;
    char contador = 0;
    char *finArchivo;
    char numeroEjemplarCadena[TAM_EJEMPLAR];

    if (!estado)
    {
        fp = fopen(nombreArchivoInstrucciones, "r");
        estado = 1;
    }

    while ((finArchivo = fgets(buff, sizeof(buff), fp)) != NULL && strcmp(buff, "\n") == 0)
        ;

    if (finArchivo != NULL)
    {
        token = strtok(buff, ",");
        if (token[0] == DEVOLVER)
            seleccion = 1;
        else if (token[0] == RENOVAR)
            seleccion = 2;
        else if (token[0] == SOLICITAR)
            seleccion = 3;
        token = strtok(NULL, ",");
        if (token[0] == ' ')
            token = token + 1;
        strcpy(nombreLibro, token);
        token = strtok(NULL, ",");
        while (token[contador] != '\n' && token[contador] != '\0')
            if (token[contador] != '\n')
                contador++;
        token[contador] = '\0';
        if (token[0] == ' ')
            token = token + 1;
        strcpy(isbnLibro, token);
        token = strtok(NULL, ",");
        if (token[0] == ' ')
            token = token + 1;
        strcpy(numeroEjemplarCadena, token);
        *numeroEjemplar = atoi(numeroEjemplarCadena);
    }
    else
    {
        estado = 0;
        fclose(fp);
        return 0;
    }
    return seleccion;
} // fin leerArchivoIntrucciones

/**************************************************************************
*   Función:   generarSolicitud()
*   Proposito:  Rutina para generar la solicitud al receptor
*   Argumentos:
*       nombreLibro:    Cadena de caracteres que tiene el nombre del 
*                       libro.
*       isbnLibro:      Cadena de caracteres que tiene el ISBN del 
*                       libro.
*   Retorno:
*       void
**************************************************************************/
void generarSolicitud(char instruccion, char *nombreLibro, char *isbnLibro, int numeroEjemplar)
{
    char mensaje[TAM_STRING];
    char *nombreInstruccion;
    //limpiarTerminal();
    if(instruccion == 'D'){
        printf("Devolución de libros\n");
    }else if(instruccion == 'P'){
        printf("Solicitud de libros\n");
    }else if(instruccion == 'R'){
        printf("Renovación de libros\n");
    }
    if (nombreLibro[0] == '\0')
    {
        printf("Escriba el nombre del libro\n");
        scanf(" %[^\n]%*c", nombreLibro);
    }
    if (isbnLibro[0] == '\0')
    {
        printf("Escriba el ISBN del libro\n");
        scanf("%s", isbnLibro);
    }
    if(numeroEjemplar == -1){
        printf("Escriba el numero del ejemplar\n");
        scanf("%d", &numeroEjemplar);
    }
    sprintf(mensaje, "%c,%s,%s,%d,%s", instruccion, nombreLibro, isbnLibro, numeroEjemplar, nombrePipeRespuesta);
    write(pipeReceptor, mensaje, TAM_STRING);
    leerPipe(pipeRespuesta, mensaje);
    switch (instruccion)
    {
    case DEVOLVER:
        nombreInstruccion = "devolucion";
        break;

    case SOLICITAR:
        nombreInstruccion = "prestamo";
        break;

    case RENOVAR:
        nombreInstruccion = "renovacion";
        break;

    default:
        break;
    }
    if (mensaje[0] == POSITIVO)
    {
        printf("Se procesado su solicitud");
    }
    else
    {
        printf("Se ha denegado su solicitud");
    }
    printf(" de %s del ejemplar %d del libro %s con isbn %s\n\n", nombreInstruccion, numeroEjemplar, nombreLibro, isbnLibro);
    //sleep(6);
} // fin generarSolicitud

/**************************************************************************
*   Función:   resolverSeleccionMenu()
*   Proposito:  Rutina para generar la solicitud al receptor
*   Argumentos:
*       seleccion:      Selección de la solicitud a realizar sea leido
*                       del usuario o del archivo.
*       nombreLibro:    Cadena de caracteres que tiene el nombre del 
*                       libro.
*       isbnLibro:      Cadena de caracteres que tiene el ISBN del 
*                       libro.
*   Retorno:
*       void
**************************************************************************/
void resolverSeleccionMenu(int seleccion, char *nombreLibro, char *isbnLibro, int numeroEjemplar)
{
    switch (seleccion)
    {
    case 1:
        generarSolicitud(DEVOLVER, nombreLibro, isbnLibro, numeroEjemplar);
        break;

    case 2:
        generarSolicitud(RENOVAR, nombreLibro, isbnLibro, numeroEjemplar);
        break;

    case 3:
        generarSolicitud(SOLICITAR, nombreLibro, isbnLibro, numeroEjemplar);
        break;

    case 0:
        salir = 1;
        break;

    default:
        break;
    }
} // fin resolverSeleccionMenu

/**************************************************************************
*   Función:   crearSecuenciaPipe()
*   Proposito:  Rutina para generar la secuencia para que se puedan
*               varios pipes abiertos desde varios procesos solicitantes.
*   Argumentos:
*       void
*   Retorno:
*       void
**************************************************************************/
void crearSecuenciaPipe()
{
    char secuencia_pipe[TAM_STRING - 50];
    char bandera_acceso;
    int contador = 0;
    do
    {
        strcpy(nombrePipeRespuesta, "PipeSolicitante");
        sprintf(secuencia_pipe, "%d", contador);
        strcat(nombrePipeRespuesta, secuencia_pipe);
        bandera_acceso = access(nombrePipeRespuesta, F_OK);
        contador++;
    } while (bandera_acceso == 0);
} // fin crearSecuenciaPipe

/**************************************************************************
*   Función:   generarSolicitudCierrePipe()
*   Proposito:  Rutina para generar la solicitud de cierre de pipe
*               al receptor.
*   Argumentos:
*       void
*   Retorno:
*       void
**************************************************************************/
void generarSolicitudCierrePipe()
{
    char mensaje[TAM_STRING];
    sprintf(mensaje, "%c,%s", CIERRE_PIPE, nombrePipeRespuesta);
    write(pipeReceptor, mensaje, TAM_STRING);
} // fin generarSolicitudCierrePipe

/**************************************************************************
*   Función:   atenderArgumentosSolicitante()
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
*           1   -   Selección de instrucciones por menu.
*           2   -   Selección de instrucciones por archivo.
**************************************************************************/
unsigned char atenderArgumentosSolicitante(int argc, char *args[])
{
    unsigned char correcto = 0;
    char bandera = 0;
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(args[i], "-p") == 0)
        {
            bandera = 1;
            i++;
            if (i < argc)
            {
                strcpy(nombrePipeReceptor, args[i]);
                if (correcto != 2)
                    correcto = 1;
            }else
            {
                printf("Argumento incompleto: \n");
                printf("Falta el nombre del pipe receptor, solo se puso -p\n");
                printf("./solicitante [-i file] -p pipeReceptor\n");
                correcto = 0;
                break;
            }
        }
        else if (strcmp(args[i], "-i") == 0)
        {
            i++;
            if (i < argc)
            {
                strcpy(nombreArchivoInstrucciones, args[i]);
                correcto = 2;
            }else
            {
                printf("Argumento incompleto: \n");
                printf("Falta el nombre del archivo de lectura, solo se puso -i\n");
                printf("./solicitante [-i file] -p pipeReceptor\n");
                correcto = 0;
                break;
            }
        }else if(args[i][0] == '-'){
            printf("Argumentos incorrectos: \n");
            printf("Opción incorrecta solo es valido usar -i para lectura de archivo y -p para el nombre del receptor\n");
            printf("./solicitante [-i file] -p pipeReceptor\n");
        }
    }
    if(!bandera){
        printf("Argumentos incompletos: \n");
        printf("Falta el nombre del pipe receptor\n");
        printf("./solicitante [-i file] -p pipeReceptor\n");
    }
    return correcto;
} // fin atenderArgumentosSolicitante

/**************************************************************************
*   Función:   manejoPipes()
*   Proposito:  Rutina para el manejo inicial de pipes en el proceso
*               solicitante,
*   Argumentos:
*       void
*   Retorno:
*       void
**************************************************************************/
void manejoPipes()
{
    char mensaje[TAM_STRING];
    pipeReceptor = abrirPipe(nombrePipeReceptor);
    printf("Se abre el pipe r");
    crearSecuenciaPipe();
    sprintf(mensaje, "%c,%s", APERTURA_PIPE, nombrePipeRespuesta);
    write(pipeReceptor, mensaje, TAM_STRING);
    pipeRespuesta = crearPipe(nombrePipeRespuesta);
    do
    {
        leerPipe(pipeRespuesta, mensaje);
        if (mensaje[0] == NEGATIVO)
        {
            printf("Hubo un problema en la comunicación con el receptor");
            write(pipeReceptor, mensaje, TAM_STRING);
            sleep(10);
        }
    } while (mensaje[0] == NEGATIVO);
    limpiarTerminal();
} // fin manejoPipes

/**************************************************************************
*   Función:   salidaSolicitante()
*   Proposito:  Rutina para la salida del proceso solicitante.
*   Argumentos:
*       void
*   Retorno:
*       void
**************************************************************************/
void salidaSolicitante()
{
    if (!errorArgumentos)
    {
        generarSolicitudCierrePipe();
    }
    close(pipeRespuesta);
    close(pipeReceptor);
    unlink(nombrePipeRespuesta);
} // fin salidaSolicitante

int main(int argc, char *args[])
{
    int seleccion;
    char selector_modo_solicitante;
    unsigned char argumentosCorrectos;
    char nombreLibro[TAM_STRING - 70];
    char isbnLibro[TAM_STRING - 90];
    int numeroEjemplar;

    atexit(salidaSolicitante);

    selector_modo_solicitante = atenderArgumentosSolicitante(argc, args);

    if (selector_modo_solicitante)
    {
        manejoPipes();

        while (!salir)
        {
            nombreLibro[0] = '\0';
            isbnLibro[0] = '\0';
            numeroEjemplar = -1;
            if (selector_modo_solicitante == 1)
            {
                seleccion = menu();
            }
            else if (selector_modo_solicitante == 2)
            {
                seleccion = leerArchivoIntrucciones(nombreLibro, isbnLibro, &numeroEjemplar);
            }
            resolverSeleccionMenu(seleccion, nombreLibro, isbnLibro, numeroEjemplar);
        }
    }
    else
    {
        errorArgumentos = 1;
    }
    return 0;
} // fin main