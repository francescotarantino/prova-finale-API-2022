#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define DEBUG 0

char* ignore;

typedef struct word {
    char* word;
    struct word *next;
} word_list_t;

typedef word_list_t *ptr_list;

ptr_list L;

typedef enum { false, true } bool;

int refactor(char x){
    if(x >= 48 && x <= 57){ // 0-9
        return x-48;
    } else if(x >= 65 && x <= 90){ // A-Z
        return x - 65 + 10;
    } else if(x >= 97 && x <= 122){ // a-z
        return x - 97 + 10 + 26; //TODO: ridurre
    } else if(x == 45){ // -
        return 62;
    } else if(x == 95){ // _
        return 63;
    }

    return 0;
}

void read_words(int k){
    ptr_list T, prev;
    int read_length = (k+2 > 19) ? k+2 : 19; // Max fra k+2 e 19 TODO: globale

    if(L == NULL){
        L = (ptr_list) malloc(sizeof(word_list_t));
        T = L;
    } else {
        T = L;
        while(T->next != NULL){
            T = T->next;
        }
        T->next = (ptr_list) malloc(sizeof(word_list_t));
        T = T->next;
    }

    T->word = (char*) malloc((k + 1) * sizeof(char));
    ignore = fgets(T->word, read_length, stdin);
    T->word[k] = '\0';
    do {
        T->next = (ptr_list) malloc(sizeof(word_list_t));
        prev = T;
        T = T->next;
        T->word = (char *) malloc((k + 1) * sizeof(char));
        ignore = fgets(T->word, read_length, stdin);
        T->word[k] = '\0';
    } while (T->word[0] != '+');
    free(T);
    prev->next = NULL;
}

bool is_in_dictionary(char* word){
    ptr_list T = L;

    while(T != NULL){
        if(strcmp(T->word, word) == 0) return true;
        T = T->next;
    }

    return false;
}

bool check_vincoli(char* word, int k, const bool non_appartiene[], const char* deve_comparire, const bool non_compare[], const int num_min_compare[], const int num_esatto_compare[]){
    bool check = true;
    int refact, j, i, count;

    for(j = 0; j < k && check; j++){
        refact = refactor(word[j]);

        if(deve_comparire[j] != '\0' && deve_comparire[j] != word[j]){
            check = false;
        } else if(non_appartiene[refact]){
            check = false;
        } else if(non_compare[j * 64 + refact]){
            check = false;
        }
    }

    for(j = 0; j < 64 && check; j++){
        count = 0;
        for(i = 0; i < k; i++){
            if(j == refactor(word[i])) count++;
        }

        if(num_esatto_compare[j] != -1){
            if(count != num_esatto_compare[j]) check = false;
        } else {
            if(count < num_min_compare[j]) check = false;
        }
    }

    return check;
}

int sort_string(const void *str1, const void *str2 ) {
    return strcmp(str1, str2);
}

