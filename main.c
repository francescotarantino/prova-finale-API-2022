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

typedef struct nodo_stack {
    ptr_node_tree key;
    struct nodo_stack *prev;
} nodo_stack_t;
typedef nodo_stack_t *ptr_nodo_stack;

ptr_tree filtered_tree;

char* lettere_esatte;
bool non_appartiene[128];
bool apparso[128];
bool* non_qui;
int num_minimo[128];
int num_esatto[128];


enum comp {
    minore,
    maggiore,
    uguale
};

void tree_insert(ptr_tree, ptr_node_tree);
bool check_presenza_albero(ptr_node_tree, char*);
enum comp string_comparison(char* x, char* y);
bool check_parola(const char*);
int check_albero(ptr_tree, ptr_node_tree, bool);
void stampa_albero_inorder(ptr_tree, ptr_node_tree);
void stampa_lista();
void aggiungi_lista_inorder(char*);
void print(char*);
void print_integer(int x);
int read_integer();
void add_to_filtered(char*);
ptr_node_tree stack_pop(ptr_nodo_stack*);

ptr_node_tree tree_delete(ptr_tree, ptr_node_tree);

int k;
int number_of_words = 0;
#define CHUNK 10000

#define CHUNK_F 10000
int i_filtered = CHUNK_F;
ptr_nodo_stack memoria_filtree = NULL;

char* ptr;
int i_leggi = CHUNK;
ptr_node_tree z_leggi = NULL;
bool leggi_parole(const bool check){
    int i, j;
    bool add;

    do {
        if(i_leggi == CHUNK){
            ptr = (char*) malloc(sizeof(char) * (k+1) * CHUNK);
            z_leggi = (ptr_node_tree) malloc(sizeof(tree_node_t) * CHUNK);
            i_leggi = 0;
        }

        i = 0;
        add = true;
        do {
            ptr[i] = getchar_unlocked();
            if(i == 0 && ptr[0] == '+'){
                if(getchar_unlocked() == 'n'){
                    while(getchar_unlocked() != '\n');

                    return true;
                } else {
                    while(getchar_unlocked() != '\n');

                    return false;
                }
            }

            // check vincoli'1
            if(check && add){
                if(non_appartiene[(unsigned char) ptr[i]]){
                    add = false;
                } else if(lettere_esatte[i] != '\0' && lettere_esatte[i] != ptr[i]) {
                    add = false;
                } else if(non_qui[i * 128 + ptr[i]]){
                    add = false;
                }
            }
            // end check vincoli'1

            i++;
        } while(i < k);
        getchar_unlocked();
        ptr[i] = '\0';

        z_leggi->p = NULL;
        z_leggi->right = NULL;
        z_leggi->left = NULL;
        z_leggi->red = true;
        z_leggi->key = ptr;
        tree_insert(tree, z_leggi);
        number_of_words++;

        // check vincoli'2
        if(check && add){
            int count;
            for(j = 45; j < 128 && add; j++){
                count = 0;

                for(i = 0; i < k; i++){
                    if(ptr[i] == j) count++;
                }

                if(num_esatto[j] != 0){
                    if(count != num_esatto[j]) add = false;
                } else {
                    if(count < num_minimo[j]) add = false;
                }

                if(j == 45) j = 47;
                if(j == 57) j = 64;
                if(j == 90) j = 94;
                if(j == 95) j = 96;
                if(j == 122) j = 128;
            }
        }
        // end check vincoli'2

        if(check && add) {
            add_to_filtered(ptr);
        }

        i_leggi++;
        ptr += k+1;
        z_leggi++;
    } while(true);
}

