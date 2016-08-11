#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <stdlib.h>

typedef struct PIECE
{
int ini;
    char nom[30];

    char ajus[15];

    int sizes;
    char bandera[15];
    char tip[10];
} PIECE;

typedef struct MBR
{
PIECE sda1;
    PIECE sda2;
    PIECE sda3;
    PIECE sda4;
    char dateCrate [150];
    int sizes;

    int disk_signature;

} MBR;

typedef struct EBR
{
int sizes;
    int siguiente;
    char nom[30];
    char bandera[15];

    int ini;
    char ajus[15];


} EBR;

#define sizeof_mbr sizeof(MBR)
#define sizeof_ebr sizeof(EBR)
#define sizeof_PART sizeof(PIECE)











