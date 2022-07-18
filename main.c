#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef struct node {
    char* key;
    struct node *left;
    struct node *right;
    struct node *p;
    bool red; //TRUE if red FALSE if black
} node_t;
typedef node_t *ptr_tree;
ptr_tree tree = NULL;

int k;
#define CHUNK 10

bool leggi_parole(){
    char* ptr;
    int i = CHUNK;

    do {
        if(i == CHUNK){
            ptr = (char*) malloc(sizeof(char) * (k+1) * CHUNK);
            i = 0;
        }
        fgets(ptr + (i * (k+1)), k+1, stdin);
        while(getchar_unlocked() != '\n');

        i++;
    } while(ptr[i * (k+1)] != '+');

    return true;
}

void nuova_partita(){

}

int main(){
    scanf("%d\n", &k);

    if(leggi_parole()){
        nuova_partita();
    } else {
        leggi_parole();
    }

    //todo

    return 0;
}

void left_rotate(ptr_tree x){
    ptr_tree y = x->right;
    x->right = y->left;

    if(y->left != NULL){
        y->left->p = x;
    }
    y->p = x->p;

    if(x->p == NULL){
        tree = y;
    } else if(x == x->p->left){
        x->p->left = y;
    } else {
        x->p->right = y;
    }
    y->left = x;
    x->p = y;
}

void right_rotate(ptr_tree x){
    ptr_tree y = x->left;
    x->left = y->right;

    if(y->right != NULL){
        y->right->p = x;
    }
    y->p = x->p;

    if(x->p == NULL){
        tree = y;
    } else if(x == x->p->right){
        x->p->right = y;
    } else {
        x->p->left = y;
    }
    y->right = x;
    x->p = y;
}

void tree_insert_fixup(ptr_tree z){
    if(z == tree){
        tree->red = false;
    } else {
        ptr_tree x = z->p, y;
        if(x->red){
            if(x == x->p->left){
                y = x->p->right;
                if(y->red){
                    x->red = false;
                    y->red = false;
                    x->p->red = true;
                    tree_insert_fixup(x->p);
                } else {
                    if(z == x->right){
                        z = x;
                        left_rotate(z);
                        x = z->p;
                    }
                    x->red = false;
                    x->p->red = true;
                    right_rotate(x->p);
                }
            } else {
                y = x->p->left;
                if(y->red){
                    x->red = false;
                    y->red = false;
                    x->p->red = true;
                    tree_insert_fixup(x->p);
                } else {
                    if(z == x->left){
                        z = x;
                        right_rotate(z);
                        x = z->p;
                    }
                    x->red = false;
                    x->p->red = true;
                    left_rotate(x->p);
                }
            }
        }
    }
}

void tree_insert(ptr_tree z){
    ptr_tree y = NULL;
    ptr_tree x = tree;

    while(x != NULL){
        y = x;
        if(strcmp(z->key, x->key) < 0){
            x = x->left;
        } else {
            x = x->right;
        }
    }
    z->p = y;

    if(y == NULL){
        tree = z;
    } else if(strcmp(z->key, y->key) < 0){
        y->left = z;
    } else {
        y->right = z;
    }

    z->left = NULL;
    z->right = NULL;
    z->red = true;
    tree_insert_fixup(z);
}