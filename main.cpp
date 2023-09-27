#include<iostream>
using namespace std;
#include "lib.cpp"
#include "class.cpp"
int main(){
/*  cout<<"Te saluda el archivo nativo.\n";
    if (is_mayus('a'))cout<<"Te saluda la condicional if (is_mayus('a')) ...sorra";
    else cout<<"Te saluda el else perra\n";
    _class cl = _class();
    cl.saludar();
// 32 en hexadecimal
    int _32 = 0x20;
  
    string cc;
    getline(cin,cc);
    if (is_bs(cc[0]))cout<<"valido\n";
    */
    
    while(true){
        string num = "";
        cout<<"Que desea hacer?\n\
        1- Automata de numeros\n\
        2-Automata de variables\n\
        3-Automata de numeros float\n\
        4-Automata de palabras reservadas\n\
        5-Automata de cadenas\n\
        \t>>";
        getline(cin, num);
        switch (num[0]){
            case '0': return 0;
            case '1': automata_numero();break;
            case '2': automata_variable();break;
            case '3': automata_float();break;
            case '4': automata_keyword();break;
            case '5': automata_string();break;
        }
    }
    return 0;
}