#include "cosos rancios.hpp"
#include "str.hpp"

bool is_int(string _s){
    str item = init(_s);
    int s = 0;
    if (item.len == 0) return false;
    for (int i = 0; i < item.len; i++){
        char c = item.cadena[i];
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
        // Es binario
        case 4:
            if (is_bin(c)) s=4;else return false;break;
        default: return false;
        }
    }
    return s==1 || s == 3 || s==4;
}
// Es un identificador
bool is_valid_ID(string _s){
    str item = init(_s);
    int s = 0;
    if (item.len == 0) return false;
    for (int i = 0; i < item.len; i++){
        char c = item.cadena[i];
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

bool is_string(string _s){
    str item = init(_s);
    int s = 0;
    cout<<"\nLongitud: "<<item.len<<endl;
    if (item.len == 0) return false;
    for (int i = 0; i < item.len; i++){
        char c = item.cadena[i];
        cout<<"Estado actual: "<<s<<endl;
        cout<<"Caracter: "<<c<<"\nEstado nuevo: "<<s<<endl;
        switch (s){
            case 0:
                if (is_q_mark(c)) s=1;
                else return false;
                break;
            case 1:
                if (is_backslash(c)) s=2;
                else if (is_q_mark(c)) s=3;
                else s=1;
                // se vale cualquier caracter
                break;
            case 2:
                if (is_bs(c)) s=1;
                else return false;
                break;
            case 3: 
                if (c==' ') s=3;
                break;
            default: return false;
        }
        
    }
    return s==3;
}
bool is_float(string _s){
    str item = init(_s);
    int s = 0;
    if (item.len == 0) return false;
    // Si es entero tambien cuenta
    if (is_int(_s)) return true;
    for (int i = 0; i < item.len; i++){
        char c = item.cadena[i];
        switch (s){
            case 0:
                if (is_dot(c)) s=2;
                else if (is_number(c)) s=1;
                else return false;
                break;
            case 1:
                if (is_number(c)) s=1;
                else if (is_dot(c)) s=2;
                else return false;
                break;
            case 2:
                if (is_number(c)) s=1;
                else return false;
                break;
            default: return false;
        }
    }
    return s==1;   
}
