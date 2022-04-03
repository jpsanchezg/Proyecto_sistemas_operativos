#include "pipes.h"

char salir = 0;
char errorArgumentos = 0;
char nombrePipeRespuesta[TAM_STRING - 70];
char nombrePipeReceptor[TAM_STRING - 70];
char nombreArchivoInstrucciones[TAM_STRING - 70];
int pipeReceptor;
int pipeRespuesta;
struct subscripcion
{
    char Noticia *noticias;
};

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

void suscribirseANuevaNoticia()
{
    printf("Ingrese el tipo de noticia: \n");
    printf("1. Noticia de prensa\n");
    printf("2. Noticia de internet\n");
    scanf("%c", &tipo_noticia);
    printf("Ingrese el titulo de la noticia: \n");
    scanf("%s", titulo_noticia);
    generarSolicitudSuscripcion(tipo_noticia, titulo_noticia);
}

void generarSolicitudSuscripcion(char tipo_noticia, char titulo_noticia[])
{
    struct Noticia noticia;
    noticia.tipo = tipo_noticia;
    strcpy(noticia.titulo, titulo_noticia);
    write(pipeReceptor, &noticia, sizeof(struct Noticia));
}
