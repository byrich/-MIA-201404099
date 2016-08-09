#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char Size[100];
int Tamanio;
char Unit[5]=" ";
char Path[100];
char Name[100];
char Type[5]=" ";
char Fit[5]=" ";
char Eliminar[50]=" ";
char Add[100];
char Id[50];

void Reconocercomando();

void Ejecutarcomando(char *Listacomandos, char *Comando);

void Reconocercomando(){
    char Var1[100];
    char Var2[100];
    char Var3[100];

    do{
        printf("Byrich: ~$ ");
        scanf(" %[^\n]s", Var1);
        if(Var1[strlen(Var1)-1]=='\\'){
            scanf(" %[^\n]s", Var2);
            strncpy(Var3, Var1, strlen(Var1)-1);
            strcat(Var3, Var2);
            strcpy(Var1, Var3);
            char *varIns = strtok(Var3, " ");
            Ejecutarcomando(Var1,varIns);
        }else if(Var1[0]=='#'){
            printf("Comentario: %s\n",Var1);
        }else{
            strcpy(Var3, Var1);
            char *varIns = strtok(Var3, " ");
            Ejecutarcomando(Var1,varIns);
        }
    }while(strcasecmp(Var1,"Salir")!=0);
}


void Ejecutarcomando(char *Listacomandos, char *Comando){
    if(strcasecmp(Comando,"mkdisk")==0){
        char *token=strtok(Listacomandos," ");
        while((token = strtok(NULL,"::"))!=NULL){
        printf("%s\n",token);
            if(strcasecmp(token,"-name")==0){
                token = strtok(NULL, " ");
                if (token[0] == ':'){
                memmove(token, token+1, strlen(token));
                }
                strcpy(Name,token);
            }else if(strcasecmp(token,"-size")==0){
                token = strtok(NULL, " ");
                if (token[0] == ':'){
                memmove(token, token+1, strlen(token));
                }
                strcpy(Size,token);
            }else if(strcasecmp(token,"-path")==0){
                token = strtok(NULL, " ");
                if (token[0] == ':'){
                memmove(token, token+1, strlen(token));
                }
               strcpy(Path,token);
            }else if(strcasecmp(token,"+unit")==0){
                      token = strtok(NULL, " ");
                if (token[0] == ':'){
                memmove(token, token+1, strlen(token));
                }
                strcpy(Unit,token);
            }
        }
    }else if(strcasecmp(Comando,"rmdisk")==0){
        char *token=strtok(Listacomandos," ");
        while((token = strtok(NULL,"::"))!=NULL){
            if(strcasecmp(token,"-path")==0){
                token = strtok(NULL, " ");
                if (token[0] == ':'){
                memmove(token, token+1, strlen(token));
                }
                strcpy(Path,token);
            }
        }
    }else if(strcasecmp(Comando,"fdisk")==0){
         char *token=strtok(Listacomandos," ");
        while((token = strtok(NULL,"::"))!=NULL){
            if(strcasecmp(token,"-name")==0){
                token = strtok(NULL, " ");
                if (token[0] == ':'){
                memmove(token, token+1, strlen(token));
                }
                strcpy(Name,token);
            }else if(strcasecmp(token,"-size")==0){
                token = strtok(NULL, " ");
                if (token[0] == ':'){
                memmove(token, token+1, strlen(token));
                }
                strcpy(Size,token);
            }else if(strcasecmp(token,"+unit")==0){
                      token = strtok(NULL, " ");
                if (token[0] == ':'){
                memmove(token, token+1, strlen(token));
                }
                strcpy(Unit,token);
            }else if(strcasecmp(token,"-path")==0){
                token = strtok(NULL, " ");
                if (token[0] == ':'){
                memmove(token, token+1, strlen(token));
                }
                strcpy(Path,token);
            }else if(strcasecmp(token,"+type")==0){
                          token = strtok(NULL, " ");
                if (token[0] == ':'){
                memmove(token, token+1, strlen(token));
                }
                strcpy(Type,token);
            }else if(strcasecmp(token,"+fit")==0){
                token = strtok(NULL, " ");
                if (token[0] == ':'){
                memmove(token, token+1, strlen(token));
                }
                strcpy(Fit,token);
            }else if(strcasecmp(token,"+delete")==0){
                token = strtok(NULL, " ");
                if (token[0] == ':'){
                memmove(token, token+1, strlen(token));
                }
                strcpy(Eliminar,token);
            }else if(strcasecmp(token,"+add")==0){
                token = strtok(NULL, " ");
                if (token[0] == ':'){
                memmove(token, token+1, strlen(token));
                }
                strcpy(Add,token);
            }
        }
    }else if(strcasecmp(Comando,"mount")==0){
         char *token=strtok(Listacomandos," ");
        while((token = strtok(NULL,"::"))!=NULL){
            if(strcasecmp(token,"-path")==0){
                token = strtok(NULL, " ");
                if (token[0] == ':'){
                memmove(token, token+1, strlen(token));
                }
                strcpy(Path,token);
            }else if(strcasecmp(token,"-name")==0){
                token = strtok(NULL, " ");
                if (token[0] == ':'){
                memmove(token, token+1, strlen(token));
                }
                strcpy(Name,token);
            }
        }
    }else if(strcasecmp(Comando,"umount")==0){
        char idtoken[]="-ide";
        int conta=1;
        char *token=strtok(Listacomandos," ");
        while((token = strtok(NULL,"::"))!=NULL){
            idtoken[3] = conta + '0';
            if(strcasecmp(token,idtoken)==0){
                token = strtok(NULL, " ");
                if (token[0] == ':'){
                memmove(token, token+1, strlen(token));
                }
                strcpy(Id,token);
                printf("primer id%s\n",idtoken);
            }
            conta++;
        }
    }else if(strcasecmp(Comando ,"exec")==0){
        char *token=strtok(Listacomandos," ");
        while((token = strtok(NULL,"::"))!=NULL){
            if(strcasecmp(token,"-path")==0){
                   token = strtok(NULL, " ");
                if (token[0] == ':'){
                memmove(token, token+1, strlen(token));
                }
               strcpy(Path,token);
            }
        }
    }else if(strcasecmp(Comando ,"rep")==0){
        char *token=strtok(Listacomandos," ");
        while((token = strtok(NULL,"::"))!=NULL){
            if(strcasecmp(token,"-name")==0){
                   token = strtok(NULL, " ");
                if (token[0] == ':'){
                memmove(token, token+1, strlen(token));
                }
                strcpy(Name,token);
            }else if(strcasecmp(token,"-path")==0){
                  token = strtok(NULL, " ");
                if (token[0] == ':'){
                    memmove(token, token+1, strlen(token));
                }
               strcpy(Path,token);
            }else if(strcasecmp(token,"-id")==0){
                   token = strtok(NULL, " ");
                if (token[0] == ':'){
                memmove(token, token+1, strlen(token));
                }
              strcpy(Id,token);
            }

        }
    }
}
