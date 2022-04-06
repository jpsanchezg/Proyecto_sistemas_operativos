#include "pipes.h"

char salir = 0;
char errorArgumentos = 0;
struct News publication[TAM_STRING];
int countNews = 0;
char nombrePipeRespuesta[TAM_STRING - 70];
char nombrePipeReceptor[TAM_STRING - 70];
char nombreArchivoInstrucciones[TAM_STRING - 70];
int timeNoticia = 0;
int pipeReceptor;
int pipeRespuesta;

void readArchivePublisher(char *nomArchivo)
{
    char cadena[100];
    FILE *stream;
    stream = fopen(nomArchivo, "r");

    if (stream == NULL)
    {
        perror("No se pudo abrir el archivo de noticias.");
        exit(1);
    }

    while (!feof(stream))
    {
        fgets(cadena, sizeof cadena, stream);
        char *tk;
        struct News newNews;
        tk = strtok(cadena, ",");
        printf("Categoria de la noticia: %s \n", tk);
        strcpy(newNews.category, tk);
        tk = strtok(NULL, ",");
        printf("Titulo de la noticia: %s \n", tk);
        strcpy(newNews.title, tk);
        publication[countNews] = newNews;
        countNews++;
    }
    fclose(stream);
}

/**************************************************************************
 *   Función:   readArguments(int argc, char *args[])
 *   Proposito: Rutina para leer los argumentos pasados por el usuario
 *               en el momento de ejecución.
 *   Argumentos:
 *        argc:   Cantidad de argumentos pasados por el usuario en el
 *               momento de ejecución.
 *       args:   Vector de argumentos pasados por el usuario en el
 *               momento de ejecución.
 *   Retorno:
 *       structura llena con los argumentos puestos
 **************************************************************************/
struct ArgPublisher readArguments(int argc, char *args[])
{
    struct ArgPublisher temp;
    if (argc != 7)
    {
        errno = 8;
        perror("ERROR: Cantidad equivocada de argumentos");
    }
    else
    {
        for (int i = 0; i < argc; i++)
        {
            if (strcmp(args[i], "-p") == 0)
            {
                strcpy(temp.pipePSC, args[i + 1]);
            }
            if (strcmp(args[i], "-p") != 0)
            {
            }

            if (strcmp(args[i], "-f") == 0)
            {
                strcpy(temp.file, args[i + 1]);
            }

            if (strcmp(args[i], "-t") == 0)
            {
                char *tp;

                temp.timeN = strtod(args[i + 1], &tp);
            }
        }
        return temp;
    }
} // fin atenderArgumentosPublicador

int main(int argc, char *args[])
{
    struct ArgPublisher entryArguments;
    entryArguments = readArguments(argc, args);
    readArchivePublisher(entryArguments.file);
    sendPipe(entryArguments.pipePSC, countNews, publication);
    return 0;
} // fin main