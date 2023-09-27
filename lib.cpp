#include<iostream>
#include<cstring>
#include<string>
using namespace std;
#include<cctype>
// Es numero
bool is_number(char _){return isdigit(_);}
// Es mayuscula
bool is_mayus(char _){return isupper(_);}
// Es minuscula
bool is_minus(char _){return islower(_);}
// Es letra valida
bool is_letter(char _){return is_mayus(_) || is_minus(_);}
// Cuenta como primero
        // Metan aqui los criterios necesarios
bool is_valid_firstChar(char _) {return is_letter(_) || _=='_';}

bool is_hex(char _){return is_number(_) || _=='A' || _=='a' || _=='B' || _=='b' || _=='C' || _=='c' || _=='D' || _=='d'|| _=='E' || _=='e' || _=='F' || _=='f';}

// Es un identificador
bool is_valid_ID(string _s){
    const char* _S = _s.c_str();
    int s = 0;
    int len = strlen(_S);
    if (len == 0) return false;
    for (int i = 0; i < len; i++){
        char c = _S[i];
        // cout<<"\nEstado: "<<s<<" Caracter: "<<c<<"\n";
        switch (s)
        {
        case 0:
            if (is_valid_firstChar(c)) s=1;
            else return false;
            break;
        case 1:
            if (is_valid_firstChar(c) || is_number(c)) s=1;
            else return false;
            break;
        default: return false;
        }
    }
    return s==1;
}
// Es un identificador
bool is_int(string _s){
    const char* _S = _s.c_str();
    int s = 0;
    int len = strlen(_S);
    if (len == 0) return false;
    for (int i = 0; i < len; i++){
        char c = _S[i];
        // cout<<"\nEstado: "<<s<<" caracter: "<<c<<'\n';
        // cout<<
        switch (s)
        {
        case 0:
        // si es 0 se espera un 0x o 0b
            if (is_number(c)){if (c=='0')s=2;else s=1;}            
            else return false;
            break;
        case 1:
            if (is_number(c)) s=1;
            else return false;
            break;
        case 2:
            if (c=='x' || c=='X') s=3;
            else if (c=='b' || c=='B') s=4;
            else {if (is_number(c)) s=1;
            else return false;}
            break;
        // Es hexadecimal
        case 3:
            if (is_hex(c)) s=3;else return false;break;
        case 4:
            if (c=='0' || c=='1') s=4;else return false;break;
        default: return false;
        }
    }
    return s==1 || s == 3 || s==4;
}


/*
Deprecated
void _test(const char * test[]) {
    cout<<"\n";
    for (int i = 0; i < 6; i++) {
        cout<<test[i]<<" es ";
        if (!is_int(test[i])) cout<<"in";
        // if (!is_valid_ID(test[i])) cout<<"in";
        cout<<"valido\n";
    }
}*/

void salida(string _, bool condicion){cout<<_<<" es ";if (!condicion) cout<<"in";cout<<"valido\n";}

void automata_numero()
{
    system("cls");cout<<"escribe un numero entero\n\tEjemplo: 0b01101011, 0xab32A, 43\n\n\t>>";
    string  _s = "";cin>>_s;salida(_s, is_int(_s));
    
}
void automata_variable()
{
    system("cls");cout<<"escribe una variable\n\tEjemplo: _chiludo, HolaMundo, evieChiluda43\n\n\t>>";
    string  _s = "";cin>>_s;salida(_s, is_valid_ID(_s));
    
}

// const char* t[5] = {"00", "_32", "My var", "MyVar", "myVar"};
const char* t[5] = {"0x32", "32", "0b0100010", "0basd", "0x32AB"};


