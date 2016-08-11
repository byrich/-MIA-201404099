#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <stdlib.h>
#include "estructuras.h"


int mkdisk(int size, char unit, char *path, char *nom, int boolean)
{

    int i=0;
    int to_print=1;
    int sizes;
    char ceros[1024];
    char fileCommand[255];
    char fullpath [200];
    sprintf(fullpath,"%s%s", path, nom);

    //codigo para crear la carpeta si no existe
    struct aux;
    struct stat st = {0};
    if (stat(path, &st) == -1){
        printf("La carpeta destino no existe, se proseguira a crear una nueva\n");
        char *command="mkdir \"";
        char *comando = (char *) malloc(1 + strlen(command)+ strlen(path));
        strcpy(comando, command);
        strcat(comando, path);
        strcat(comando, "\"");
        printf("%s\n", comando);
        system(comando);
    }
    //codigo para crear el archivo con el tamaño indicado
    if(unit=='m' || unit=='\0'){
        sizes = size*1024*1024;
    }else if(unit=='k'){
        sizes = size*1024;
    }else{
        printf("Error: La unidad incorrecta \n");
        return 1;
    }
    //se verifica el tamaño del disco a crear
    if(sizes<10*1024*1024){
        printf("Error: El disco deber ser mayor o igual a 10MB\n");
        return 1;
    }
    //se verifica si el archivo ya existe
    if (stat(fullpath, &st) != -1){
        printf("Error: archivo previamente creado\n");
        return 1;
    }
    //momento en el que realmente se crea el archivo conforme al comando
    FILE *disco = fopen(fullpath,"wb+");
    for(i=0; i<(sizes/1024); i++)
    {
        fwrite(&ceros,1024,1,disco);
    }
    fclose(disco);
    //Para almacenar el MBR al ini del archivo
    MBR mbr;
    //parte para el tamaño
    mbr.sizes=sizes;
    //parte para la fecha
    time_t tiempo = time(0);
    struct tm *tlocal = localtime(&tiempo);
    char output[150];
    strftime(output,150,"%d/%m/%y %H:%M:%S",tlocal);
    strcpy(mbr.dateCrate,output);
    //parte para el numero identificador aleatorio
    srand (time(NULL));
    int numero = rand();
    //todos los atributos que se asignaran al crearse a los atributos de tip particion
    mbr.disk_signature=numero;
    mbr.sda1.ini=0;
    mbr.sda1.sizes=0;
    strcpy(mbr.sda1.bandera,"i");
    strcpy(mbr.sda1.nom,"");
    strcpy(mbr.sda1.ajus,"");
    mbr.sda2.ini=0;
    mbr.sda2.sizes=0;
    strcpy(mbr.sda2.bandera,"i");
    strcpy(mbr.sda2.nom,"");
    strcpy(mbr.sda2.ajus,"");
    mbr.sda3.ini=0;
    mbr.sda3.sizes=0;
    strcpy(mbr.sda3.nom,"");
    strcpy(mbr.sda3.bandera,"i");
    strcpy(mbr.sda3.ajus,"");
    mbr.sda4.ini=0;
    mbr.sda4.sizes=0;
    strcpy(mbr.sda4.nom,"");
    strcpy(mbr.sda4.bandera,"i");
    strcpy(mbr.sda4.ajus,"");
    //momento en el que se quiere almacenar el MBR del disco creado
    FILE *archivo;
    archivo = fopen(fullpath, "rb+");
    fseek(archivo, 0, SEEK_SET);
    fwrite(&mbr, sizeof_mbr, 1, archivo);
    fclose(archivo);
    printf("tamaño del mbr: %d\n",mbr.sizes);
}

int rmdisk(int boolean,char *path)
{
    struct stat st = {0};
    if (stat(path, &st) == -1)
    {
        printf("Error: Disco no encontrado\n");
        return 1;
    }
    remove(path);

}

