#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef struct node {
    char* key;
    struct node *left;
    struct node *right;
} node_t;
typedef node_t *ptr_tree;

ptr_tree tree = NULL;

char* ignore;

int k; // lunghezza delle stringhe

int refactor(char);
void aggiungi_albero(char*);
bool check_albero(ptr_tree, char*);
bool stringhe_uguali(char*, char*);
void stampa_inorder(ptr_tree);

void leggi_parole(){
    char* read;

    do {
        read = (char*) malloc(sizeof(char) * (k+1));
        ignore = fgets(read, k+1, stdin);
        while(getchar_unlocked() != '\n');

        if(read[0] != '+') aggiungi_albero(read);
    } while(read[0] != '+');
}

void nuova_partita(){
    char p[k+1], r[k+1], res[k+1]; // r: riferimento, p: parola corrente, res: output
    int n, i, j, refact;
    int count_r[64] = {0}, count_r_tmp[64] = {0};
    bool won = false;
    char x;
    res[k] = '\0';

    ignore = fgets(r, k+1, stdin);

    if(!scanf("%d\n", &n)){
        return;
    }

    for(i = 0; i < k; i++){
        count_r[refactor(r[i])]++;
    }

    for(j = 0; j < n; j++){
        ignore = fgets(p, k+1, stdin);
        while(getchar_unlocked() != '\n');

        if(p[0] == '+' && p[1] == 's') { //+stampa_filtrate
            //TODO
            j--;
        } else if(p[0] == '+' && p[1] == 'i') { //+inserisci_inizio
            leggi_parole();
            j--;
        } else {
            if(stringhe_uguali(p, r)){
                printf("ok\n");

                do{
                    x = getchar_unlocked();
                } while(x != '+' && x != EOF);

                won = true;
                break;
            } else if(check_albero(tree, p)){
                for(i = 0; i < 64; i++){
                    count_r_tmp[i] = count_r[i];
                }

                for(i = 0; i < k; i++){
                    refact = refactor(p[i]);

                    if(p[i] == r[i]){
                        res[i] = '+';
                        count_r_tmp[refact]--;
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
        getchar_unlocked();
    }
}

int main(){
    char x;

    if(scanf("%d\n", &k) > 0){
        leggi_parole();
    }

    nuova_partita();

    x = (char) getchar_unlocked();
    while(x != EOF){
        while(getchar_unlocked() != '\n');

        if(x == 'n'){
            nuova_partita();
        } else if(x == 'i'){
            leggi_parole();
        }

        x = (char) getchar_unlocked();
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

void aggiungi_albero(char* string){
    ptr_tree x = tree, y = NULL, z;

    z = (ptr_tree) malloc(sizeof(node_t));
    z->key = string;
    z->left = NULL;
    z->right = NULL;

    while(x != NULL){
        y = x;
        if(strcmp(z->key, x->key) < 0){
            x = x->left;
        } else {
            x = x->right;
        }
    }

    if(y == NULL){
        tree = z;
    } else if(strcmp(z->key, y->key) < 0) {
        y->left = z;
    } else {
        y->right = z;
    }
}

bool check_albero(ptr_tree x, char* string) {
    if (x == NULL) {
        return false;
    } else {
        int cmp = strcmp(string, x->key);

        if (cmp == 0) {
            return true;
        } else if (cmp < 0) {
            return check_albero(x->left, string);
        } else {
            return check_albero(x->right, string);
        }
    }
}

void stampa_inorder(ptr_tree x){
    if(x != NULL){
        stampa_inorder(x->left);
        printf("%s\n", x->key);
        stampa_inorder(x->right);
    }
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