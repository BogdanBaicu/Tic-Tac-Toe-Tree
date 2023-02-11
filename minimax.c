#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define DIM 3

// Structuri
typedef struct Ttable{
    char position[DIM][DIM];
}Ttable;

typedef struct Tgame{
    char player;
    int dist;
    Ttable *board;
    struct Tgame *fiu[10];
    int true_or_false;
}Tgame;

// Fucntii table
void afisare_tabla(Ttable *tabla, char *input_file, int nr_of_tabs)
{
    // Functie care scrie in fisier o tabla de joc
    FILE *fw;
    fw=fopen(input_file,"a");

    int i,j,k;
    for(i=0;i<DIM;i++)
        for(j=0;j<DIM;j++)
            {
                if(j==0)
                {
                    for(k=0;k<nr_of_tabs;k++)
                        fputc('\t',fw);
                    fputc(tabla->position[i][j],fw);
                    fputc(' ',fw);
                }
                if(j==1)
                {
                    fputc(tabla->position[i][j],fw);
                    fputc(' ',fw);
                }
                if(j==2)
                {
                    fputc(tabla->position[i][j],fw);
                    fputc('\n',fw);
                }
            }
    fputc('\n',fw);
    fclose(fw);
}

int empty_spaces(Ttable *tabla)
{
    // Functie care afla numarul de mutari disponibile
    int i,j,nr=0;
    for(i=0;i<DIM;i++)
        for(j=0;j<DIM;j++)
            if(tabla->position[i][j]=='-')
                nr++;
    return nr;
}

void copy_table(Ttable *tabla1, Ttable *tabla2)
{
    // Functie care realizeaza copia unei table de joc
    for(int i=0;i<DIM;i++)
        for(int j=0;j<3;j++)
            tabla1->position[i][j]=tabla2->position[i][j];
}

// Functii game
Tgame* init_game()
{
    // Functie de initalizarea a une variabile de tip Tgame
    Tgame *joc=malloc(sizeof(Tgame));
    joc->board=malloc(sizeof(Ttable));
    joc->dist=0;
    for(int i=0;i<10;i++)
        joc->fiu[i]=NULL;
    return joc;
}

void citire_initiala(Tgame *joc, char *input_file)
{
    // Functie care realizeaza citirea datelor initiale
    FILE *fo;
    fo=fopen(input_file,"r");
    
    // Cititre jucator care incepe
    char buff=fgetc(fo);
    joc->player=buff;
    buff=fgetc(fo);

    // Citire tabla initiala
    for(int i=0;i<DIM;i++)
        for(int j=0;j<DIM;j++)
        {
            buff=fgetc(fo);
            joc->board->position[i][j]=buff;
            buff=fgetc(fo);
        }

    fclose(fo);
}

int is_frunza(Tgame *joc)
{
    // Functie care verifica daca un nod este frunza
    for(int i=0;i<10;i++)
        if(joc->fiu[i]!=NULL)
            return 0;
    return 1;
}

void free_game(Tgame *joc)
{
    // Functie de eliberare a memoriei
    if(is_frunza(joc)!=1)
        for(int i=0;i<10;i++)
            if(joc->fiu[i]!=NULL)
                free_game(joc->fiu[i]);
    if(joc!=NULL)
    {
        free(joc->board);
        free(joc);
    }
}

int victory(Ttable *tabla)
{
    // Functie care verifica daca unul dintre jucatori a castigat

    // Verificare pe linii si coloane
    for(int i=0;i<3;i++)
    {
        if(tabla->position[i][0]=='X'&&tabla->position[i][1]=='X'&&tabla->position[i][2]=='X')
            return 1;
        if(tabla->position[i][0]=='O'&&tabla->position[i][1]=='O'&&tabla->position[i][2]=='O')
            return -1;
        if(tabla->position[0][i]=='X'&&tabla->position[1][i]=='X'&&tabla->position[2][i]=='X')
            return 1;
        if(tabla->position[0][i]=='O'&&tabla->position[1][i]=='O'&&tabla->position[2][i]=='O')
            return -1;
    }
    // Verificare pe diagonala principala si secundara
    if(tabla->position[0][0]=='X'&&tabla->position[1][1]=='X'&&tabla->position[2][2]=='X')
        return 1;
    if(tabla->position[0][0]=='O'&&tabla->position[1][1]=='O'&&tabla->position[2][2]=='O')
        return -1;
    if(tabla->position[0][2]=='X'&&tabla->position[1][1]=='X'&&tabla->position[2][0]=='X')
        return 1;
    if(tabla->position[0][2]=='O'&&tabla->position[1][1]=='O'&&tabla->position[2][0]=='O')
        return -1;
    return 0;
}

void add_game(Tgame *joc, Ttable *tabla, int dist, char player)
{
    copy_table(joc->board,tabla);
    joc->dist=dist+1;
    if(player=='X')
        joc->player='O';
    else 
        joc->player='X';
}

