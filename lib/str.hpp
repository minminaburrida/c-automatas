#include<iostream>
#include <string>
// #include <cstring>
#include <vector>

struct ubicacion{int inicio; int fin;};

struct Token{
    int linea;
    ubicacion caracteres;
    string valor;
    string tipo;
};

Token tokens[] = {};

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

}std::vector<Token> analizar(std::string _s) {
    std::vector<Token> tokens;
    std::string token_actual;
    int linea = 1;
    int inicio = 0;
    bool ignore = false;

    for (char c : _s) {
        if (c == '#' && !ignore) {
            ignore = true; // Empezar a ignorar los comentarios
        } else if (c == '\n' && ignore) {
            ignore = false; // Dejar de ignorar los comentarios al final de la línea
        }

        if (!ignore) {
            if (c == ' ') {
                // Crear un nuevo token
                Token tk;
                tk.linea = linea;
                tk.caracteres.inicio = inicio;
                tk.caracteres.fin = inicio + token_actual.length() - 1;
                
                // Reemplazar '\n' con "\\n" en el valor del token
                std::string valor_token = token_actual;
                size_t found = valor_token.find("\n");
                while (found != std::string::npos) {
                    valor_token.replace(found, 1, "\\n");
                    found = valor_token.find("\n", found + 2); // Avanzar dos caracteres para evitar bucles infinitos
                }
                
                tk.valor = valor_token;
                tk.tipo = "Tipo del token"; // Define el tipo apropiado
                tokens.push_back(tk);

                // Reiniciar el token_actual y actualizar la posición de inicio
                token_actual.clear();
                inicio += token_actual.length() + 1; // +1 para avanzar después del espacio
            } else {
                token_actual += c;
            }
        }

        if (c == '\n') {
            linea++;
            inicio = 0;
        }
    }

    // Agregar el último token si no termina con un espacio
    if (!token_actual.empty()) {
        Token tk;
        tk.linea = linea;
        tk.caracteres.inicio = inicio;
        tk.caracteres.fin = inicio + token_actual.length() - 1;
        
        // Reemplazar '\n' con "\\n" en el valor del token
        std::string valor_token = token_actual;
        size_t found = valor_token.find("\n");
        while (found != std::string::npos) {
            valor_token.replace(found, 1, "\\n");
            found = valor_token.find("\n", found + 2); // Avanzar dos caracteres para evitar bucles infinitos
        }
        
        tk.valor = valor_token;
        tk.tipo = "Tipo del token"; // Define el tipo apropiado
        tokens.push_back(tk);
    }

    return tokens;
}
