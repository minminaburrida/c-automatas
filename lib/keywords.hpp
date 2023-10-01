#include<string>
#include <set>
#include<cctype>
using namespace std;

// Para ver si es valido \n \t \\ \" \'
set<char> types= {'n','t',92, '"', 'b', 'r'};
// Palabras reservadas
set<string> keywords= {
    "if", "switch",
    "else", "si",
    "while",
    "for",
    "with",
    "seleccionar",
    "caso",
    "verdadero",
    "falso"
    // TODO: Agreguen las keywords en español

};
bool is_keyword(string s){return keywords.find(s) != keywords.end();}
bool is_bs(char s){return types.find(s) != types.end();}