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

void tree_insert(ptr_tree, ptr_node_tree);

int k;
#define CHUNK 100

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
    char x;

    fgets(r, k+1, stdin);

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
            i = 0;
            p[i] = x;
//TODO!
        }
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

    scanf("%d\n", &k);

    if(leggi_parole()){
        nuova_partita();
    } else {
        leggi_parole();
    }

    //todo

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
        if(strcmp(z->key, x->key) < 0){
            x = x->left;
        } else {
            x = x->right;
        }
    }
    z->p = y;

    if(y == T->nil){
        T->root = z;
    } else if(strcmp(z->key, y->key) < 0){
        y->left = z;
    } else {
        y->right = z;
    }

    z->left = T->nil;
    z->right = T->nil;
    z->red = true;
    tree_insert_fixup(T, z);
}