int fdisk_agregar(int boolean,int size, char unit, char *path, char tip[10], char fit[12], char *nom)
{
    //Se va a verificar el tip que se quiere almacenar
    if(strcmp(tip,"P")!=0 && strcmp(tip,"E")!=0 && strcmp(tip,"L")!=0){
        printf("El tipo de particion es incorrecto\n");
        return 1;
    }
    //se va a verificar el ajus de la particion

    if(strcmp(fit,"BF")!=0 && strcmp(fit,"FF")!=0 && strcmp(fit,"WF")!=0){
        printf("El tipo de la particion es incorrecto\n");
        return 1;
    }

    //Se va a verificar si es que la ruta ingresada es correcta y existe el disco
    struct stat st = {0};
    if (stat(path, &st) == -1)
    {
        printf("Error: Disco no encontrado\n");
        return 1;
    }
    //se obtiene el sizes
    int sizes;
    if(unit=='B')
    {
        sizes=size;
    }
    else if(unit=='K' || unit=='/0')
    {
        sizes=size*1024;
    }
    else if(unit=='M')
    {
        sizes=size*1024*1024;
    }
    else
    {
        printf("Error: tipo de unidad incorrecta\n");
        return 1;
    }

    //se verificara que el sizes sea mayor de 2MB
    if(sizes<(2*1024*1024)){
        printf("Error: Tamaño de particion debe ser mayor o igual a 2MB\n");
        return 1;
    }

    MBR mbr;
    FILE *archivo;
    archivo = fopen(path, "rb+");
    fseek(archivo, 0, SEEK_SET);
    fread(&mbr, sizeof_mbr, 1, archivo);


    if(tip=="L")
    {
        int tamanioParticion=0;
        int inicioParticion=0;
        int indiceEbr=0;
        if(strcmp(mbr.sda1.tip,"E")==0)
        {
            indiceEbr=mbr.sda1.ini;
            tamanioParticion=mbr.sda1.sizes;
            inicioParticion=mbr.sda1.ini;
        }

        else if(strcmp(mbr.sda2.tip,"E")==0)
        {
            indiceEbr=mbr.sda2.ini;
            tamanioParticion=mbr.sda2.sizes;
            inicioParticion=mbr.sda2.ini;
        }
        else if(strcmp(mbr.sda3.tip,"E")==0)
        {
            indiceEbr=mbr.sda3.ini;
            tamanioParticion=mbr.sda3.sizes;
            inicioParticion=mbr.sda3.ini;
        }
        else if(strcmp(mbr.sda4.tip,"E")==0)
        {
            indiceEbr=mbr.sda4.ini;
            tamanioParticion=mbr.sda4.sizes;
            inicioParticion=mbr.sda4.ini;
        }
        if(indiceEbr==0)
        {
            printf("Error: No se encontro la particion extendida a la que asignar la particion logica\n");
            return 1;
        }
        EBR ebr;
        int u=0;
        while(u==0)
        {

            fseek(archivo, indiceEbr, SEEK_SET);
            fread(&ebr, sizeof_ebr, 1, archivo);

            //se va a verificar si existe un espacio entre los EBR al que se le pueda insertar el EBR nuevo
            if((indiceEbr+ebr.sizes+sizeof_ebr)!=ebr.siguiente && ebr.siguiente!=0)
            {
                if((ebr.siguiente-(indiceEbr+ebr.sizes+sizeof_ebr))>=sizes)
                {
                    printf("Espacio libre entre las particiones logicas para insertar una nueva\n");
                    //primero se va a crear un nuevo EBR al cual se le asigna sus datos y ademas su siguiente que es el mismo que el temporal, luego a este se le escribe en el documento y al igual que el temporal con el atributo siguiente apuntando al nuevo creado
                    EBR nuevo;
                    strcpy(nuevo.ajus,fit);
                    strcpy(nuevo.nom,nom);
                    strcpy(nuevo.bandera, "a");
                    nuevo.sizes=sizes;
                    nuevo.siguiente=ebr.siguiente;
                    nuevo.ini=(indiceEbr+ebr.sizes+sizeof_ebr);
                    fseek(archivo, nuevo.ini, SEEK_SET);
                    fwrite(&nuevo, sizeof_ebr, 1, archivo);
                    ebr.siguiente=nuevo.ini;
                    fseek(archivo, ebr.ini, SEEK_SET);
                    fwrite(&ebr, sizeof_ebr, 1, archivo);
                    fclose(archivo);
                    return 1;
                }
            }

            if(strcmp(ebr.bandera,"i")==0 || ebr.sizes==0)
            {
                ebr.ini=indiceEbr;
                //se verificara si se dispone de espacio suficiente para almacenar la particion logica
                if((indiceEbr+sizes)>(inicioParticion+tamanioParticion))
                {
                    fclose(archivo);
                    printf("Error: Espacio insuficiente en la particion Extendida del disco\n");
                    return 1;
                }

                printf("Se almacenara la nueva particion logica a partir de la posiscion: %d\n", ebr.ini);
                strcpy(ebr.ajus,fit);
                strcpy(ebr.nom,nom);
                strcpy(ebr.bandera, "a");
                ebr.sizes=sizes;
                fseek(archivo, indiceEbr, SEEK_SET);
                fwrite(&ebr, sizeof_ebr, 1, archivo);
                fclose(archivo);
                return 1;
            }
            if(ebr.siguiente==0)
            {

                EBR nueva;
                nueva.ini=ebr.ini+sizeof_ebr;
                nueva.siguiente=0;
                nueva.sizes=0;
                strcpy(mbr.sda1.bandera,"i");
                fseek(archivo, indiceEbr, SEEK_SET);

                ebr.siguiente=indiceEbr+ebr.sizes+sizeof_ebr;
                //se va a verificar que la particion extendida tenga suficiente espacio para almacenar el EBR
                if(ebr.siguiente>(indiceEbr+tamanioParticion))
                {
                    printf("Error: Espacio restante es insuficiente\n");
                    fclose(archivo);
                    return 1;
                }
                fwrite(&ebr, sizeof_ebr, 1, archivo);
                fseek(archivo, indiceEbr+ebr.sizes+sizeof_ebr, SEEK_SET);
                fwrite(&nueva, sizeof_ebr, 1, archivo);
            }
            indiceEbr=ebr.siguiente;
        }

        fclose(archivo);

        return 1;
    }


    int y = espaciolibre1(mbr);

    printf("Espacio libre entre: %d y %d\n",y, nextPosition(mbr, y));

    int espacio=nextPosition(mbr, y)-y;


    if(espacio>sizes)
    {
        printf("Espacio suficiente para el almacenamiento de la particion\n");
        PIECE nueva;
        strcpy(nueva.ajus,fit);
        nueva.ini=y;
        strcpy(nueva.nom,nom);
        strcpy(nueva.bandera, "a");
        nueva.sizes=sizes;
        strcpy(nueva.tip,tip);

        if(mbr.sda1.sizes==0)
        {
            mbr.sda1=nueva;
        }
        else if(mbr.sda2.sizes==0)
        {
            mbr.sda2=nueva;
        }
        else if(mbr.sda3.sizes==0)
        {
            mbr.sda3=nueva;
        }
        else if(mbr.sda4.sizes==0)
        {
            mbr.sda4=nueva;
        }
        else
        {
            printf("Error: Maximo numero de particiones: 4\n");
            return 1;
        }

        FILE *archivo;
        archivo = fopen(path, "rb+");
        fseek(archivo, 0, SEEK_SET);
        fwrite(&mbr, sizeof_mbr, 1, archivo);
        if(tip=="E")
        {
            EBR ebr;
            ebr.ini=y;
            ebr.siguiente=0;
            ebr.sizes=0;
            strcpy(ebr.bandera, "i");
            fseek(archivo, y, SEEK_SET);
            fwrite(&ebr, sizeof_ebr, 1, archivo);
        }
        fclose(archivo);

    }
    else
    {
        printf("Espacio suficiente para el almacenamiento de la particion\n");
        return 1;
    }
}


