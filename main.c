#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_LENGTH 1000

typedef struct node {
    char* word;
    struct node *next;
} list_t;
typedef list_t *ptr_list;

typedef struct tree {
    char* word;
    struct tree *l;
    struct tree *r;
} tree_t;
typedef tree_t *ptr_tree;

ptr_list hash_table[TABLE_LENGTH];
ptr_tree tree_dict = NULL;

char* ignore;

int k; // lunghezza delle stringhe

int refactor(char);
int hash(char*);
void aggiungi_parola(char*);
bool check_dizionario(char*);
bool stringhe_uguali(char*, char*);

void leggi_parole(){
    char read[k + 1];

    do {
        ignore = fgets(read, k+1, stdin);
        while(getchar() != '\n');

        aggiungi_parola(read);
    } while(read[0] != '+');
}

void nuova_partita(){
    char p[k+1], r[k+1], res[k+1]; // r: riferimento, p: parola corrente, res: output
    int n, i, j;
    bool won = false;
    res[k] = '\0';

    ignore = fgets(r, k+1, stdin);

    if(!scanf("%d\n", &n)){
        return;
    }

    for(j = 0; j < n; j++){
        ignore = fgets(p, k+1, stdin);
        while(getchar() != '\n');

        if(p[0] == '+' && p[1] == 's') { //+stampa_filtrate
            //TODO
            j--;
        } else if(p[0] == '+' && p[1] == 'i') { //+inserisci_inizio
            leggi_parole();
            j--;
        } else {
            if(stringhe_uguali(p, r)){
                printf("ok\n");

                char x;
                do{
                    x = getchar();
                } while(x != '+' && x != EOF);

                won = true;
                break;
            } else if(check_dizionario(p)){
                for(i = 0; i < k; i++){
                    if(p[i] == r[i]){
                        res[i] = '+';
                    } else {
                        //TODO
                    }
                }

                printf("%s\n", res);
            } else {
                printf("not_exists\n");
                j--;
            }
        }
    }

    if(!won){
        printf("ko\n");
        getchar();
    }
}

int main(){
    char x;

    if(scanf("%d\n", &k) > 0){
        leggi_parole();
    }

    nuova_partita();

    x = (char) getchar();
    while(x != EOF){
        while(getchar() != '\n');

        if(x == 'n'){
            nuova_partita();
        } else if(x == 'i'){
            leggi_parole();
        }

        x = (char) getchar();
    }

    return 0;
}

int refactor(char x){
    if(x >= 48 && x <= 57){ // 0-9
        return x-48;
    } else if(x >= 65 && x <= 90){ // A-Z
        return x - 55;
    } else if(x >= 97 && x <= 122){ // a-z
        return x - 61;
    } else if(x == 45){ // -
        return 62;
    } else if(x == 95){ // _
        return 63;
    }

    return 0;
}

int hash(char* string){
    unsigned int i, ris;

    ris = 1;

    for(i = 0; i < k; i++){
        ris = ris*13 + refactor(string[i]);
    }

    return (ris - k) % TABLE_LENGTH;
}

void aggiungi_parola(char* string){
    int hash_tmp = hash(string);
    ptr_list list_tmp;

    ptr_tree x, y;
    x = tree_dict;
    while(x != NULL){
        y = x;
        if(strcmp(string, x->word) < 0)
            x = x->l;
        else
            x = x->r;
    }

    list_tmp = hash_table[hash_tmp];

    if(list_tmp == NULL){
        hash_table[hash_tmp] = (ptr_list) malloc(sizeof(list_t));
        hash_table[hash_tmp]->word = (char*) malloc((k+1) * sizeof(char));
        hash_table[hash_tmp]->next = NULL;
        strcpy(hash_table[hash_tmp]->word, string);
    } else {
        while(list_tmp->next != NULL){
            list_tmp = list_tmp->next;
        }
        list_tmp->next = (ptr_list) malloc(sizeof (list_t));
        list_tmp->next->word = (char *) malloc((k+1) * sizeof(char));
        list_tmp->next->next = NULL;
        strcpy(list_tmp->next->word, string);
    }
}

bool check_dizionario(char* string){
    int hash_tmp = hash(string);
    ptr_list list_tmp = hash_table[hash_tmp];

    do {
        if(stringhe_uguali(list_tmp->word, string)){
            return true;
        } else {
            list_tmp = list_tmp->next;
        }
    } while(list_tmp != NULL);

    return false;
}

bool stringhe_uguali(char* s1, char* s2){
    bool check = true;

    while((*s1 != '\0' || *s2 != '\0') && check){
        check = *s1 == *s2;

        s1++;
        s2++;
    }

    return check;
}