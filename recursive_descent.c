#include <stdio.h>
#include <ctype.h>
#include <string.h>

void E();
void T();
void Eprime();
void Tprime();
void check();

char expr[10];
int flag,count;

void main(){
    flag=0;
    count=0;
    printf("enter exprression");
    scanf("%s",expr);

    E();

    if (count==strlen(expr) && flag==0)
        printf("valid");
    else
        printf("not valid");
}

void E(){
    T();
    Eprime();
}

void T(){
    check();
    Tprime();
}

void Eprime(){
    if (expr[count]=='+'){
            count++;
            T();
            Eprime();
    }
}

void Tprime(){
    if (expr[count]=='*'){
            count++;
            check();
            Tprime();
    }
}

void check(){
    if (isalnum(expr[count]))
        count++;
    
    else if (expr[count]=='('){
        count++;
        E();
        if (expr[count]==')')
            count++;
        else
            flag=1;
    }
    else
        flag=1;
}