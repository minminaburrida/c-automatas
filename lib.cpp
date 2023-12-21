#include<iostream>
#include<cstring>
#include<string>
#include"lib/automatas.hpp"
#include"lib/lexemas.hpp"
// #include"./lib/analizador/analizar.h"
void salida(string _, bool condicion){cout<<_<<" es ";if (!condicion) cout<<"in";cout<<"valido\n";}

void automata_numero()
{
    system("cls");cout<<"escribe un numero entero\n\tEjemplo: 0b01101011, 0xab32A, 43\n\n\t>>";
    string  _s = "";getline(cin, _s);salida(_s, is_int(_s));
    
}
void automata_variable()
{
    system("cls");cout<<"escribe una variable\n\tEjemplo: _evie, HolaMundo, ramires_533\n\n\t>>";
    string  _s = "";getline(cin, _s);salida(_s, is_valid_ID(_s));
    
}
void automata_float(){
    system("cls");cout<<"escribe un numero\n\tEjemplo:3.2, 3\n\n\t>>";
    string  _s = "";getline(cin, _s);salida(_s, is_float(_s));
}
void automata_keyword(){
    system("cls");cout<<"escribe una palabra reservada\n\tEjemplo: if, else, with\n\n\t>>";
    string  _s = "";getline(cin, _s);salida(_s, is_keyword(_s));
}
void automata_string(){
    system("cls");cout<<"escribe una cadena\n\tEjemplo: \"es cine pa\"\n\n\t>>";
    string  _s = "";getline(cin, _s);salida(_s, is_string(_s));
}


void analisis(){read_file();}
void leer_archivo(){
    analisis();
}
void lester(char val){lexu(val);}

    // Probably deprecated LOL
    
    // // string _file = read();
    // // cout<<_file<<endl;
    // vector<Token> tokens = analizar(read());
    // for (const Token& token : tokens) {
    //     std::cout << "Token: " << token.valor << std::endl;
    //     std::cout << "Línea: " << token.linea << std::endl;
    //     std::cout << "Posición de inicio: " << token.caracteres.inicio << std::endl;
    //     std::cout << "Posición de fin: " << token.caracteres.fin << std::endl;
    //     std::cout << "Tipo: " << token.tipo << std::endl;
    //     std::cout << std::endl; // Dos saltos de línea entre tokens
    // }
// const char* t[5] = {"00", "_32", "My var", "MyVar", "myVar"};
// const char* t[5] = {"0x32", "32", "0b0100010", "0basd", "0x32AB"};