void nuova_partita(){
    filtered_tree = (ptr_tree) malloc(sizeof(rb_tree_t));
    filtered_tree->nil = (ptr_node_tree) malloc(sizeof(tree_node_t));
    filtered_tree->nil->left = NULL;
    filtered_tree->nil->right = NULL;
    filtered_tree->nil->p = NULL;
    filtered_tree->nil->red = false;
    filtered_tree->nil->key = NULL;
    filtered_tree->root = filtered_tree->nil;

    char p[k+1], r[k+1], res[k+1];
    res[k] = '\0';

    int n, j, i;
    bool check, won = false, never = true;
    char x;
    int count_r[128] = {0}, count_r_tmp[128] = {0}, num_minimo_tmp[128] = {0};

    lettere_esatte = calloc(k, sizeof(char));
    memset(non_appartiene, false,  sizeof(non_appartiene));
    memset(apparso, false, sizeof(apparso));
    non_qui = calloc(128, sizeof(bool) * k);
    memset(num_minimo, 0, sizeof(num_minimo));
    memset(num_esatto, 0, sizeof(num_esatto));

    i = 0;
    do {
        x = getchar_unlocked();
        r[i] = x;
        count_r[(unsigned char) x]++;
        i++;
    } while(i < k);
    r[k] = '\0';

    getchar_unlocked();
    n = read_integer();

    for(j = 0; j < n; j++){
        x = getchar_unlocked();

        if(x == '+'){
            if(getchar_unlocked() == 's'){ // stampa
                while(getchar_unlocked() != '\n');

                if(never){
                    stampa_albero_inorder(tree, tree->root);
                } else {
                    stampa_albero_inorder(filtered_tree, filtered_tree->root);
                }
                j--;
            } else { // inserisci
                while(getchar_unlocked() != '\n');

                leggi_parole(!never);
                j--;
            }
        } else {
            memcpy(count_r_tmp, count_r, sizeof(count_r));

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
                print("ok");

                do{
                    x = getchar_unlocked();
                } while(x != '+' && x != EOF);

                won = true;
                break;
            } else {
                if(check_presenza_albero(tree->root, p)){

                    // TODO si può fare con un unico ciclo??
                    for(i = 0; i < k; i++){
                        if(r[i] == p[i]){
                            lettere_esatte[i] = p[i];
                            apparso[(unsigned char) p[i]] = true;
                            num_minimo_tmp[(unsigned char) p[i]]++;
                        }
                    }
                    for(i = 0; i < k; i++){
                        if(r[i] != p[i]){
                            if(count_r_tmp[(unsigned char) p[i]] > 0){
                                res[i] = '|';

                                count_r_tmp[(unsigned char) p[i]]--;

                                apparso[(unsigned char) p[i]] = true;
                                non_qui[i * 128 + p[i]] = true;
                                num_minimo_tmp[(unsigned char) p[i]]++;
                            } else {
                                res[i] = '/';

                                if(!apparso[(unsigned char) p[i]]){
                                    non_appartiene[(unsigned char) p[i]] = true;
                                } else {
                                    num_esatto[(unsigned char) p[i]] = num_minimo_tmp[(unsigned char) p[i]];
                                }

                                non_qui[i * 128 + p[i]] = true;
                            }
                        }
                    }

                    for(i = 45; i < 128; i++){
                        if(num_minimo_tmp[i] > num_minimo[i]){
                            num_minimo[i] = num_minimo_tmp[i];
                        }

                        num_minimo_tmp[i] = 0;

                        if(i == 45) i = 47;
                        if(i == 57) i = 64;
                        if(i == 90) i = 94;
                        if(i == 95) i = 96;
                        if(i == 122) i = 128;
                    }

                    if(never) {
                        never = false;
                        i = check_albero(tree, tree->root, true);
                    } else {
                        i = check_albero(filtered_tree, filtered_tree->root, false);
                    }

                    print(res);

                    print_integer(i);
                    putchar_unlocked('\n');
                } else {
                    print("not_exists");
                    j--;
                }
            }
        }
    }

    if(!won){
        print("ko");
        getchar_unlocked();
    }

    free(lettere_esatte);
    free(non_qui);
    free(filtered_tree->nil);
    free(filtered_tree);

    while(memoria_filtree != NULL){
        free(stack_pop(&memoria_filtree));
        i_filtered = CHUNK_F;
    }
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

    k = read_integer();

    if(leggi_parole(false)){
        nuova_partita();
    } else {
        leggi_parole(false);
    }

    char x = getchar_unlocked();
    while(x != '\n' && x != EOF) {
        if(x == 'n'){
            while(getchar_unlocked() != '\n');
            nuova_partita();
        } else if(x == 'i'){
            while(getchar_unlocked() != '\n');
            leggi_parole(false);
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
    while(x != tree->nil){
        switch (string_comparison(string, x->key)) {
            case uguale:
                return true;
            case minore:
                x = x->left;
                break;
            case maggiore:
                x = x->right;
                break;
        }
    }

    return false;
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

bool check_parola(const char* word){
    int i, j, count;

    for(j = 0; j < k; j++){
        if(non_appartiene[(unsigned char) word[j]]){
            return false;
        } else if(lettere_esatte[j] != '\0' && lettere_esatte[j] != word[j]){
            return false;
        } else if(non_qui[j * 128 + word[j]]){
            return false;
        }
    }

    for(j = 45; j < 128; j++){
        count = 0;

        for(i = 0; i < k; i++){
            if(word[i] == j) count++;
        }

        if(num_esatto[j] != 0){
            if(count != num_esatto[j]) return false;
        } else {
            if(count < num_minimo[j]) return false;
        }

        if(j == 45) j = 47;
        if(j == 57) j = 64;
        if(j == 90) j = 94;
        if(j == 95) j = 96;
        if(j == 122) j = 128;
    }

    return true;
}

ptr_node_tree tree_maximum(ptr_tree T, ptr_node_tree x){
    while(x->right != T->nil){
        x = x->right;
    }

    return x;
}

ptr_node_tree tree_predecessor(ptr_tree T, ptr_node_tree x){
    if(x->left != T->nil){
        return tree_maximum(T, x->left);
    }

    ptr_node_tree y = x->p;
    while (y != T->nil && x == y->left){
        x = y;
        y = y->p;
    }

    return y;
}

ptr_nodo_stack stack = NULL;

void stack_push(ptr_nodo_stack* y, ptr_node_tree x){
    ptr_nodo_stack tmp = *y;

    *y = (ptr_nodo_stack) malloc(sizeof(nodo_stack_t));
    (*y)->prev = tmp;
    (*y)->key = x;
}

ptr_node_tree stack_pop(ptr_nodo_stack* x){
    ptr_nodo_stack tmp = *x;
    ptr_node_tree to_return;

    if(*x != NULL){
        (*x) = tmp->prev;
        to_return = tmp->key;
        free(tmp);
    } else {
        to_return = NULL;
    }

    return to_return;
}

ptr_nodo_stack stack_del = NULL;

int check_albero(ptr_tree TREE, ptr_node_tree T, bool never){
    int counter = 0;
    bool go = true;

    while(go){
        if(T != tree->nil && T != filtered_tree->nil){
            stack_push(&stack, T);
            T = T->left;
        } else {
            T = stack_pop(&stack);

            if(T == NULL){
                go = false;
            } else {
                if(never){
                    if(check_parola(T->key)){
                        counter++;

                        add_to_filtered(T->key);
                    }
                } else {
                    if(check_parola(T->key)){
                        counter++;
                    } else {
                        if(!never){
                            stack_push(&stack_del, T);
                        }
                    }
                }

                T = T->right;
            }
        }
    }


    if(!never){
        ptr_node_tree del = stack_pop(&stack_del);
        while(del != NULL){
            tree_delete(filtered_tree, del);
            del = stack_pop(&stack_del);
        }
    }


    return counter;
}

void stampa_albero_inorder(ptr_tree TREE, ptr_node_tree T){
    if(T != TREE->nil){
        stampa_albero_inorder(TREE, T->left);

        print(T->key);

        stampa_albero_inorder(TREE, T->right);
    }
}

void print(char* string){
    while(*string != '\0'){
        putchar_unlocked(*string);
        string++;
    }
    putchar_unlocked('\n');
}

void print_integer(int x){
    if(x >= 10){
        print_integer(x / 10);
    }

    putchar_unlocked(48 + x % 10);
}

int read_integer(){
    char x;
    int res = 0;

    while((x = getchar_unlocked()) != '\n'){
        res = (res * 10) + x - 48;
    }

    return res;
}

ptr_node_tree tree_minimum(ptr_tree T, ptr_node_tree x){
    while(x->left != T->nil){
        x = x->left;
    }

    return x;
}

ptr_node_tree tree_successor(ptr_tree T, ptr_node_tree x){
    if(x->right != T->nil){
        return tree_minimum(T, x->right);
    }

    ptr_node_tree y = x->p;
    while(y != T->nil && x == y->right){
        x = y;
        y = y->p;
    }

    return y;
}

void tree_delete_fixup(ptr_tree T, ptr_node_tree x){
    ptr_node_tree w;
    if(x->red || x->p == T->nil){
        x->red = false;
    } else if(x == x->p->left) {
        w = x->p->right;
        if(w->red){
            w->red = false;
            x->p->red = true;
            left_rotate(T, x->p);
            w = x->p->right;
        }

        if(w->left->red == false && w->right->red == false){
            w->red = true;
            tree_delete_fixup(T, x->p);
        } else {
            if(w->right->red == false){
                w->left->red = false;
                w->red = true;
                right_rotate(T, w);
                w = x->p->right;
            }
            w->red = x->p->red;
            x->p->red = false;
            w->right->red = false;
            left_rotate(T, x->p);
        }
    } else {
        w = x->p->left;
        if(w->red){
            w->red = false;
            x->p->red = true;
            right_rotate(T, x->p);
            w = x->p->left;
        }

        if(w->left->red == false && w->right->red == false){
            w->red = true;
            tree_delete_fixup(T, x->p);
        } else {
            if(w->left->red == false){
                w->right->red = false;
                w->red = true;
                left_rotate(T, w);
                w = x->p->left;
            }
            w->red = x->p->red;
            x->p->red = false;
            w->left->red = false;
            right_rotate(T, x->p);
        }
    }
}

ptr_node_tree tree_delete(ptr_tree T, ptr_node_tree z) {
    ptr_node_tree x, y;
    if(z->left == T->nil || z->right == T->nil){
        y = z;
    } else {
        y = tree_successor(T, z);
    }

    if(y->left != T->nil){
        x = y->left;
    } else {
        x = y->right;
    }

    x->p = y->p;

    if(y->p == T->nil){
        T->root = x;
    } else if(y == y->p->left){
        y->p->left = x;
    } else {
        y->p->right = x;
    }

    if(y != z){
        z->key = y->key;
    }

    if(y->red == false){
        tree_delete_fixup(T, x);
    }

    return y;
}

void add_to_filtered(char* add){
    ptr_node_tree new;
    if(i_filtered == CHUNK_F){
        new = (ptr_node_tree) malloc(CHUNK_F*sizeof(tree_node_t));
        stack_push(&memoria_filtree, new);
        i_filtered = 0;
    } else {
        new = memoria_filtree->key + i_filtered;
    }

    new->p = NULL;
    new->right = NULL;
    new->left = NULL;
    new->red = true;
    new->key = add;

    tree_insert(filtered_tree, new);

    i_filtered++;
}