int espaciolibre1(struct MBR a,int boolean, int tam)
{

    int answer=a.sizes;
    //si existe un espacio vacio antes de cualquier particion:
    if(sizeof_mbr!=a.sda1.ini && sizeof_mbr!=a.sda2.ini && sizeof_mbr!=a.sda3.ini && sizeof_mbr!=a.sda4.ini)
    {
        return sizeof_mbr;
    }
    //si existe un espacio vacio despues de la primera particion
    int aux=(a.sda1.ini+a.sda1.sizes);
    if(aux!=a.sda2.ini && aux!=a.sda3.ini && aux!=a.sda4.ini && (answer>(a.sda1.ini+a.sda1.sizes)))
    {

        answer=a.sda1.ini+a.sda1.sizes;
    }
    //si existe un espacio vacio despues de la segunda particion
    aux=(a.sda2.ini+a.sda2.sizes);
    if(aux!=a.sda3.ini && aux!=a.sda4.ini && (answer>(a.sda2.ini+a.sda2.sizes)))
    {
        answer=a.sda2.ini+a.sda2.sizes;
    }
    //si existe un espacio vacio despues de la tercera particion
    if((a.sda3.ini+a.sda3.sizes)!=a.sda4.ini && (answer>(a.sda3.ini+a.sda3.sizes)))
    {
        answer=a.sda3.ini+a.sda3.sizes;
    }
    //seccion que verifica si es que el espacio vacio viene hasta al final de todas las particiones
    int v=a.sda1.ini+a.sda1.sizes;
    if((a.sda2.ini+a.sda2.sizes)>v)
    {
        v=a.sda2.ini+a.sda2.sizes;
    }
    if((a.sda3.ini+a.sda3.sizes)>v)
    {
        v=a.sda3.ini+a.sda3.sizes;
    }
    if((a.sda4.ini+a.sda4.sizes)>v)
    {
        v=a.sda4.ini+a.sda4.sizes;
    }
    if(v<answer)
    {
        answer=v;
    }
    //seccion que verifica si es que es posible rescribir una particion eliminada
    if(a.sda1.sizes!=0 && strcmp(a.sda1.bandera, "i")==0 && answer>a.sda1.ini)
    {
        answer=a.sda1.ini;
    }
    if(a.sda2.sizes!=0 && strcmp(a.sda2.bandera, "i")==0 && answer>a.sda2.ini)
    {
        answer=a.sda2.ini;
    }
    if(a.sda3.sizes!=0 && strcmp(a.sda3.bandera, "i")==0 && answer>a.sda3.ini)
    {
        answer=a.sda3.ini;
    }
    if(a.sda4.sizes!=0 && strcmp(a.sda4.bandera, "i")==0 && answer>a.sda4.ini)
    {
        answer=a.sda4.ini;
    }

    return answer;
}

int nextPosition(int boolean,struct MBR mbr, int indexs)
{
    int answer=mbr.sizes;
    if(mbr.sda1.sizes!=0 && mbr.sda1.ini>indexs && mbr.sda1.ini<answer)
    {
        answer=mbr.sda1.ini;
    }
    if(mbr.sda2.sizes!=0 && mbr.sda2.ini>indexs && mbr.sda2.ini<answer)
    {
        answer=mbr.sda2.ini;
    }
    if(mbr.sda3.sizes!=0 && mbr.sda3.ini>indexs && mbr.sda3.ini<answer)
    {
        answer=mbr.sda3.ini;
    }
    if(mbr.sda4.sizes!=0 && mbr.sda4.ini>indexs && mbr.sda4.ini<answer)
    {
        answer=mbr.sda4.ini;
    }
    return answer;
}

