#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

#define otherSize 100

int respuesta;
int envio;
char nombrePR[otherSize];

void openPipe(char *nombre)
{
    int fd;

    unlink(nombre);
    mkfifo(nombre, 0);
    chmod(nombre, 460);
    fd = open(nombre, O_RDONLY);
    respuesta = fd;
    sleep(1);
}

void writer(char *nombre)
{
    int fd;
    do
    {
        fd = open(nombre, O_WRONLY | O_NONBLOCK);
        if (fd == -1)
        {
            printf("\e[1;1H\e[2J");
            printf("Abriendo pipe.\n");
            printf("\e[1;1H\e[2J");
            sleep(1);
            printf("Abriendo pipe..\n");
            printf("\e[1;1H\e[2J");
            sleep(1);
            printf("Abriendo pipe...\n");
            printf("\e[1;1H\e[2J");
            sleep(1);
        }

    } while (fd == -1);
    envio = fd;
}

void lector()
{
    char message[otherSize];

    if (read(respuesta, message, otherSize) > 0)
    {
        printf("%s\n", message);
    }
}

void devolver_libro()
{
    char nombreLibro[otherSize - 50];
    char mensaje[otherSize];

    printf("Escriba el nombre del libro a devolver\n$");
    scanf("%s", nombreLibro);
    sprintf(mensaje, "%s, %c, %s", nombrePR, 'D', nombreLibro);
    write(envio, mensaje, otherSize);
    lector();
}

void solicitar_libro()
{
    char nombreLibro[otherSize - 50];
    char mensaje[otherSize];

    printf("Escriba el nombre del libro que desea solicitar\n$");
    scanf("%s", nombreLibro);
    sprintf(mensaje, "%s, %c, %s", nombrePR, 'P', nombreLibro);
    write(envio, mensaje, otherSize);
    lector();
}

void renovar_libro()
{
    char nombreLibro[otherSize - 50];
    char mensaje[otherSize];

    printf("Escriba el nombre del libro a renovar\n$");
    scanf("%s", nombreLibro);
    sprintf(mensaje, "%s, %c, %s", nombrePR,'R', nombreLibro);
    write(envio, mensaje, otherSize);
    lector();
}

void crearNombreRespuesta(char *nombreIni){
    int val;
    int counter=0;

   do{
        sprintf(nombrePR,"%s%d",nombreIni,counter);
        val=access(nombrePR, F_OK);
        counter++;
    } while (val==0);
}

int menu()
{
    int op;
    printf("Que desea hacer? \n1)Devolver un libro \n2)Solicitar un libro \n3)Renovar un libro (proxiamente)  \n0)Salir\n$");
    scanf("%d", &op);
    return (op);
}

void fin()
{
    close(envio);
    close(respuesta);
    unlink(nombrePR);
}

int main(int argc, char *argv[])
{
    int opcion = 1;
    int p = -1;
    int f = -1;
    char nombreP[otherSize - 50];
    char message[otherSize];
    //if (strcmp(argv[1], '-i'))

    atexit(fin);

    for (int i = 0; i < argc; i++)
    {

        if ((strcmp(argv[i], "-p")) == 0)
        {
            p = i;
        }
        if ((strcmp(argv[i], "-i")) == 0)
        {
            f = i;
        }
    }

    if (p > 0)
    {
        
        writer(argv[p + 1]);
        crearNombreRespuesta(argv[p+1]);       
        write(envio,nombrePR,otherSize);
        openPipe(nombrePR);
        lector();

        if (f > 0)
        {
            FILE *file = fopen(argv[f + 1], "r");
            if (file == NULL)
            {
                printf("Error opening file");
                exit(1);
            }
            char line[otherSize];
            while (fgets(line, sizeof(line), file) != NULL)
            {  
                char mensajeFinal[otherSize];
                sprintf(mensajeFinal, "%s, %s", nombrePR, line);
                write(envio, mensajeFinal, otherSize);
                lector();
            }
        }
        else
        {
            do
            {
                opcion = menu();
                switch (opcion)
                {
                case 1:
                    devolver_libro();
                    break;
                case 2:
                    solicitar_libro();
                    break;
                case 3:
                    printf("Proximamente\n");
                    break;
                case 0:
                    printf("Gracias por usar nuestro servicio\n");
                    break;
                default:
                    printf("Opcion no valida\n");
                    break;
                }

            } while (opcion != 0);
        }
    }
    else
    {
        printf("El formato para usar este programa es $solicitante [–i file] –p nombrePipe\n");
        return 0;
    }

    return 0;
}