void new_game(int k){
    char p[k+1], r[k+1], res[k+1];
    // r è la parola di riferimento, p la parola corrente
    int count_r[64] = {0};
    bool apparso[64] = {[0 ... 63] = false};
    int n, i, j, ii;
    int read_length = (k+2 > 19) ? k+2 : 19;
    ptr_list T;
    int refact;
    int count_filtered;
    bool won = false;

    // TODO: controllare le inizializzazioni!!!

    bool non_appartiene[64] = {[0 ... 63] = false}; // true se l'i-esimo simbolo NON appartiene a rif

    char deve_comparire[k]; // il simbolo appartiene all'i-esima posizione, se \0 allora ND
    for(i = 0; i < k; i++) deve_comparire[i] = '\0';

    bool non_compare[64 * k]; // true nel i-esimo posto non compare il simbolo j-esimo
    for(i = 0; i < k; i++)
        for(j = 0; j < 64; j++)
            non_compare[i * 64 + j] = false;

    int num_min_compare[64] = {0}; // # MINIMO di volte che l'i-esimo carattere compare
    int num_min_compare_temp[64] = {0};

    int num_esatto_compare[64] = {[0 ... 63] = -1};

    res[k] = '\0';

    #if DEBUG
    printf("\nNew game\n");
    #endif

    ignore = fgets(r, k+1, stdin);
    #if DEBUG
    printf("String: %s\n", r);
    #endif

    for(i = 0; i < k; i++){
        count_r[refactor(r[i])]++;
    }

    if(!scanf("%d\n", &n)){
        return;
    }
    #if DEBUG
    printf("#confronti: %d\n", n);
    #endif

    for(i = 0; i < n; i++){
        ignore = fgets(p, read_length, stdin);

        count_filtered = 0;

        if(p[0] == '+' && p[1] == 's') { // +stampa_filtrate
            #if DEBUG
            printf("Stampa filtrate!\n");
            #endif

            //AAAAAAA TODO: troppo complesso
            T = L;
            while(T != NULL){
                if(check_vincoli(T->word, k, non_appartiene, deve_comparire, non_compare, num_min_compare, num_esatto_compare)) count_filtered++;

                T = T->next;
            }
            char output[count_filtered][k + 1];
            //fine aaaaaa

            count_filtered = 0;
            T = L;
            while(T != NULL){
                if(check_vincoli(T->word, k, non_appartiene, deve_comparire, non_compare, num_min_compare, num_esatto_compare)){
                    strcpy(output[count_filtered], T->word);
                    count_filtered++;
                }


                T = T->next;
            }

            qsort(output, count_filtered, k+1, sort_string);

            for(j = 0; j < count_filtered; j++){
                printf("%s\n", output[j]);
            }

            i--;
        } else if(p[0] == '+' && p[1] == 'i'){ // +inserisci_inizio
            #if DEBUG
            printf("Inserisci parole\n");
            #endif

            read_words(k);
            
            i--;
        } else {
            p[k] = '\0';
            #if DEBUG
            printf("Curr: %s\n", p);
            #endif

            if(strcmp(p, r) == 0){ // Verifico se p == r TODO: migliorare la verifica (complessità)
                printf("ok\n");
                //TODO: bisogna terminare la partita!!!

                char x;
                do {
                    x = getchar();
                } while(x != '+' && x != EOF);

                won = true;
                break;
            } else if(is_in_dictionary(p)){ // Verifico se p è nel dizionario

                //TODO: da migliorare
                for(ii = 0; ii < 64; ii++) count_r[ii] = 0;
                for(ii = 0; ii < k; ii++){
                    count_r[refactor(r[ii])]++;
                }
                //fino a qui

                for(j = 0; j < k; j++) {
                    refact = refactor(p[j]);

                    if(p[j] == r[j]){ // match dei caratteri
                        res[j] = '+';
                        count_r[refact]--;

                        apparso[refact] = true;
                        deve_comparire[j] = p[j];
                        num_min_compare_temp[refact]++;
                    }
                }

                for(j = 0; j < k; j++){
                    refact = refactor(p[j]);

                    if(deve_comparire[j] != p[j]){
                        if(count_r[refact] > 0){
                            res[j] = '|';
                            count_r[refact]--;

                            apparso[refact] = true;
                            non_compare[j * 64 + refact] = true;
                            num_min_compare_temp[refact]++;
                        } else {
                            res[j] = '/';

                            if(!apparso[refact]){ // se il carattere attuale non è apparso mai durante la scansione
                                non_appartiene[refact] = true;
                            } else {
                                num_esatto_compare[refact] = num_min_compare_temp[refact];
                            }
                            non_compare[j * 64 + refact] = true;
                        }
                    }
                }

                for(j = 0; j < 64; j++){
                    if(num_min_compare_temp[j] > num_min_compare[j]){
                        num_min_compare[j] = num_min_compare_temp[j];
                    }
                    num_min_compare_temp[j] = 0;
                }

                T = L;
                while(T != NULL){
                    if(check_vincoli(T->word, k, non_appartiene, deve_comparire, non_compare, num_min_compare, num_esatto_compare)) count_filtered++;

                    T = T->next;
                }

                printf("%s\n%d\n", res, count_filtered);
            } else {
                printf("not_exists\n");
                i--;
            }
        }
    }

    if(!won){
        printf("ko\n");
        getchar();
    }
}

void print_list(){
    ptr_list T = L;
    printf("Print list:\n");
    while(T != NULL){
        printf("%s\n", T->word);
        T = T->next;
    }
}

int main(){
    int k;

    L = NULL;

    if(scanf("%d\n", &k) > 0){
        read_words(k);
    }

    #if DEBUG
    print_list(L);
    #endif

    new_game(k);

    char x, y;

    x = getchar();
    while(x != EOF){
        if(x == 'n'){
            do{
                y = getchar();
            } while(y != '\n');
            new_game(k);
        } else if(x == 'i'){
            do{
                y = getchar();
            } while(y != '\n');

            read_words(k);
        }
        x = getchar();
    }

    return 0;
}