// Cerinte
void cerinta1(Tgame *joc, char *fisier)
{
    // Functi ce rezolva prima cerinta

    // Verifica daca unul dintr jucatori a castigat
    if(victory(joc->board)!=0)
    {
        return;
    }
    int i,j,count=0;
    if(empty_spaces(joc->board))
        for(i=0;i<DIM;i++)
            for(j=0;j<DIM;j++)
                if(joc->board->position[i][j]=='-')
                {
                    joc->fiu[count]=init_game();
                    add_game(joc->fiu[count],joc->board,joc->dist,joc->player);
                    joc->fiu[count]->board->position[i][j]=joc->player;
                    afisare_tabla(joc->fiu[count]->board,fisier,joc->fiu[count]->dist);
                    cerinta1(joc->fiu[count],fisier);
                    count++;
                }

}

void make_tree(Tgame *joc)
{
    // Realizeaza arborele de la cerinta 1 pentru a se putea 
    // aplica operatiile asupra lui
    if(victory(joc->board)!=0)
    {
        return;
    }
    int i,j,count=0;
    if(empty_spaces(joc->board))
        for(i=0;i<DIM;i++)
            for(j=0;j<DIM;j++)
                if(joc->board->position[i][j]=='-')
                {
                    joc->fiu[count]=init_game();
                    add_game(joc->fiu[count],joc->board,joc->dist,joc->player);
                    joc->fiu[count]->board->position[i][j]=joc->player;
                    make_tree(joc->fiu[count]);
                    count++;
                }
}

void is_true_or_false(Tgame *joc, char player)
{
    // Pentru fiecare nod din arbore verifica daca este true sau false
    // Daca a castigat cineva, deci este nod terminal
    if(victory(joc->board)==1)
    {
        if(player=='X')
            joc->true_or_false=1;
        else 
            joc->true_or_false=0;
    }
    if(victory(joc->board)==-1)
    {
        if(player=='O')
            joc->true_or_false=1;
        else 
            joc->true_or_false=0;
    }
    // Daca nu a castigat nimeni
    if(victory(joc->board)==0)
    {
        // Daca este nod terminal
        if(empty_spaces(joc->board)==0)
            joc->true_or_false=0;
        // Daca nu este nod terminal, apeleaza recursiv pentru fii nodului
        else
        {
            int i,ok;
            for(i=0;i<10;i++)
                if(joc->fiu[i]!=NULL)
                    is_true_or_false(joc->fiu[i],player);
            // Daca jucatorul care face mutarea acum este acelasi cu primul 
            // jucator care muta
            if(joc->player==player)
            {
                ok=0;
                for(i=0;i<10;i++)
                    if(joc->fiu[i]!=NULL)
                        if(joc->fiu[i]->true_or_false==1)
                            ok=1;
                joc->true_or_false=ok;
            }
            // Daca jucatorul care face mutarea acum nu este acelasi cu primul 
            // jucator care muta
            if(joc->player!=player)
            {
                ok=1;
                for(i=0;i<10;i++)
                    if(joc->fiu[i]!=NULL)
                        if(joc->fiu[i]->true_or_false!=1)
                            ok=0;
                joc->true_or_false=ok;
            }       
        }
    }
}

void afisare_cerinta2(Tgame *joc, FILE *fisier)
{
    // Functie ce realizeaza afisarea corespunzatoare
    // Daca nodul este true
    if(joc->true_or_false==1)
    {
        for(int i=0;i<joc->dist;i++)
            fputc('\t',fisier);
        fputc('T',fisier);
        fputc('\n',fisier);
    }
    // Daca nodul este false
    if(joc->true_or_false==0)
    {
        for(int i=0;i<joc->dist;i++)
            fputc('\t',fisier);
        fputc('F',fisier);
        fputc('\n',fisier);
    }
    // Inaintare recursiva
    for(int j=0;j<10;j++)
        if(joc->fiu[j]!=NULL)
            afisare_cerinta2(joc->fiu[j],fisier);
}

void cerinta2(Tgame *joc, char* fisier)
{
    // Functie ce rezolva taskul 2
    FILE *fw;
    fw=fopen(fisier,"a");
    make_tree(joc);
    is_true_or_false(joc,joc->player);
    afisare_cerinta2(joc,fw);
    fclose(fw);
}

int main(int argc, char **argv)
{   
    // Initializare joc
    Tgame *joc;
    joc=init_game();
    citire_initiala(joc,argv[2]);
    
    if((strcmp(argv[1],"-c1"))==0)
    {
        afisare_tabla(joc->board,argv[3],joc->dist);
        cerinta1(joc,argv[3]);
    }
    if((strcmp(argv[1],"-c2"))==0)
        cerinta2(joc,argv[3]);    
        
    free_game(joc);
    return 0;
}