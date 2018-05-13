#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
13/05/2018

Davide Segullo Ingegneria Informatica Unime

Struttura file su memoria di massa:
Cognome Nome    Email
    - descrizione pref_int    pref_naz    numero
    - descrizione pref_int    pref_naz    numero (n-esimo)

*/

#define N 21
#define P 4

typedef struct num_tel{
    char descrizione[N];
    char pref_int[P];
    char pref_naz[P];
    char numero[8];
    struct num_tel *next;
} num_tel;

typedef struct dati_contatto{
    char cognome[N];
    char nome[N];
    char email[N];
} dati_contatto;

typedef struct contatto{
    dati_contatto dati;
    num_tel *numeri_tel;
    struct contatto *next;
}contatto;

typedef enum{false,true} bool;

bool crea_num(num_tel **num, num_tel *p){
    *num = (num_tel*)malloc(sizeof(num_tel));

    if (num == NULL)
        return false;

    strcpy((*num)->descrizione,p->descrizione);
    strcpy((*num)->numero,p->numero);
    strcpy((*num)->pref_int,p->pref_int);
    strcpy((*num)->pref_naz,p->pref_naz);
    (*num)->next = NULL;

    return true;
}

bool crea_contatto(contatto **num, contatto *p){
    *num = (contatto*)malloc(sizeof(contatto));

    if (num == NULL)
        return false;

    (*num)->dati=p->dati;
    (*num)->numeri_tel = p->numeri_tel;
    (*num)->next = NULL;

    return true;
}

bool inserisci_num(num_tel **num, num_tel *p){

    if (num == NULL){
        crea_num(num,p);
        return false;
    }

    num_tel *q = (num_tel *)malloc(sizeof(num_tel));
    strcpy(q->descrizione,p->descrizione);
    strcpy(q->numero,p->numero);
    strcpy(q->pref_int,p->pref_int);
    strcpy(q->pref_naz,p->pref_naz);
    q->next = *num;
    *num = q;

    return true;
}

bool inserisci(contatto **rubrica, contatto *p){

    if(rubrica == NULL){
        crea_contatto(rubrica,p);
        return false;
    }

    contatto *successive = NULL;
    contatto *current = NULL;
    successive = *rubrica;

    while (successive != NULL && strcmp(successive->dati.cognome,p->dati.cognome) < 0){

        if (strcmp(successive->dati.cognome,p->dati.cognome) == 0)
            if (strcmp(successive->dati.cognome,p->dati.cognome) > 0)
                break;

        current = successive;
        successive = successive->next;
    }

    contatto *q = (contatto*)malloc(sizeof(contatto));
    q->dati=p->dati;
    q->numeri_tel = p->numeri_tel;
    q->next = NULL;

    if (current == NULL){
        q->next = *rubrica;
        *rubrica = q;
        return true;
    }

    if (successive == NULL){
        current->next = q;
        return true;
    }

    q->next = successive;
    current->next = q;

    return true;
}

bool carica(contatto **rubrica, char file_name[]){ //Carica la rubrica da un file
    FILE *file;

    file = fopen("rubrica.txt","r");

    if (file == NULL)
        return false;

    while(!feof(file)){
        contatto *temp = (contatto *)malloc(sizeof(contatto));
        temp->next = NULL;
        temp->numeri_tel = NULL;

        num_tel *num = (num_tel *)malloc(sizeof(num_tel));
        num->next = NULL;

        fscanf(file,"%s\t%s\t%s\n",temp->dati.cognome,temp->dati.nome,temp->dati.email);

        while(fscanf(file,"\t- %s\t%s\t%s\t%s\n",num->descrizione,num->pref_int,num->pref_naz,num->numero)>0){
            inserisci_num(&(temp->numeri_tel),num);
        }

        inserisci(rubrica, temp);
    }

    fclose(file);
    return true;
}

bool salva(contatto *rubrica, char file_name[]){  //Salva la lista rubrica in un file
    FILE *file;

    file = fopen(file_name,"w");

    if (file == NULL)
        return false;

    for(contatto *p=rubrica; p!=NULL; p=p->next){
        fprintf(file,"%s\t%s\t%s\n",p->dati.cognome,p->dati.nome,p->dati.email);
        for(num_tel *q=p->numeri_tel; q!=NULL; q=q->next)
            fprintf(file,"\t- %s\t%s\t%s\t%s\n",q->descrizione,q->pref_int,q->pref_naz,q->numero);
    }

    fclose(file);
    return true;
}

bool cerca(contatto *rubrica){

    if (rubrica == NULL)
        return false;

    char cognome[21];

    printf("Cerca\nInserire il cognome da cercare: ");
    scanf("%s",cognome);

    for (contatto *p=rubrica; p!=NULL; p=p->next){
        if(strcmp(p->dati.cognome,cognome)==0){
            printf("%s\t%s\t%s\n",p->dati.cognome,p->dati.nome,p->dati.email);
            for (num_tel *q=p->numeri_tel; q!=NULL; q=q->next)
                printf("%s\t%s\t%s\t%s\n",q->descrizione,q->numero,q->pref_int,q->pref_naz);
        }
    }

    return true;
}

void inserimento_input(contatto **rubrica){
    contatto *p = (contatto *)malloc(sizeof(contatto));
    p->next = NULL;
    p->numeri_tel = NULL;

    int n_num = 0;

    printf("Inserimento di un nuovo contatto\nCognome: ");
    scanf("%s",p->dati.cognome);
    printf("Nome: ");
    scanf("%s",p->dati.nome);
    printf("E-Mail: ");
    scanf("%s",p->dati.email);

    printf("Quanti numeri vuoi inserire?\n");
    scanf("%d",&n_num);

    for (int i=0; i<n_num; i++){
        num_tel *num = (num_tel *)malloc(sizeof(num_tel));
        num->next = NULL;

        printf("Inserimento Numero:\nDescrizione: ");
        scanf("%s",num->descrizione);
        printf("Prefisso internazionale: ");
        scanf("%s",num->pref_int);
        printf("Prefisso nazionale: ");
        scanf("%s",num->pref_naz);
        printf("Numero: ");
        scanf("%s",num->numero);

        inserisci_num(&(p->numeri_tel),num);
    }

    inserisci(rubrica, p);
}

int main()
{
    contatto *rubrica = NULL;
    int scelta=0;

    do {
        printf("0)Cerca\n1)Inserisci\n2)Salve Rubrica\n3)Carica Rubrica\n4)Cerca In Rubrica\nScelta: ");
        scanf("%d",&scelta);

        switch(scelta){
        case 0:
            cerca(rubrica);
            break;
        case 1:
            inserimento_input(&rubrica);
            break;
        case 2:
            salva(rubrica,"rubrica.txt");
            break;
        case 3:
            carica(&rubrica,"rubrica.txt");
            break;
        }
    } while (scelta!=5);

    return 0;
}
