#include <stdio.h>
#include <stdlib.h>

// --- Structures ---

// Node for storing NFA transitions
struct Node {
    int to;
    struct Node* next;
};

// A DFA state (set of NFA states)
struct DFAState {
    int states[20]; // states[i] == 1 means NFA state q_i is included
};

// --- Global Variables ---

int numStates, numAlphabets, numFinalStates, numTransitions;
int startState;
char alphabet[20];
int finalStates[20];
struct Node* transition[20][20] = {NULL}; // NFA transitions table
struct DFAState dfaStates[50];            // all discovered DFA states
int dfaCount = 0;                         // number of DFA states found

// --- Helper Functions ---

int findAlphabetIndex(char c) {
    for (int i = 0; i < numAlphabets; i++)
        if (alphabet[i] == c) return i;
    return -1;
}

void addTransition(int from, char symbol, int to) {
    int idx = findAlphabetIndex(symbol);
    if (idx == -1) {
        printf("Invalid symbol: %c\n", symbol);
        exit(1);
    }
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->to = to;
    newNode->next = transition[from][idx];
    transition[from][idx] = newNode;
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

int areSame(struct DFAState a, struct DFAState b) {
    for (int i = 1; i <= numStates; i++)
        if (a.states[i] != b.states[i]) return 0;
    return 1;
}

int addDFAState(struct DFAState newState) {
    for (int i = 0; i < dfaCount; i++)
        if (areSame(dfaStates[i], newState)) return 0;
    dfaStates[dfaCount++] = newState;
    return 1;
}

// --- Main Program ---

int main() {
    // --- Input NFA ---
    printf("Enter number of alphabets: ");
    scanf("%d", &numAlphabets);

    printf("Enter the alphabets (no spaces): ");
    for (int i = 0; i < numAlphabets; i++)
        scanf(" %c", &alphabet[i]);

    printf("Enter number of NFA states: ");
    scanf("%d", &numStates);

    printf("Enter start state: ");
    scanf("%d", &startState);

    printf("Enter number of final states: ");
    scanf("%d", &numFinalStates);

    printf("Enter final states: ");
    for (int i = 0; i < numFinalStates; i++)
        scanf("%d", &finalStates[i]);

    printf("Enter number of transitions: ");
    scanf("%d", &numTransitions);

    printf("Enter transitions (from symbol to): e.g. 1 a 2\n");
    for (int i = 0; i < numTransitions; i++) {
        int from, to;
        char sym;
        scanf("%d %c %d", &from, &sym, &to);
        addTransition(from, sym, to);
    }

    printf("\n--- NFA Input Complete ---\n");

    // --- Initialize DFA ---
    struct DFAState start = {0};
    start.states[startState] = 1;
    addDFAState(start);

    // --- Subset Construction Algorithm ---
    for (int i = 0; i < dfaCount; i++) {
        struct DFAState current = dfaStates[i];

        for (int a = 0; a < numAlphabets; a++) {
            struct DFAState next = {0};

            // Collect all reachable NFA states
            for (int s = 1; s <= numStates; s++) {
                if (current.states[s]) {
                    struct Node* temp = transition[s][a];
                    while (temp != NULL) {
                        next.states[temp->to] = 1;
                        temp = temp->next;
                    }
                }
            }

            // Print transition
            printState(current);
            printf(" --%c--> ", alphabet[a]);
            int empty = 1;
            for (int k = 1; k <= numStates; k++)
                if (next.states[k]) empty = 0;
            if (empty) printf("{} (Trap)");
            else printState(next);
            printf("\n");

            // Add new DFA state
            addDFAState(next);
        }
    }

    // --- Print DFA Summary ---
    printf("\nDFA States:\n");
    for (int i = 0; i < dfaCount; i++) {
        printState(dfaStates[i]);
        printf("\t");
    }

    printf("\n\nAlphabets:\n");
    for (int i = 0; i < numAlphabets; i++)
        printf("%c\t", alphabet[i]);

    printf("\nStart State: q%d\n", startState);

    // Print DFA final states
    printf("\nFinal States of DFA:\n");
    for (int i = 0; i < dfaCount; i++) {
        for (int j = 1; j <= numStates; j++) {
            if (dfaStates[i].states[j]) {
                for (int k = 0; k < numFinalStates; k++) {
                    if (finalStates[k] == j) {
                        printState(dfaStates[i]);
                        printf("\t");
                        goto next_state;
                    }
                }
            }
        }
        next_state: ;
    }
    printf("\n");

    return 0;
}
