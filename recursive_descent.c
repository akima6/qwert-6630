#include <stdio.h>
#include <stdlib.h>

struct Node {
    int to;
    struct Node* next;
};

struct DFAState {
    int states[20];
};

struct Node* transition[20][20] = {NULL};
struct DFAState dfaStates[20];
char alphabet[20];
int numStates, numAlphabets, startState, finalStates[20], numFinals;
int dfaCount = 0;

int findAlphabetIndex(char c) {
    for (int i = 0; i < numAlphabets; i++)
        if (alphabet[i] == c) return i;
    return -1;
}

void addTransition(int from, char symbol, int to) {
    int idx = findAlphabetIndex(symbol);
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->to = to;
    newNode->next = transition[from][idx];
    transition[from][idx] = newNode;
}

int areSame(struct DFAState a, struct DFAState b) {
    for (int i = 1; i <= numStates; i++)
        if (a.states[i] != b.states[i]) return 0;
    return 1;
}

void printState(struct DFAState st) {
    printf("{");
    int first = 1;
    for (int i = 1; i <= numStates; i++) {
        if (st.states[i]) {
            if (!first) printf(",");
            printf("q%d", i);
            first = 0;
        }
    }
    printf("}");
}

int main() {
    int n, from, to;
    char sym;

    printf("Enter number of alphabets: ");
    scanf("%d", &numAlphabets);
    printf("Enter alphabets: ");
    for (int i = 0; i < numAlphabets; i++) scanf(" %c", &alphabet[i]);

    printf("Enter number of states: ");
    scanf("%d", &numStates);

    printf("Enter start state: ");
    scanf("%d", &startState);

    printf("Enter number of final states: ");
    scanf("%d", &numFinals);
    printf("Enter final states: ");
    for (int i = 0; i < numFinals; i++) scanf("%d", &finalStates[i]);

    printf("Enter number of transitions: ");
    scanf("%d", &n);
    printf("Enter transitions (from symbol to):\n");
    for (int i = 0; i < n; i++) {
        scanf("%d %c %d", &from, &sym, &to);
        addTransition(from, sym, to);
    }

    // Initial DFA state (only the NFA start)
    struct DFAState start = {0};
    start.states[startState] = 1;
    dfaStates[dfaCount++] = start;

    // Subset construction
    for (int i = 0; i < dfaCount; i++) {
        struct DFAState current = dfaStates[i];

        for (int k = 0; k < numAlphabets; k++) {
            struct DFAState next = {0};
            for (int s = 1; s <= numStates; s++) {
                if (current.states[s]) {
                    struct Node* temp = transition[s][k];
                    while (temp) {
                        next.states[temp->to] = 1;
                        temp = temp->next;
                    }
                }
            }

            if (!areSame(next, (struct DFAState){0})) {
                int exists = 0;
                for (int t = 0; t < dfaCount; t++)
                    if (areSame(dfaStates[t], next)) exists = 1;

                if (!exists) dfaStates[dfaCount++] = next;

                // ✅ Print only the transition summary
                printState(current);
                printf(" --%c--> ", alphabet[k]);
                printState(next);
                printf("\n");
            }
        }
    }

    printf("\nDFA States:\n");
    for (int i = 0; i < dfaCount; i++) {
        printState(dfaStates[i]);
        printf("\n");
    }

    printf("\nStart state: q%d\n", startState);

    printf("Final states: ");
    for (int i = 0; i < dfaCount; i++) {
        for (int f = 0; f < numFinals; f++) {
            for (int s = 1; s <= numStates; s++) {
                if (dfaStates[i].states[s] && s == finalStates[f]) {
                    printState(dfaStates[i]);
                    printf(" ");
                    break;
                }
            }
        }
    }
    printf("\n");
}
    
