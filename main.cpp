#include<iostream>
using namespace std;
#include "lib.cpp"
int main(){
    while(true){
        string num = "";
        cout<<"Que desea hacer?\n\
        1- Automata de numeros\n\
        2-Automata de variables\n\
        3-Automata de numeros float\n\
        4-Automata de palabras reservadas\n\
        5-Automata de cadenas\n\
        6-Leer archivo\n\
        \t>>";
        getline(cin, num);
        switch (num[0]){
            case '0': return 0;
            case '1': automata_numero();break;
            case '2': automata_variable();break;
            case '3': automata_float();break;
            case '4': automata_keyword();break;
            case '5': automata_string();break;
            case '6': leer_archivo();break;
            default: analisis();break;
        }
    }
    return 0;
}