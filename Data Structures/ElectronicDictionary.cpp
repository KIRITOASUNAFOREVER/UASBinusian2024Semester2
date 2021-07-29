#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct extraData{
    char lang[31];
};

struct node{
    char word[51];
    int height;
    extraData data;
    struct node *left, *right;
}*root;

struct langs {
    char lang[31];
    int total;
    langs *next;
}*head;

int totalLang;
bool Available;
char delWord[51];

struct node *newNode(char word[], extraData data){
    node *temp = (node*) malloc(sizeof(node));
    strcpy(temp->word, word);
    temp->left = NULL;
    temp->right = NULL;
    temp->height = 1;
    temp->data = data;
    return temp;
}

int max(int a, int b){
    if(a > b){
        return a;
    } else return b;
}

int getHeight(node* curr){
    if(!curr){
        return 0;
    } else {
        return curr->height;
    }
}

int getBF(node* curr){ 
    if(!curr) return 0;
    return getHeight(curr->left) - getHeight(curr->right);
}

node* rightRotate(node* A){
    node *B = A->left;
    node *loseChild = B->right;
    B->right = A;
    A->left = loseChild;

    A->height = max(getHeight(A->left), getHeight(A->right)) + 1;
    B->height = max(getHeight(B->left), getHeight(B->right)) + 1;

    return B;
}

node* leftRotate(node* A){
    node *B = A->right;
    node *loseChild = B->left;
    B->left = A;
    A->right = loseChild;

    A->height = max(getHeight(A->left), getHeight(A->right)) + 1;
    B->height = max(getHeight(B->left), getHeight(B->right)) + 1;

    return B;
}

struct node *input(node* curr, char word[51], extraData data){
    if(!curr){
        curr = newNode(word, data);
    } else {
        if(strcmp (word, curr->word) > 0){
            curr->right = input(curr->right, word, data);
        } else {
            curr->left = input(curr->left, word, data);
        }
    }
    curr->height = max(getHeight(curr->left), getHeight(curr->right)) + 1;
    int BF = getBF(curr);
    if(BF > 1){
        if(strcmp(word, curr->left->word) > 0){
            curr->left = leftRotate(curr->left);
        }
        return rightRotate(curr);
    } else if(BF < -1) {
        if(strcmp(word, curr->left->word) < 0){
            curr->right = rightRotate(curr->right);
        }
        return leftRotate(curr);
    }
    return curr;
}

struct node *getPredecessor(node* curr){
    node *predecessor = curr->left;
    while(predecessor->right){
        predecessor = predecessor->right;
    }
    return predecessor;
}

struct node *deleteNode(node *curr, char word[]){
    if(!curr){
        return curr;
    } else if(strcmp(word, curr->word) > 0){
        curr->right = deleteNode(curr->right, word);
    } else if(strcmp(word, curr->word) < 0){
        curr->left = deleteNode(curr->left, word);
    } else {
        if(!curr->left || !curr->right){
            node *temp;

            if(curr->right)
                temp = curr->right;
            else
                temp = curr->left;

            if(!temp) {
                temp = curr;
                curr = NULL;
            } else {
                *curr = *temp;
            }

            free(temp);
        } else {
            node *temp = getPredecessor(curr);

            strcpy(curr->word, temp->word);
            curr->data = temp->data;

            curr->left = deleteNode(curr->left, temp->word);
        }
    }

    if(!curr){
        return curr;
    }

    curr->height = max(getHeight(curr->left), getHeight(curr->right)) + 1;

    int BF = getBF(curr);

    if(BF > 1){
        if(getBF(curr->left) < 0){
            curr->left = leftRotate(curr->left);
        }
        return rightRotate(curr);
    } else if(BF < -1) {
        if(getBF(curr->right) > 0){
            curr->right = rightRotate(curr->right);
        }
        return leftRotate(curr);
    }
    return curr;
}

void printBased(char lang[], node *curr){
    if(!curr) return;
    printBased(lang, curr->left);
    if(strcmp(lang, curr->data.lang) == 0){
        printf("- %s\n", curr->word);
    }
    printBased(lang, curr->right);
}

void deleteBased(char lang[], node *curr, bool deleted){
    if(!curr) return;
    deleteBased(lang, curr->left, deleted);
    if(strcmp(lang, curr->data.lang) == 0){
       strcpy(delWord, curr->word);
    }
    deleteBased(lang, curr->right, deleted);
}

void searchAvailability(char word[], node *curr){
    if(!curr) return;
    searchAvailability(word, curr->left);
    if(strcmp(word, curr->word) == 0){
       Available = true;
    }
    searchAvailability(word, curr->right);
}

