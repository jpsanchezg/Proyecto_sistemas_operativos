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

int main(int arg, char *args[])
{
    
}

void publicar(struct Noticia *noticia, struct Pipe *pipe)
{
    write(pipe->fd, noticia, sizeof(struct Noticia));
}
