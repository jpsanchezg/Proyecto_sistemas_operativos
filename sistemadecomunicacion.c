
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