void printAll(int curr){
    if(curr > totalLang){
        return;
    } else {
        langs *nodeCurr = head;
        int langCurr = 1;
        while(langCurr < curr){
            nodeCurr = nodeCurr->next;
            langCurr++;
        }
        printf("%s (%d):\n", nodeCurr->lang, nodeCurr->total);
        printBased(nodeCurr->lang, root);
        printAll(curr+1);
    }
}

void menu(){
    int l;
    scanf(" %d", &l);
    for(int i = 0; i < l; i++){
        char text[10]; scanf(" %s", text);
        printf("Case %d:\n", i+1);
        if(strcmp("ADD", text) == 0){
            extraData data;
            char word[51];
            scanf(" %s %s", data.lang, word);
            root = input(root, word, data);
            if(head == NULL){
                langs *newLang = (langs*) malloc(sizeof(langs));
                strcpy(newLang->lang, data.lang);
                newLang->total = 1;
                newLang->next = NULL;
                totalLang++;
                head = newLang;
            } else {
                langs *curr = head;
                bool same = false;
                while(!same){
                    if(strcmp(curr->lang, data.lang) == 0){
                        same = true;
                        curr->total++;
                    } else if(curr->next){ 
                        curr = curr->next;
                    } else {
                        break;
                    }
                }
                if(!same){
                    curr = head;
                    if(strcmp(curr->lang, data.lang) > 0){
                        langs *newLang = (langs*) malloc(sizeof(langs));
                        strcpy(newLang->lang, data.lang);
                        newLang->total = 1;
                        newLang->next = NULL;
                        head = newLang;
                        head->next = curr;
                    } else {
                        bool get = false;
                        while(curr->next){
                            if(strcmp(curr->next->lang, data.lang) > 0){
                                langs *newLang = (langs*) malloc(sizeof(langs));
                                strcpy(newLang->lang, data.lang);
                                newLang->total = 1;
                                newLang->next = curr->next;
                                get = true;
                                curr->next = newLang;
                                break;
                            } else {
                                curr = curr->next;
                            }
                        }
                        if(!get){
                            langs *newLang = (langs*) malloc(sizeof(langs));
                            strcpy(newLang->lang, data.lang);
                            newLang->total = 1;
                            newLang->next = NULL;
                            curr->next = newLang;
                        }
                    }
                    totalLang++;
                }
            }
            printf("Successfully Added\n");
        } else if(strcmp("SHOW-ALL", text) == 0){
            printAll(1);
        } else if(strcmp("SHOW-LANG", text) == 0){
            char lang[31];
            scanf(" %s", lang);
            langs *curr = head;
            bool same = false;
            while(!same){
                if(strcmp(curr->lang, lang) == 0){
                    printBased(lang, root);
                    same = true;
                } else if(curr->next){
                    curr = curr->next;
                } else {
                    break;
                }
            }
            if(!same){
                printf("%s language not found\n", lang);
            }
        } else if(strcmp("DEL-LANG", text) == 0){
            char lang[31];
            scanf(" %s", lang);
            langs *curr = head;
            bool same = false;
            while(!same){
                if(strcmp(curr->lang, lang) == 0){
                    while(curr->total > 0){
                        deleteBased(lang, root, false);
                        root = deleteNode(root, delWord);
                        curr->total--;
                    }
                    curr = head;
                    while(strcmp(curr->next->lang, lang) != 0){
                        curr = curr->next;
                    }
                    langs *deleteThis = curr->next;
                    curr->next = curr->next->next;
                    free(deleteThis);
                    deleteThis = NULL;
                    totalLang--;
                    printf("Successfully Deleted\n");
                    same = true;
                } else if(curr->next){
                    curr = curr->next;
                } else {
                    break;
                }
            }
            if(!same){
                printf("%s language not found\n", lang);
            }
        } else if(strcmp("DEL-WORD", text) == 0){
            Available = false;
            char word[51], lang[31];
            scanf(" %s %s", lang, word);
            searchAvailability(word, root);
            if(Available){
                bool same = false;
                langs *curr = head;
                while(!same){
                    if(strcmp(curr->lang, lang) == 0){
                        curr->total--;
                        same = true;
                    } else if(curr->next){
                        curr = curr->next;
                    } else {
                        break;
                    }
                }
                if(curr->total == 0){
                    curr = head;
                    while(strcmp(curr->next->lang, lang) != 0){
                        curr = curr->next;
                    }
                    langs *deleteThis = curr->next;
                    curr->next = curr->next->next;
                    free(deleteThis);
                    deleteThis = NULL;
                    totalLang--;
                }
                root = deleteNode(root, word);
                printf("Successfully Deleted\n");
            } else {
                printf("%s %s word not found\n", lang, word);
            }
        } 
    }
}


int main(){
    totalLang = 0;
    head = NULL;
    root = NULL;
    menu();
}
