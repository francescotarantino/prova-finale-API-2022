#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef struct tree_node {
    char* key;
    struct tree_node *left;
    struct tree_node *right;
    struct tree_node *p;
    bool red; //TRUE if red FALSE if black
} tree_node_t;
typedef tree_node_t *ptr_node_tree;

typedef struct {
    tree_node_t *root;
    tree_node_t *nil;
} rb_tree_t;
typedef rb_tree_t *ptr_tree;
ptr_tree tree;

typedef struct node_list {
    char* key;
    struct node_list *next;
} node_list_t;
typedef node_list_t *ptr_list;
ptr_list pointer_memory_list = NULL, list = NULL, last = NULL;

typedef struct {
    char* lettere_esatte;
    bool non_appartiene[128];
    bool apparso[128];
    bool* non_qui;
    int num_minimo[128];
    int num_esatto[128];
} vincoli_t;

enum comp {
    minore,
    maggiore,
    uguale
};

void tree_insert(ptr_tree, ptr_node_tree);
bool check_presenza_albero(ptr_node_tree, char*);
enum comp string_comparison(char* x, char* y);
bool check_parola(vincoli_t*, const char*);

int k;
#define CHUNK 300

char* ptr;
int i_leggi = CHUNK;
ptr_node_tree z_leggi = NULL;
bool leggi_parole(){
    do {
        if(i_leggi == CHUNK){
            ptr = (char*) malloc(sizeof(char) * (k+1) * CHUNK);
            z_leggi = (ptr_node_tree) malloc(sizeof(tree_node_t) * (k+1) * CHUNK);
            i_leggi = 0;
        }
        fgets(ptr, k+1, stdin);
        while(getchar_unlocked() != '\n');

        if(*ptr != '+'){
            z_leggi->p = NULL;
            z_leggi->right = NULL;
            z_leggi->left = NULL;
            z_leggi->red = true;
            z_leggi->key = ptr;
            tree_insert(tree, z_leggi);
        }

        i_leggi++;
        ptr += k+1;
        z_leggi++;
    } while(ptr[-(k+1)] != '+');

    if(ptr[-(k+1) + 1] == 'n'){
        return true;
    } else {
        return false;
    }
}

void nuova_partita(){
    char p[k+1], r[k+1], res[k+1];
    res[k] = '\0';

    int n, j, i;
    bool check, won = false, never = true;
    char x;
    int count_r[128] = {0}, count_r_tmp[128] = {0}, num_minimo_tmp[128] = {0};

    vincoli_t vincoli;
    vincoli.lettere_esatte = calloc(k, sizeof(char));
    memset(vincoli.non_appartiene, false, 128 * sizeof(bool));
    memset(vincoli.apparso, false, 128 * sizeof(bool));
    vincoli.non_qui = calloc(128, sizeof(bool) * k);
    memset(vincoli.num_minimo, 0, 128 * sizeof(bool));
    memset(vincoli.num_esatto, 0, 128 * sizeof(bool));

    i = 0;
    do {
        x = getchar_unlocked();
        r[i] = x;
        count_r[(unsigned char) x]++;
        i++;
    } while(i < k);
    r[k] = '\0';

    scanf("%d\n", &n);

    for(j = 0; j < n; j++){
        x = getchar_unlocked();

        if(x == '+'){
            if(getchar_unlocked() == 's'){
                // TODO Stampa
                j--;
            } else {
                leggi_parole();

                j--;
            }

            while(getchar_unlocked() != '\n');
        } else {
            memcpy(count_r_tmp, count_r, 128 * sizeof(int));

            i = 0;
            check = true;
            do {
                if(x != r[i]){
                    check = false;
                } else {
                    res[i] = '+';
                    count_r_tmp[(unsigned char) x]--;
                }
                p[i] = x;
                x = getchar_unlocked();
                i++;
            } while(i < k);
            p[i] = '\0';

            if(check){
                printf("ok\n");

                do{
                    x = getchar_unlocked();
                } while(x != '+' && x != EOF);

                won = true;
                break;
            } else {
                if(check_presenza_albero(tree->root, p)){
                    for(i = 0; i < k; i++){
                        if(r[i] != p[i]){
                            if(count_r_tmp[(unsigned char) p[i]] > 0){
                                res[i] = '|';

                                count_r_tmp[(unsigned char) p[i]]--;

                                vincoli.apparso[(unsigned char) p[i]] = true;
                                vincoli.non_qui[i * 128 + p[i]] = true;
                                num_minimo_tmp[(unsigned char) p[i]]++;
                            } else {
                                res[i] = '/';

                                if(!vincoli.apparso[(unsigned char) p[i]]){
                                    vincoli.non_appartiene[(unsigned char) p[i]] = true;
                                } else {
                                    vincoli.num_esatto[(unsigned char) p[i]] = num_minimo_tmp[(unsigned char) p[i]];
                                }

                                vincoli.non_qui[i * 128 + p[i]] = true;
                            }
                        } else {
                            vincoli.lettere_esatte[i] = p[i];
                            vincoli.apparso[(unsigned char) p[i]] = true;
                            num_minimo_tmp[(unsigned char) p[i]]++;
                        }
                    }

                    for(i = 45; i < 128; i++){
                        if(num_minimo_tmp[i] > vincoli.num_minimo[i]){
                            vincoli.num_minimo[i] = num_minimo_tmp[i];
                        }

                        num_minimo_tmp[i] = 0;

                        if(i == 45) i = 47;
                        if(i == 57) i = 64;
                        if(i == 90) i = 96;
                        if(i == 122) i = 128;

                    }

                    printf("%s\n", res);
                } else {
                    printf("not_exists\n");
                    j--;
                }
            }
        }
    }

    if(!won){
        printf("ko\n");
        getchar_unlocked();
    }

    free(vincoli.lettere_esatte);
    free(vincoli.non_qui);
}

