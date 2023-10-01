#include<string>
#include <set>
#include<cctype>
using namespace std;
// Es mayuscula
bool is_mayus(char _){return isupper(_);}
// Es minuscula
bool is_minus(char _){return islower(_);}
// Es letra valida

// Es "
bool is_q_mark(char _){return _=='"' || _==96;}
/* Es \  */
bool is_backslash(char _){return _=='\\';}

bool is_letter(char _){return is_mayus(_) || is_minus(_);}
// Cuenta como primero
        // Metan aqui los criterios necesarios
bool is_valid_firstChar(char _) {return is_letter(_) || _=='_';}

bool is_number(char _){return isdigit(_);}
bool is_hex(char _){return is_number(_) || _=='A' || _=='a' || _=='B' || _=='b' || _=='C' || _=='c' || _=='D' || _=='d'|| _=='E' || _=='e' || _=='F' || _=='f';}
bool is_bin(char _){return _=='0' || _=='1';}
bool is_dot(char _){return _=='.';}

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