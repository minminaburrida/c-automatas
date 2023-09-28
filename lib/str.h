#include <string>
#include <cstring>
class str{
    public:
        const char* cadena;
        int s = 0;
        int len;
};

str init(string _s){
    str _S = str();
    _S.cadena = _s.c_str();
    _S.len = strlen(_S.cadena);
    return _S;

}