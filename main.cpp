#include<iostream>
using namespace std;
#include "lib.cpp"
#include "class.cpp"

int main(){
    cout<<"Te saluda el archivo nativo.\n";
    if (is_mayus('a'))cout<<"Te saluda la condicional if (is_mayus('a')) ...sorra";
    else cout<<"Te saluda el else perra\n";
    _class cl = _class();
    cl.saludar();
// 32 en hexadecimal
    int _32 = 0x20;
    while(true){
        string num = "";
        cout<<"Que desea hacer?\n1- Automata de numeros\n2-Automata de variables\n\t>>";
        cin>>num;
        switch (num[0]){
            case '0': return 0;
            case '1': automata_numero();break;
            case '2': automata_variable();break;

        }
    }
    return 0;
}