#include <stdio.h>
#include <stdlib.h>
#include "list.h"


typedef struct{
   int sudo[9][9];
}Node;

Node* createNode(){
  Node* n=(Node) malloc(sizeof(Node));
  return n;
}

Node copy(Node* n){
    Node* new=(Node*) malloc(sizeof(Node));
    new =n;
    return new;
}

Node* read_file (char* file_name){
  Node* n = createNode();
  FILE* file = fopen (file_name, "r");
  int i,j;
  for(i=0;i<9;i++){
       for(j=0;j<9;j++){
          if(!fscanf (file, "%d", &n->sudo[i][j]))
            printf("failed to read data!");
       }
  }

  fclose (file);
  return n;
}

void print_node(Node* n){
    int i,j;
    for(i=0;i<9;i++){
       for(j=0;j<9;j++)
          printf("%d ", n->sudo[i][j]);
       printf("\n");
    }
    printf("\n");
}

int is_valid(Node* n){
    int vistos[10];

    //FILAS
    for (int i = 0; i < 9; i++){
        for (int j = 0; j < 10; j++){
            vistos[j] = 0;
        }
        for (int j = 0; j < 9; j++){
            if (n->sudo[i][j] != 0){
                if (vistos[n->sudo[i][j]] == 1) return 0;
                vistos[n->sudo[i][j]] = 1;
            }
        }
    }

    //COLS
    for (int i = 0; i < 9; i++){
        for (int j = 0; j < 10; j++){
            vistos[j] = 0;
        }
        for (int j = 0; j < 9; j++){
            if (n->sudo[j][i] != 0){
                if (vistos[n->sudo[j][i]] == 1) return 0;
                vistos[n->sudo[j][i]] = 1;
            }
        }
    }

    //SUBMATRICES
    for (int i = 0; i < 9; i+=3){
        for (int j = 0; j < 9; j+=3){
            for (int k = 0; k < 10; k++){
                vistos[k] = 0;
            }
            for (int k = i; k < i+3; k++){
                for (int l = j; l < j+3; l++){
                    if (n->sudo[k][l] != 0){
                        if (vistos[n->sudo[k][l]] == 1) return 0;
                        vistos[n->sudo[k][l]] = 1;
                    }
                }
            }
        }
    }

    return 1;
}


List* get_adj_nodes(Node* n){
    List* list = createList();

    int fila = -1 , col = -1;
    for (int i = 0; i < 9 && fila == -1; i++){
        for (int j = 0; j < 9; j++){
            if (n->sudo[i][j] == 0){
                fila = i;
                col = j;
                break;
            }
        } 
    }
    if (col == -1 || fila == -1) return list;

    for (int i = 1; i <= 9; i++){
        Node* new = copy(n);
        new->sudo[fila][col] = i;

        if (is_valid(new)){
            pushBack(list, new);
        }
        else{
            free(new);
        }
    }

    return list;
}

int is_final(Node* n){
    for (int i = 0; i < 9; i++){
        for (int j = 0; j < 9; j++){
            if (n->sudo[i][j] == 0) return 0;
        }
    }
    return 1;
}

Node* DFS(Node* initial, int* cont){
    Stack* stack = createStack();
    push(stack, initial);
    Node* current = NULL;

    while (!is_empty(stack)){
        current = top(stack);
        pop(stack);
        (cont)++;

        if (is_final(current)){
            clean(stack);
            return current;
        }

        List adj = get_adj_nodes(current);
        Node* node = first(adj);

        while (node != NULL){
            push(stack, node);
            node = next(adj);
        }
        clean(adj);
    }
    clean(stack);
    return NULL;
}




int main( int argc, charargv[] ){

  Node* initial= read_file("s12a.txt");;

  int cont=0;
  Node* final = DFS(initial, &cont);
  printf("iterations:%d\n",cont);
  print_node(final);

  return 0;
}