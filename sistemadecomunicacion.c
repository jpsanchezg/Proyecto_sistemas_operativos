
#include "pipes.h"

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
