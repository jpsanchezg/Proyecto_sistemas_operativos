/* Inclusión de archivos */
#include "pipes.h"

struct Noticia
{
    char tipo;
    char titulo[TAM_NOTICIA];
};

struct Pipe
{
    int fd;
    char nombrePipe[TAM_STRING];
};

struct subscripcion
{
    struct Noticia *noticias;
};


void publicar(struct Noticia *noticia, struct Pipe *pipe)
{
    write(pipe->fd, noticia, sizeof(struct Noticia));
}

int main(int argc, char *args[])
{
    banderaArchivoSalida = 0;
    if (atenderArgumentosReceptor(argc, args))
    {
        if (leerBaseDeDatos())
        {
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