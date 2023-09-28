#include<iostream>
#include<cstring>
bool is_number(char _){return isdigit(_);}
bool is_hex(char _){return is_number(_) || _=='A' || _=='a' || _=='B' || _=='b' || _=='C' || _=='c' || _=='D' || _=='d'|| _=='E' || _=='e' || _=='F' || _=='f';}
bool is_bin(char _){return _=='0' || _=='1';}
bool is_dot(char _){return _=='.';}
