
#include "pipes.h"

struct subscripcion
{
    char Noticia *noticias;
};

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