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

typedef struct node_list {
    char* key;
    struct node_list *next;
} node_list_t;
typedef node_list_t *ptr_list;
ptr_list list = NULL, last = NULL;

typedef struct {
    char* lettere_esatte;
    bool non_appartiene[64];
    bool apparso[64];
    bool* non_qui;
    int num_minimo[64];
    int num_esatto[64];
} vincolo_t;

int k;
char* ignore;
int refactor(char);
bool stringhe_uguali(char*, char*);
void print_list();
bool check_presenza_albero(ptr_tree, char*);

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

void leggi_parole(){
    char* read;

    do {
        read = (char*) malloc(sizeof(char) * (k+1));
        ignore = fgets(read, k+1, stdin);
        if(k+1 < 16) while(getchar_unlocked() != '\n');

        if(read[0] != '+') aggiungi_albero(read);
    } while(read[0] != '+');
}

void create_inorder_list(ptr_tree x){
    if(x != NULL){
        create_inorder_list(x->left);

        ptr_list new;
        new = (ptr_list) malloc(sizeof(node_list_t));
        new->key = x->key;
        new->next = NULL;

        if(list == NULL){
            list = new;
            last = new;
        } else {
            last->next = new;
            last = new;
        }

        create_inorder_list(x->right);
    }
}

void delete_from_list(ptr_list x, ptr_list prev){
    if(prev != NULL){
        prev->next = x->next;
    } else {
        list = x->next;
    }

    free(x);
}

int check_vincoli(vincolo_t *vincoli){
    bool check = true;
    int refact, j, i, sum = 0, count;
    ptr_list x = list, prev = NULL, tmp;

    while(x != NULL){
        for(j = 0; j < k && check; j++){
            refact = refactor(x->key[j]);

            if(vincoli->lettere_esatte[j] != '\0' && vincoli->lettere_esatte[j] != x->key[j]){
                check = false;
            } else if(vincoli->non_appartiene[refact]){
                check = false;
            } else if(vincoli->non_qui[j * 64 + refact]){
                check = false;
            }
        }

        if(check){
            for(j = 0; j < 64 && check; j++){
                count = 0;

                for(i = 0; i < k; i++){
                    if(refactor(x->key[i]) == j) count++;
                }

                if(vincoli->num_esatto[j] != 0){
                    if(count != vincoli->num_esatto[j]) check = false;
                } else {
                    if(count < vincoli->num_minimo[j]) check = false;
                }
            }
        }

        if(check){
            sum++;
            prev = x;
            x = x->next;
        } else {
            tmp = x->next;
            delete_from_list(x, prev);
            x = tmp;
        }

        check = true;
    }

    return sum;
}

void nuova_partita(){
    create_inorder_list(tree);

    char p[k+1], r[k+1], res[k+1]; // r: riferimento, p: parola corrente, res: output
    res[k] = '\0';

    int n, i, j, refact;
    int count_r[64] = {0}, count_r_tmp[64] = {0};
    bool won = false;
    char x;

    vincolo_t vincoli;
    vincoli.lettere_esatte = malloc(sizeof(char) * k);
    memset(vincoli.lettere_esatte, 0, sizeof(char) * k);
    memset(vincoli.non_appartiene, false, sizeof(vincoli.non_appartiene));
    memset(vincoli.apparso, false, sizeof(vincoli.apparso));
    vincoli.non_qui = malloc(sizeof(bool) * 64 * k);
    memset(vincoli.non_qui, false, sizeof(bool) * 64 * k);
    memset(vincoli.num_minimo, 0, sizeof(vincoli.num_minimo));
    memset(vincoli.num_esatto, 0, sizeof(vincoli.num_esatto));

    int num_minimo_tmp[64] = {0};

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
            print_list(tree);

            j--;
        } else if(p[0] == '+' && p[1] == 'i') { //+inserisci_inizio
            leggi_parole();

            ptr_list tmp;
            while(list != NULL){
                tmp = list;
                list = list->next;
                free(tmp);
            }
            create_inorder_list(tree);
            check_vincoli(&vincoli);

            j--;
        } else {
            if(stringhe_uguali(p, r)){
                printf("ok\n");

                do{
                    x = getchar_unlocked();
                } while(x != '+' && x != EOF);

                won = true;
                break;
            } else if(check_presenza_albero(tree, p)){
                for(i = 0; i < 64; i++){
                    count_r_tmp[i] = count_r[i];
                }

                for(i = 0; i < k; i++){
                    refact = refactor(p[i]);

                    if(p[i] == r[i]){
                        res[i] = '+';
                        count_r_tmp[refact]--;

                        vincoli.lettere_esatte[i] = p[i];
                        vincoli.apparso[refact] = true;
                        num_minimo_tmp[refact]++;
                    }
                }

                for(i = 0; i < k; i++){
                    refact = refactor(p[i]);

                    if(vincoli.lettere_esatte[i] != p[i]){
                        if(count_r_tmp[refact] > 0){
                            res[i] = '|';
                            count_r_tmp[refact]--;

                            vincoli.apparso[refact] = true;
                            vincoli.non_qui[i * 64 + refact] = true;
                            num_minimo_tmp[refact]++;
                        } else {
                            res[i] = '/';

                            if(!vincoli.apparso[refact]){
                                vincoli.non_appartiene[refact] = true;
                            } else {
                                vincoli.num_esatto[refact] = num_minimo_tmp[refact];
                            }

                            vincoli.non_qui[i * 64 + refact] = true;
                        }
                    }
                }

                for(i = 0; i < 64; i++){
                    if(num_minimo_tmp[i] > vincoli.num_minimo[i]){
                        vincoli.num_minimo[i] = num_minimo_tmp[i];
                    }

                    num_minimo_tmp[i] = 0;
                }

                printf("%s\n%d\n", res, check_vincoli(&vincoli));
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

    free(vincoli.lettere_esatte);
    free(vincoli.non_qui);

    ptr_list tmp;
    while(list != NULL){
        tmp = list;
        list = list->next;
        free(tmp);
    }
}

int main(){
    char x;

    if(scanf("%d\n", &k) > 0){
        leggi_parole();
    }

    nuova_partita();

    x = getchar_unlocked();
    while(x != '\n' && x != EOF) {
        if(x == 'n'){
            while(getchar_unlocked() != '\n');
            nuova_partita();
        } else if(x == 'i'){
            while(getchar_unlocked() != '\n');
            leggi_parole();
        }

        x = getchar_unlocked();
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

bool stringhe_uguali(char* s1, char* s2){
    bool check = true;

    while((*s1 != '\0' || *s2 != '\0') && check){
        check = *s1 == *s2;

        s1++;
        s2++;
    }

    return check;
}

void print_list(){
    ptr_list tmp = list;

    while(tmp != NULL){
        printf("%s\n", tmp->key);
        tmp = tmp->next;
    }
}

bool check_presenza_albero(ptr_tree x, char* string) {
    if (x == NULL) {
        return false;
    } else {
        int cmp = strcmp(string, x->key);

        if (cmp == 0) {
            return true;
        } else if (cmp < 0) {
            return check_presenza_albero(x->left, string);
        } else {
            return check_presenza_albero(x->right, string);
        }
    }
}
