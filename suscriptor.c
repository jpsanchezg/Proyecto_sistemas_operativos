#include "pipes.h"

char salir = 0;
char errorArgumentos = 0;
char nombrePipeRespuesta[TAM_STRING - 70];
char nombrePipeReceptor[TAM_STRING - 70];
char nombreArchivoInstrucciones[TAM_STRING - 70];
int pipeReceptor;
int pipeRespuesta;

int menu()
{
    int seleccion;
    // limpiarTerminal();
    printf("=========================================================================\n");
    printf("                  Suscripcion de noticias\n");
    printf("=========================================================================\n");
    printf("               1. Suscribirse a una nueva noticia\n");
    printf("               2. Mostrar noticias\n");
    printf("               0. salir\n");
    printf("=========================================================================\n");
    scanf("%d", &seleccion);
    return seleccion;
}
void salidaSuscriptor()
{
    if (!errorArgumentos)
    {
        generarSolicitudCierrePipe();
    }
    close(pipeRespuesta);
    close(pipeReceptor);
    unlink(nombrePipeRespuesta);
}

unsigned char atenderArgumentosSuscriptor(int argc, char *args[])
{
    unsigned char correcto = 0;
    char bandera = 0;
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(args[i], "-s") == 0)
        {
            bandera = 1;
            i++;
            if (i < argc)
            {
                strcpy(nombrePipeReceptor, args[i]);
                if (correcto != 2)
                    correcto = 1;
            }
            else
            {
                printf("Argumento incompleto: \n");
                printf("Falta el nombre del pipe receptor, solo se puso -s\n");
                printf("./suscriptor -s pipeReceptor\n");
                correcto = 0;
                break;
            }
        }
        else if (args[i][0] == '-')
        {
            printf("Argumentos incorrectos: \n");
            printf("Opción incorrecta solo es valido usar -s para el nombre del receptor\n");
            printf("./suscriptor -s pipeReceptor\n");
        }
    }
    if (!bandera)
    {
        printf("Argumentos incompletos: \n");
        printf("Falta el nombre del pipe receptor\n");
        printf("./suscriptor -s pipeReceptor\n");
    }
    return correcto;
} // fin atenderArgumentosSolicitante

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
void mostrarnoticias()
{
    int fd;
    int tam;
    char buffer[TAM_NOTICIA];
    fd = open(nombrePipeReceptor, O_RDONLY);
    if (fd == -1)
    {
        printf("No se pudo abrir el pipe %s\n", nombrePipeReceptor);
        exit(1);
    }
    while (1)
    {
        tam = read(fd, buffer, TAM_NOTICIA);
        if (tam == 0)
        {
            printf("No hay noticias\n");
            break;
        }
        else
        {
            printf("%s\n", buffer);
        }
    }
}

void suscribirseANuevaNoticia()
{
    printf("Ingrese el tipo de noticia: \n");
    printf("A. Noticia de Arte\n");
    printf("E. Noticia de Farandula y espectaculos\n");
    printf("C. Noticia de ciencia\n");
    printf("P. Noticia de politica\n");
    printf("S. Noticia de sucesos\n");
    scanf("%c", &tipo_noticia);
    generarSolicitudSuscripcion(tipo_noticia );
}

void generarSolicitudSuscripcion(char tipo_noticia)
{
    struct subscripcion subscripcion;
    subscripcion.tipo_noticia = tipo_noticia;
    write(pipeReceptor, &subscripcion, sizeof(struct subscripcion));
}
