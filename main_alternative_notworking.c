#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//#define HASH_LENGTH 1000

// Definizioni strutture dati
typedef struct tree_node {
    char* key;
    struct tree_node *left;
    struct tree_node *right;
} tree_node_t;
typedef tree_node_t *ptr_tree;

typedef struct list_node {
    char* key;
    struct list_node *next;
} list_node_t;
typedef list_node_t *ptr_list;

enum word_status {
    exact_position,
    different_position,
    not_exists,
    none
};

// Definizioni variabili globali
int k;
char* ignore;
ptr_tree tree = NULL;
//ptr_list hash_table[HASH_LENGTH] = {[0 ... HASH_LENGTH-1] = NULL}

// Definizioni funzioni ausiliarie
bool tree_search(ptr_tree, char*);
void tree_insert(char*);
bool stringhe_uguali(char*, char*);
int refactor(char);
char unrefactor(int);

void leggi_parole(){
    char* read;

    do {
        read = (char*) malloc(sizeof(char) * (k+1));
        ignore = fgets(read, k+1, stdin);
        if(k+1 < 16) while(getchar_unlocked() != '\n');

        if(read[0] != '+'){
            tree_insert(read);
            //hash_table_insert(read);
        }
    } while(read[0] != '+');

    free(read);
}

bool parola_contiene_lettera(char* word, char x){
    int i;

    for(i = 0; i < k; i++){
        if(word[i] == x) return true;
    }

    return false;
}

bool check_vincoli(enum word_status *vincoli, char* word){
    int i, j;

    for(i = 0; i < k; i++){
        for(j = 0; j < 64; j++){
            switch (vincoli[i * 64 + j]) {
                case exact_position:
                    if(refactor(word[i]) != j){
                        return false;
                    }
                    break;
                case different_position:
                    if(refactor(word[i]) == j || !parola_contiene_lettera(word, unrefactor(j))){
                        return false;
                    }
                    break;
                case not_exists:
                    if(parola_contiene_lettera(word, unrefactor(j))){
                        return false;
                    }
                    break;
                default:
                    break;
            }
        }
    }

    return true;
}

int tree_vincoli(ptr_tree x, enum word_status *vincoli){
    if(x != NULL){
        return ((check_vincoli(vincoli, x->key)) ? 1 : 0) + tree_vincoli(x->right, vincoli) + tree_vincoli(x->left, vincoli);
    } else {
        return 0;
    }
}

void nuova_partita(){
    char p[k+1], r[k+1], res[k+1]; // r: riferimento, p: parola corrente, res: output
    res[k] = '\0';

    enum word_status vincoli[64 * k];
    memset(vincoli, none, sizeof(vincoli));

    bool won = false;
    int n, i, j, refact;
    int count_r[64] = {0}, count_r_tmp[64] = {0};

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

        if(p[0] == '+' && p[1] == 's'){
            //TODO

            j--;
        } else if(p[0] == '+' && p[1] == 'i'){
            leggi_parole();

            //TODO

            j--;
        } else {
            if(stringhe_uguali(r, p)){
                printf("ok\n");

                char x;
                do{
                    x = getchar_unlocked();
                } while(x != '+' && x != EOF);

                won = true;
                break;
            } else if(tree_search(tree, p)){
                for(i = 0; i < 64; i++){
                    count_r_tmp[i] = count_r[i];
                }

                for(i = 0; i < k; i++){
                    refact = refactor(p[i]);

                    if(p[i] == r[i]){
                        res[i] = '+';
                        count_r_tmp[refact]--;

                        vincoli[i * 64 + refact] = exact_position;
                    }
                }

                for(i = 0; i < k; i++){
                    refact = refactor(p[i]);

                    if(p[i] != r[i]){
                        if(count_r_tmp[refact] > 0){
                            res[i] = '|';
                            count_r_tmp[refact]--;

                            vincoli[i * 64 + refact] = different_position;
                        } else {
                            res[i] = '/';

                            vincoli[i * 64 + refact] = not_exists;
                        }
                    }
                }

                printf("%s\n%d\n", res, tree_vincoli(tree, vincoli));

            } else {
                printf("not_exists\n");
                j--;
            }
        }
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
        if(x != '+') while(getchar_unlocked() != '\n');

        if(x == 'n'){
            nuova_partita();
        } else if(x == 'i'){
            leggi_parole();
        }

        x = (char) getchar_unlocked();
    }

    return 0;
}

// Funzioni ausiliarie

bool tree_search(ptr_tree x, char* string) {
    if (x == NULL) {
        return false;
    } else {
        int cmp = strcmp(string, x->key);

        if (cmp == 0) {
            return true;
        } else if (cmp < 0) {
            return tree_search(x->left, string);
        } else {
            return tree_search(x->right, string);
        }
    }
}

void tree_insert(char* string) {
    ptr_tree x = tree, y = NULL, z;

    z = (ptr_tree) malloc(sizeof(tree_node_t));
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

bool stringhe_uguali(char* s1, char* s2){
    bool check = true;

    while((*s1 != '\0' || *s2 != '\0') && check){
        check = *s1 == *s2;

        s1++;
        s2++;
    }

    return check;
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

char unrefactor(int x){
    if(x < 10){
        return x + 48;
    } else if(x >= 10 && x < 36){
        return x + 55;
    } else if(x >= 36 && x < 62){
        return x + 61;
    } else if(x == 62) {
        return '-';
    } else {
        return '_';
    }
}
