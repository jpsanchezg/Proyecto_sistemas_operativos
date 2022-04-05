

#include "pipes.h"

char salir = 0;
char errorArgumentos = 0;
char nombrePipeRespuesta[TAM_STRING - 70];
char nombrePipeReceptor[TAM_STRING - 70];
char nombreArchivoInstrucciones[TAM_STRING - 70];
int timeNoticia = 0;
int pipeReceptor;
int pipeRespuesta;

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
}

void leerArchivoPublicador()
{
    FILE *archivo;
    archivo = fopen(nombreArchivoInstrucciones, "r");
    if (archivo == NULL)
    {
        printf("No se pudo abrir el archivo de instrucciones\n");
        exit(1);
    }
    fscanf(archivo, "%s", nombrePipeReceptor);
    fscanf(archivo, "%s", nombrePipeRespuesta);
    fclose(archivo);
}

void generarSolicitudCierrePipe()
{
    char mensaje[TAM_STRING];
    sprintf(mensaje, "%c,%s", CIERRE_PIPE, nombrePipeRespuesta);
    write(pipeReceptor, mensaje, TAM_STRING);
}

unsigned char atenderArgumentosPublicador(argc, args)
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
                {
                    correcto = 1;
                }
            }
            else
            {
                printf("Argumento incompleto: \n");
                printf("Falta el nombre del pipe receptor, solo se puso -p\n");
                printf("./publicador -p pipeReceptor -f [-i file] -t timeN\n");
                correcto = 0;
                break;
            }
            if (strcmp(args[i], "-f") == 0)
            {
                i++;
                if (i < argc)
                {
                    strcpy(nombreArchivoInstrucciones, args[i]);
                    if (correcto != 2)
                    {
                        correcto = 1;
                    }
                }
                else
                {
                    printf("Argumento incompleto: \n");
                    printf("Falta el nombre del archivo de instrucciones, solo se puso -f\n");
                    printf("./publicador -p pipeReceptor -f [-i file] -t timeN\n");
                    correcto = 0;
                    break;
                }
                if(strcmp(args[i], "-t") == 0)
                {
                    i++;
                    if (i < argc)
                    {
                        strcpy(timeNoticia, args[i]);
                        if (correcto != 2)
                        {
                            correcto = 1;
                        }
                    }
                    else
                    {
                        printf("Argumento incompleto: \n");
                        printf("Falta el nombre del archivo de instrucciones, solo se puso -t\n");
                        printf("./publicador -p pipeReceptor -f [-i file] -t timeN\n");
                        correcto = 0;
                        break;
                    }
                }
            }
        }
    }
    if (!bandera)
    {
        printf("Argumentos incompletos: \n");
        printf("Falta el nombre del pipe receptor\n");
        printf("./publicador [-i file] -p pipeReceptor\n");
    }
    return correcto;
} // fin atenderArgumentosPublicador

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
}

void salidaPublicador()
{
    if (!errorArgumentos)
    {
        generarSolicitudCierrePipe();
    }
    close(pipeRespuesta);
    close(pipeReceptor);
    unlink(nombrePipeRespuesta);
}



int main(int argc, char *args[])
{
    int seleccion;
    char selector_modo_solicitante;
    unsigned char argumentosCorrectos;
    char nombreLibro[TAM_STRING - 70];
    char isbnLibro[TAM_STRING - 90];
    int numeroEjemplar;

    atexit(salidaPublicador);

    selector_modo_solicitante = atenderArgumentosPublicador(argc, args);

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