int main(){
    tree = (ptr_tree) malloc(sizeof(rb_tree_t));
    tree->nil = (ptr_node_tree) malloc(sizeof(tree_node_t));
    tree->nil->left = NULL;
    tree->nil->right = NULL;
    tree->nil->p = NULL;
    tree->nil->red = false;
    tree->nil->key = NULL;
    tree->root = tree->nil;

    scanf("%d\n", &k);

    if(leggi_parole()){
        nuova_partita();
    } else {
        leggi_parole();
    }

    char x = getchar_unlocked();
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

void left_rotate(ptr_tree T, ptr_node_tree x){
    ptr_node_tree y = x->right;
    x->right = y->left;

    if(y->left != T->nil){
        y->left->p = x;
    }
    y->p = x->p;

    if(x->p == T->nil){
        T->root = y;
    } else if(x == x->p->left){
        x->p->left = y;
    } else {
        x->p->right = y;
    }
    y->left = x;
    x->p = y;
}

void right_rotate(ptr_tree T, ptr_node_tree x){
    ptr_node_tree y = x->left;
    x->left = y->right;

    if(y->right != T->nil){
        y->right->p = x;
    }
    y->p = x->p;

    if(x->p == T->nil){
        T->root = y;
    } else if(x == x->p->right){
        x->p->right = y;
    } else {
        x->p->left = y;
    }
    y->right = x;
    x->p = y;
}

void tree_insert_fixup(ptr_tree T, ptr_node_tree z){
    if(z == T->root){
        T->root->red = false;
    } else {
        ptr_node_tree x = z->p, y;
        if(x->red){
            if(x == x->p->left){
                y = x->p->right;
                if(y->red){
                    x->red = false;
                    y->red = false;
                    x->p->red = true;
                    tree_insert_fixup(T, x->p);
                } else {
                    if(z == x->right){
                        z = x;
                        left_rotate(T, z);
                        x = z->p;
                    }
                    x->red = false;
                    x->p->red = true;
                    right_rotate(T, x->p);
                }
            } else {
                y = x->p->left;
                if(y->red){
                    x->red = false;
                    y->red = false;
                    x->p->red = true;
                    tree_insert_fixup(T, x->p);
                } else {
                    if(z == x->left){
                        z = x;
                        right_rotate(T, z);
                        x = z->p;
                    }
                    x->red = false;
                    x->p->red = true;
                    left_rotate(T, x->p);
                }
            }
        }
    }
}

void tree_insert(ptr_tree T, ptr_node_tree z){
    ptr_node_tree y = T->nil;
    ptr_node_tree x = T->root;

    while(x != T->nil){
        y = x;
        if(string_comparison(z->key, x->key) == minore){
            x = x->left;
        } else {
            x = x->right;
        }
    }
    z->p = y;

    if(y == T->nil){
        T->root = z;
    } else if(string_comparison(z->key, y->key) == minore){
        y->left = z;
    } else {
        y->right = z;
    }

    z->left = T->nil;
    z->right = T->nil;
    z->red = true;
    tree_insert_fixup(T, z);
}

bool check_presenza_albero(ptr_node_tree x, char* string) {
    if (x == tree->nil) {
        return false;
    } else {
        switch (string_comparison(string, x->key)) {
            case uguale:
                return true;
            case minore:
                return check_presenza_albero(x->left, string);
            case maggiore:
                return check_presenza_albero(x->right, string);
        }
    }
}

enum comp string_comparison(char* x, char* y){ // true if x <= y, false otherwise
    while(*x != '\0' || *y != '\0'){
        if(*x < *y){
            return minore;
        } else if(*x > *y){
            return maggiore;
        }
        x++;
        y++;
    }

    return uguale;
}

bool check_parola(vincoli_t *vincoli, const char* word){
    int i, j, count;

    for(j = 0; j < k; j++){
        if(vincoli->non_appartiene[(unsigned char) word[j]]){
            return false;
        } else if(vincoli->lettere_esatte[j] != '\0' && vincoli->lettere_esatte[j] != word[j]){
            return false;
        } else if(vincoli->non_qui[j * 128 + word[j]]){
            return false;
        }
    }

    for(j = 45; j < 128; j++){
        count = 0;

        for(i = 0; i < k; i++){
            if(word[i] == j) count++;
        }

        if(vincoli->num_esatto[j] != 0){
            if(count != vincoli->num_esatto[j]) return false;
        } else {
            if(count < vincoli->num_minimo[j]) return false;
        }

        if(j == 45) j = 47;
        if(j == 57) j = 64;
        if(j == 90) j = 96;
        if(j == 122) j = 128;
    }

    return true;
}