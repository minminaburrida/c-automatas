#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

istringstream inStream; // Simulamos la entrada de cadena en lugar de un archivo
int idxLine = 0;
string srcLine; // Línea leída actualmente
int srcRow = 0; // Número de línea actual

int srcChar;     // Carácter leído actualmente
string srcToken; // Token actual
int srcToktyp;   // Tipo de token

int next_is_EOL()
{
    if (idxLine >= srcLine.length())
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
int next_is_EOF()
{
    if (inStream.eof())
    {
        // Ya no hay más líneas, pero aún hay que asegurarse de que estamos al
        // final de la línea anterior.
        if (next_is_EOL() == 1)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return 0;
    }
}

void NextLine()
{
    if (inStream.eof())
    {
        return;
    }
    getline(inStream, srcLine);
    srcRow++;
    idxLine = 0;
}

void NextChar()
{
    idxLine++;
    if (next_is_EOL() == 1)
    {
        srcChar = 0;
    }
    else
    {
        srcChar = srcLine[idxLine];
    }
}

bool is_mayus(int c)
{
    return isupper(c);
}

bool is_minus(int c)
{
    return islower(c);
}

bool is_number(int c)
{
    return isdigit(c);
}

void ExtractIdentifier()
{
    srcToken = "";
    srcToktyp = 2;
    int IsToken = 1;
    while (IsToken == 1)
    {
        srcToken += static_cast<char>(srcChar);
        NextChar();
        if (next_is_EOL() == 1)
        {
            return;
        }
        IsToken = is_mayus(srcChar) || is_minus(srcChar) || is_number(srcChar);
    }
}

void ExtractSpace()
{
    srcToken = "";
    srcToktyp = 1;
    int IsToken = 1;
    while (IsToken == 1)
    {
        srcToken += static_cast<char>(srcChar);
        NextChar();
        if (next_is_EOL() == 1)
        {
            return;
        }
        if (srcChar == ' ')
        {
            IsToken = 1;
        }
        else if (srcChar == '\t')
        {
            IsToken = 1;
        }
        else
        {
            IsToken = 0;
        }
    }
}

void ExtractNumber()
{
    srcToken = "";
    srcToktyp = 3;
    int IsToken = 1;
    while (IsToken == 1)
    {
        srcToken += static_cast<char>(srcChar);
        NextChar();
        if (next_is_EOL() == 1)
        {
            return;
        }
        IsToken = is_number(srcChar);
    }
}

void ExtractString()
{
    srcToken = "";
    srcToktyp = 4;
    int IsToken = 1;
    while (IsToken == 1)
    {
        srcToken += static_cast<char>(srcChar);
        NextChar();
        if (next_is_EOL() == 1)
        {
            return;
        }
        if (srcChar != '"')
        {
            IsToken = 1;
        }
        else
        {
            IsToken = 0;
        }
    }
    NextChar();
    srcToken += '"';
}

void ExtractComment()
{
    srcToken = "";
    srcToktyp = 5;
    while (next_is_EOL() == 0)
    {
        srcToken += static_cast<char>(srcChar);
        NextChar();
    }
}

int NextCharIs(const string &car)
{
    if (idxLine >= srcLine.length() - 1)
    {
        return 0;
    }
    if (srcLine[idxLine + 1] == car[0])
    {
        return 1;
    }
    return 0;
}

void ExtractSquareBrackets()
{
    srcToken = "";
    srcToktyp = 7;                          // Token de corchetes
    srcToken += static_cast<char>(srcChar); // Agrega el '[' o ']'
    NextChar();                             // Pasa al siguiente carácter
}

void ExtractRoundBrackets()
{
    srcToken = "";
    srcToktyp = 8;                          // Token de paréntesis
    srcToken += static_cast<char>(srcChar); // Agrega el '(' o ')'
    NextChar();                             // Pasa al siguiente carácter
}
void NextToken()
{
    srcToktyp = 9; // Desconocido por defecto
    if (inStream.eof())
    {
        srcToken = "";
        srcToktyp = 10; // Devuelve token EOF
        return;
    }
    if (next_is_EOL() == 1)
    {
        // Estamos al final de una línea.
        srcToken = "";
        srcToktyp = 0; // Devolvemos Fin de línea
        NextLine();    // Movemos el cursor al siguiente token.
    }
    else
    {
        srcChar = srcLine[idxLine];
        // Hay caracteres por leer en la línea
        if (is_mayus(srcChar))
        {
            ExtractIdentifier();
            return;
        }
        else if (is_minus(srcChar) || is_number(srcChar))
        {
            ExtractIdentifier();
            return;
        }
        else if (srcChar == '_')
        {
            ExtractIdentifier();
        }
        else if (srcChar == ' ')
        {
            ExtractSpace();
        }
        else if (srcChar == '\t')
        {
            ExtractSpace();
        }
        else if (srcChar == '"')
        {
            ExtractString();
        }
        else if (srcChar == '>')
        {
            srcToktyp = 6; // Operador
            NextChar();    // Pasa al siguiente
            if (srcChar == '=')
            { // Es >=
                srcToken = ">=";
                NextChar(); // Pasa al siguiente
            }
            else
            { // Es ">"
                srcToken = ">";
            }
        }
        else if (srcChar == '<')
        {
            srcToktyp = 6; // Operador
            NextChar();    // Pasa al siguiente
            if (srcChar == '=')
            { // Es <=
                srcToken = "<=";
                NextChar(); // Pasa al siguiente
            }
            else if (srcChar == '>')
            { // Es <>
                srcToken = "<>";
                NextChar(); // Pasa al siguiente
            }
            else
            { // Es "<"
                srcToken = "<";
            }
        }
        else if (srcChar == '/')
        {
            if (NextCharIs("/"))
            { // Es comentario
                ExtractComment();
            }
            else
            {
                srcToken = '/'; // Acumula
                srcToktyp = 9;  // Desconocido
                NextChar();     // Pasa al siguiente
            }
        }
        else if (srcChar == '(')
        {
            srcToken = "("; // Token de paréntesis "(" o ")"
            srcToktyp = 11; // Token de paréntesis
            NextChar();
        }
        else if (srcChar == ')')
        {
            srcToken = ")"; // Token de paréntesis "(" o ")"
            srcToktyp = 11; // Token de paréntesis
            NextChar();
        }
        else if (srcChar == '[')
        {
            srcToken = "["; // Token de paréntesis "[" o "]"
            srcToktyp = 11; // Token de paréntesis
            NextChar();
        }
        else if (srcChar == ']')
        {
            srcToken = "]"; // Token de paréntesis "[" o "]"
            srcToktyp = 11; // Token de paréntesis
            NextChar();
        }
        else
        {                                          // Cualquier otro caso
            srcToken = static_cast<char>(srcChar); // Acumula
            srcToktyp = 9;                         // Desconocido
            NextChar();                            // Pasa al siguiente
        }
    }
}
string read_file()
{
    string filename = "read.txt"; // Cambia esto al nombre de tu archivo
    ifstream _file(filename);

    if (!_file.is_open())
        return "Error al abrir el archivo " + filename;

    vector<string> programLines; // Almacena las líneas del programa
    string line;
    bool insideMultilineComment = false;

    while (getline(_file, line))
    {
        if (!insideMultilineComment)
        {
            size_t start = line.find("/*");
            if (start != string::npos)
            {
                // Se inicia un comentario multilineal
                size_t end = line.find("*/");
                if (end != string::npos)
                {
                    // El comentario se cierra en la misma línea
                    line = line.substr(0, start) + line.substr(end + 2);
                }
                else
                {
                    // El comentario se extiende a las siguientes líneas
                    insideMultilineComment = true;
                    line = line.substr(0, start);
                }
            }
        }
        else
        {
            size_t end = line.find("*/");
            if (end != string::npos)
            {
                // Se cierra un comentario multilineal
                insideMultilineComment = false;
                line = line.substr(end + 2);
            }
            else
            {
                // La línea está completamente dentro de un comentario multilineal, se ignora
                line = "";
            }
        }

        size_t hashPos = line.find('#'); // Busca comentarios con #
        if (hashPos != string::npos)
        {
            // Línea contiene un comentario con #
            line = line.substr(0, hashPos); // Añade la parte de la línea antes del #
        }

        // Si no estamos dentro de un comentario multilineal ni es un comentario con #, procesamos la línea
        if (!line.empty())
        { // Evitar líneas vacías
            programLines.push_back(line);
        }
    }

    _file.close();
    string programText;
    for (const string &programLine : programLines)
    {
        programText += programLine + '\n';
    }
    // Imprime el src sin comentarios
    // cout << programText << '\n';
    return programText;
}

bool is_comment = false;
bool in_comment = false;
// Tipos de datos:
/*
keyword: palabra reservada
variable: una variable de cualquier tipo
int: numero sin punto decimal
float: numero con punto decimal
':': tipo de variable
'(': abrir grupo
')': cerrar grupo
'[': abrir array
']': cerrar array
*/
string leer() {}

bool isOperator(char c)
{
    return (c == ':' || c == '*' || c == '(' || c == ')' || c == '[' || c == ']' || c == '=' || c == '!');
}

bool isUnderscore(char c)
{
    return (c == '_');
}

bool isLetter(char c)
{
    return std::isalpha(static_cast<unsigned char>(c)) || isUnderscore(static_cast<unsigned char>(c));
}

bool isDigit(char c)
{
    return std::isdigit(static_cast<unsigned char>(c)) || c == '.';
}

struct token
{
    int linea;
    int caracter;
    std::string tipo;
    std::string tipo_short;
    std::string valor;
};
std::vector<token> tokens;
void crearToken(std::vector<token> &tokens, int &line, int &_char, const std::string &tipo, const std::string &tipo_short, const std::string &valor)
{
    token nuevoToken;
    nuevoToken.linea = line;
    nuevoToken.caracter = _char - valor.length();
    nuevoToken.tipo = tipo;
    nuevoToken.tipo_short = tipo;
    nuevoToken.valor = valor;
    tokens.push_back(nuevoToken);
}
std::vector<std::string> palabrasReservadas = {
    "Si", "Sino", "Mientras", "Para", "Hacer", "Entero", "Decimal", "Entonces"
                                                                    "Caracter",
    "Cadena", "Retornar", "Romper", "Repetir", "Nuevo",
    "Fin", "Programa", "Imprimir", "Leer", "Hasta", "Funcion"};
string id_or_keyword(string _)
{
    for (const std::string &palabra : palabrasReservadas)
    {
        if (_ == palabra)
            return "Palabra Reservada";
    }
    return "Identificador";
}
std::vector<token> analizadorLexico()
{
    std::string programText = read_file();
    bool enCadena = false;
    bool enID = false;
    bool enNum = false;
    bool opLogico = false;
    std::string cadena;
    // Linea y caracter
    int line = 1, _char = 1;
    std::vector<token> tokens;

    for (int i = 0; i < programText.length(); ++i)
    {
        char c = programText[i];

        if (enCadena)
        {
            // Detectar fin de cadena
            if (c == '"')
            {
                if (programText[i - 1] == '\\')
                {
                    cadena += c;
                }
                else
                {
                    enCadena = false;
                    // Crear un token de cadena
                    crearToken(tokens, line, _char, "Cadena", "STR", cadena);
                    cadena.clear();
                }
            }
            else
            {
                cadena += c; // Continuar acumulando caracteres dentro de la cadena
            }
        }
        else
        {
            // Pilinsote
            switch (c)
            {
            case ' ':
                // Manejo de espacios
                if (enID)
                {
                    if (id_or_keyword(cadena) == "Palabra Reservada")
                        crearToken(tokens, line, _char, id_or_keyword(cadena), "PR", cadena);
                    else
                        crearToken(tokens, line, _char, id_or_keyword(cadena), "ID", cadena);
                    enID = false;
                    cadena.clear();
                }
                if (enNum)
                {
                    crearToken(tokens, line, _char, "Numero", "NUM", cadena);
                    enNum = false;
                    cadena.clear();
                }
                if (opLogico)
                {
                    if (cadena == "&&")
                    {
                        crearToken(tokens, line, line, "Operador logico AND", "LOP", "&&");
                    }
                    if (cadena == "||")
                    {
                        crearToken(tokens, line, line, "Operador logico OR", "LOP", "||");
                    }
                    cadena.clear();
                    opLogico = false;
                }
                break;

            case '\n':
                // Manejo de saltos de línea
                if (enID)
                {
                    if (id_or_keyword(cadena) == "Palabra Reservada")
                        crearToken(tokens, line, _char, id_or_keyword(cadena), "PR", cadena);
                    else
                        crearToken(tokens, line, _char, id_or_keyword(cadena), "ID", cadena);
                    enID = false;
                    cadena.clear();
                }
                if (enNum)
                {
                    crearToken(tokens, line, _char, "Numero", "NUM", cadena);
                    enNum = false;
                    cadena.clear();
                }
                crearToken(tokens, line, _char, "Salto de Linea", "ENDL", "\n");
                cadena.clear();
                opLogico = false; // Reiniciar la bandera de operador lógico
                line++;
                _char = 1;
                break;

            case '(':
                // Manejo de paréntesis izquierdos
                if (enID)
                {
                    if (id_or_keyword(cadena) == "Palabra Reservada")
                        crearToken(tokens, line, _char, id_or_keyword(cadena), "PR", cadena);
                    else
                        crearToken(tokens, line, _char, id_or_keyword(cadena), "ID", cadena);
                    enID = false;
                    cadena.clear();
                }
                if (enNum)
                {
                    crearToken(tokens, line, _char, "Numero", "num", cadena);
                    enNum = false;
                    cadena.clear();
                }
                crearToken(tokens, line, _char, "Inicio de grupo", "(", "(");
                break;

            case ')':
                // Manejo de paréntesis derechos
                if (enID)
                {
                    if (id_or_keyword(cadena) == "Palabra Reservada")
                        crearToken(tokens, line, _char, id_or_keyword(cadena), "PR", cadena);
                    else
                        crearToken(tokens, line, _char, id_or_keyword(cadena), "ID", cadena);
                    enID = false;
                    cadena.clear();
                }
                if (enNum)
                {
                    crearToken(tokens, line, _char, "Numero", "NUM", cadena);
                    enNum = false;
                    cadena.clear();
                }
                crearToken(tokens, line, _char, "Simbolo Fin de grupo", ")", ")");
                break;

            case '"':
                // Stream de cadena
                if (enID)
                {
                    if (id_or_keyword(cadena) == "Palabra Reservada")
                        crearToken(tokens, line, _char, id_or_keyword(cadena), "PR", cadena);
                    else
                        crearToken(tokens, line, _char, id_or_keyword(cadena), "ID", cadena);
                    enID = false;
                    cadena.clear();
                }
                if (enNum)
                {
                    crearToken(tokens, line, _char, "Numero", "NUM", cadena);
                    enNum = false;
                    cadena.clear();
                }
                enCadena = true;
                break;

            case '*':
                // Stream de cadena
                if (enID)
                {
                    if (id_or_keyword(cadena) == "Palabra Reservada")
                        crearToken(tokens, line, _char, id_or_keyword(cadena), "PR", cadena);
                    else
                        crearToken(tokens, line, _char, id_or_keyword(cadena), "ID", cadena);
                    enID = false;
                    cadena.clear();
                }
                if (enNum)
                {
                    crearToken(tokens, line, _char, "Numero", "NUM", cadena);
                    enNum = false;
                    cadena.clear();
                }
                if (!opLogico)
                    opLogico = programText[i + 1] == '=';
                if (opLogico)
                {
                    crearToken(tokens, line, _char, "Operador Multiplicacion directa", "DOP", "*=");
                    opLogico = false;
                    i++;
                }
                else if (programText[i + 1] == '*')
                {
                    crearToken(tokens, line, _char, "Operador Multiplicar al Cuadrado", "OP", "**");
                    i++;
                }
                else
                    crearToken(tokens, line, _char, "Simbolo Multiplicacion", "DOP", "*");
                break;
            case '/':
                // Stream de cadena
                if (enID)
                {
                    if (id_or_keyword(cadena) == "Palabra Reservada")
                        crearToken(tokens, line, _char, id_or_keyword(cadena), "PR", cadena);
                    else
                        crearToken(tokens, line, _char, id_or_keyword(cadena), "ID", cadena);
                    enID = false;
                    cadena.clear();
                }
                if (enNum)
                {
                    crearToken(tokens, line, _char, "Numero", "NUM", cadena);
                    enNum = false;
                    cadena.clear();
                }
                if (!opLogico)
                    opLogico = programText[i + 1] == '=';
                if (opLogico)
                {
                    crearToken(tokens, line, _char, "Operador Division directa", "DOP", "/=");
                    opLogico = false;
                    i++;
                }
                else
                    crearToken(tokens, line, _char, "Simbolo Division", "OP", "/");
                break;
            case '+':
                // Stream de cadena
                if (enID)
                {
                    if (id_or_keyword(cadena) == "Palabra Reservada")
                        crearToken(tokens, line, _char, id_or_keyword(cadena), "PR", cadena);
                    else
                        crearToken(tokens, line, _char, id_or_keyword(cadena), "ID", cadena);
                    enID = false;
                    cadena.clear();
                }
                if (enNum)
                {
                    crearToken(tokens, line, _char, "Numero", "NUM", cadena);
                    enNum = false;
                    cadena.clear();
                }
                if (!opLogico)
                    opLogico = programText[i + 1] == '=';
                if (opLogico)
                {
                    crearToken(tokens, line, _char, "Operador Suma directa", "DOP", "+=");
                    opLogico = false;
                    i++;
                }
                else if (programText[i + 1] == '+')
                {
                    crearToken(tokens, line, _char, "Operador Sumar 1", "UOP", "++");
                    i++;
                }
                else
                    crearToken(tokens, line, _char, "Simbolo Suma", "OP", "+");
                break;
            case '-':
                // Stream de cadena
                if (enID)
                {
                    if (id_or_keyword(cadena) == "Palabra Reservada")
                        crearToken(tokens, line, _char, id_or_keyword(cadena), "PR", cadena);
                    else
                        crearToken(tokens, line, _char, id_or_keyword(cadena), "ID", cadena);
                    enID = false;
                    cadena.clear();
                }
                if (enNum)
                {
                    crearToken(tokens, line, _char, "Numero", "NUM", cadena);
                    enNum = false;
                    cadena.clear();
                }
                if (!opLogico)
                    opLogico = programText[i + 1] == '=';
                if (opLogico)
                {
                    crearToken(tokens, line, _char, "Operador Resta directa", "DOP", "-=");
                    opLogico = false;
                    i++;
                }
                else if (programText[i + 1] == '-')
                {
                    crearToken(tokens, line, _char, "Operador Restar 1", "UOP", "--");
                    i++;
                }
                else
                    crearToken(tokens, line, _char, "Simbolo Resta", "OP", "-");
                break;
            case '%':
                // Stream de cadena
                if (enID)
                {
                    if (id_or_keyword(cadena) == "Palabra Reservada")
                        crearToken(tokens, line, _char, id_or_keyword(cadena), "PR", cadena);
                    else
                        crearToken(tokens, line, _char, id_or_keyword(cadena), "ID", cadena);
                    enID = false;
                    cadena.clear();
                }
                if (enNum)
                {
                    crearToken(tokens, line, _char, "Numero", "NUM", cadena);
                    enNum = false;
                    cadena.clear();
                }
                if (!opLogico)
                    opLogico = programText[i + 1] == '=';
                if (opLogico)
                {
                    crearToken(tokens, line, _char, "Operador Residuo directo", "DOP", "%=");
                    opLogico = false;
                    i++;
                }
                else
                    crearToken(tokens, line, _char, "Simbolo Residuo", "OP", "%");
                break;
            case '!':
                // Stream de cadena
                if (enID)
                {
                    if (id_or_keyword(cadena) == "Palabra Reservada")
                        crearToken(tokens, line, _char, id_or_keyword(cadena), "PR", cadena);
                    else
                        crearToken(tokens, line, _char, id_or_keyword(cadena), "ID", cadena);
                    enID = false;
                    cadena.clear();
                }
                if (enNum)
                {
                    crearToken(tokens, line, _char, "Numero", "NUM", cadena);
                    enNum = false;
                    cadena.clear();
                }
                if (!opLogico)
                    opLogico = programText[i + 1] == '=';
                if (opLogico)
                {
                    crearToken(tokens, line, _char, "Operador Logico Distinto de", "LOP", "!=");
                    opLogico = false;
                    i++;
                }
                else
                    crearToken(tokens, line, _char, "Operador logico NOT", "NOT", "!");
                break;
            case '&':
                // Stream de cadena
                if (enID)
                {
                    if (id_or_keyword(cadena) == "Palabra Reservada")
                        crearToken(tokens, line, _char, id_or_keyword(cadena), "PR", cadena);
                    else
                        crearToken(tokens, line, _char, id_or_keyword(cadena), "ID", cadena);
                    enID = false;
                    cadena.clear();
                }
                if (enNum)
                {
                    crearToken(tokens, line, _char, "Numero", "NUM", cadena);
                    enNum = false;
                    cadena.clear();
                }
                if (!opLogico)
                {
                    opLogico = (i < programText.length() - 1) && (programText[i + 1] == '&');
                }

                if (opLogico)
                {
                    crearToken(tokens, line, _char, "Operador logico AND", "LOP", "&&");
                    opLogico = false;
                    i++;
                }
                else
                {
                    crearToken(tokens, line, _char, "Simbolo AND invalido", "NLOP", "&");
                }
                break;

            case '|':
                // Stream de cadena
                if (enID)
                {
                    if (id_or_keyword(cadena) == "Palabra Reservada")
                        crearToken(tokens, line, _char, id_or_keyword(cadena), "PR", cadena);
                    else
                        crearToken(tokens, line, _char, id_or_keyword(cadena), "ID", cadena);
                    enID = false;
                    cadena.clear();
                }
                if (enNum)
                {
                    crearToken(tokens, line, _char, "Numero", "NUM", cadena);
                    enNum = false;
                    cadena.clear();
                }
                if (!opLogico)
                {
                    opLogico = (i < programText.length() - 1) && (programText[i + 1] == '|');
                }

                if (opLogico)
                {
                    crearToken(tokens, line, _char, "Operador logico OR", "LOP", "||");
                    opLogico = false;
                    i++;
                }
                else
                {
                    crearToken(tokens, line, _char, "Simbolo OR invalido", "NLOP", "|");
                }
                break;
            case '=':
                // Stream de cadena
                if (enID)
                {
                    if (id_or_keyword(cadena) == "Palabra Reservada")
                        crearToken(tokens, line, _char, id_or_keyword(cadena), "PR", cadena);
                    else
                        crearToken(tokens, line, _char, id_or_keyword(cadena), "ID", cadena);
                    enID = false;
                    cadena.clear();
                }
                if (enNum)
                {
                    crearToken(tokens, line, _char, "Numero", "NUM", cadena);
                    enNum = false;
                    cadena.clear();
                }
                if (!opLogico)
                {
                    opLogico = (i < programText.length() - 1) && (programText[i + 1] == '=');
                }

                if (opLogico)
                {
                    crearToken(tokens, line, _char, "Operador logico Igual Que", "ROP", "==");
                    opLogico = false;
                    i++;
                }
                else
                {
                    crearToken(tokens, line, _char, "Simbolo de Asignacion", "EQUAL", "=");
                }
                break;

            case ':':
                // Stream de cadena
                if (enID)
                {
                    if (id_or_keyword(cadena) == "Palabra Reservada")
                        crearToken(tokens, line, _char, id_or_keyword(cadena), "PR", cadena);
                    else
                        crearToken(tokens, line, _char, id_or_keyword(cadena), "ID", cadena);
                    enID = false;
                    cadena.clear();
                }
                if (enNum)
                {
                    crearToken(tokens, line, _char, "Numero", "NUM", cadena);
                    enNum = false;
                    cadena.clear();
                }

                crearToken(tokens, line, _char, "Simbolo para Tipado","TYPE", ":");
                break;
            case '>':
                // Stream de cadena
                if (enID)
                {
                    if (id_or_keyword(cadena) == "Palabra Reservada")
                        crearToken(tokens, line, _char, id_or_keyword(cadena), "PR", cadena);
                    else
                        crearToken(tokens, line, _char, id_or_keyword(cadena), "ID", cadena);
                    enID = false;
                    cadena.clear();
                }
                if (enNum)
                {
                    crearToken(tokens, line, _char, "Numero", "NUM", cadena);
                    enNum = false;
                    cadena.clear();
                }
                if (!opLogico)
                    opLogico = programText[i + 1] == '=';
                if (opLogico)
                {
                    crearToken(tokens, line, _char, "Operador logico Mayor Igual Que", "ROP", ">=");
                    opLogico = false;
                    i++;
                }
                else
                    crearToken(tokens, line, _char, "Simbolor Mayor Que", "ROP", ">");
                opLogico = true;
                cadena += c;
                break;

            case '<':
                // Stream de cadena
                if (enID)
                {
                    if (id_or_keyword(cadena) == "Palabra Reservada")
                        crearToken(tokens, line, _char, id_or_keyword(cadena), "PR", cadena);
                    else
                        crearToken(tokens, line, _char, id_or_keyword(cadena), "ID", cadena);
                    enID = false;
                    cadena.clear();
                }
                if (enNum)
                {
                    crearToken(tokens, line, _char, "Numero", "NUM", cadena);
                    enNum = false;
                    cadena.clear();
                }
                if (!opLogico)
                    opLogico = programText[i + 1] == '=';
                if (opLogico)
                {
                    crearToken(tokens, line, _char, "Operador Menor Igual Que", "ROP", "<=");
                    opLogico = false;
                    i++;
                }
                else
                    crearToken(tokens, line, _char, "Simbolor Menor Que", "ROP", "<");
                opLogico = true;
                cadena += c;
                break;

            default:
                if (enCadena)
                {
                    cadena += c;
                }
                else if (isLetter(c) || isUnderscore(c))
                {
                    // Stream de identificador/keyword
                    if (!enID)
                    {
                        enID = true;
                        cadena.clear();
                    }
                    cadena += c;
                }
                else if (is_number(c))
                {
                    // Stream de un número
                    if (!enNum)
                    {
                        if (c == '0')
                        {
                            // puede ser un numero hex/bin con x, o b mayus/minus
                        }
                        enNum = true;
                        cadena.clear();
                    }
                    cadena += c;
                }
                else if (c == '=')
                {
                    // Operador de igualdad "=="
                    if (programText[i + 1] == '=')
                    {
                        crearToken(tokens, line, _char, "Operador de Igualdad", "ROP", "==");
                        cadena.clear();
                        opLogico = true; // Establece la bandera para operadores lógicos
                        i++;             // Avanzar un carácter adicional para omitir el segundo "="
                    }
                    else
                    {
                        // Simbolo de asignacion "="
                        crearToken(tokens, line, _char, "Simbolo de Asignacion", "EQUAL", "=");
                    }
                }
                else
                {
                    // Si no coincide con ninguna categoría conocida, consideramos el carácter como un token separado
                    crearToken(tokens, line, _char, "Token Desconocido", "?", std::string(1, c));
                }
                break;
            }
        }
        _char++;
    }

    // Imprimir los tokens encontrados
    return tokens;
}
class AnalizadorSintactico
{
public:
    AnalizadorSintactico(const std::vector<token> &tokens) : tokens(tokens), pos(0) {}

    void analizar()
    {
        // El punto de entrada del análisis sintáctico
        while (pos < tokens.size() && tokens[pos].tipo != "Fin de Archivo")
        {
            instruccion();
        }
    }

private:
    const std::vector<token> &tokens;
    size_t pos;

    void emparejar(const std::string &tipo_esperado)
    {
        if (pos < tokens.size() && tokens[pos].tipo == tipo_esperado)
        {
            ++pos;
        }
        else
        {
            std::string error_msg = "Error de sintaxis. Se esperaba: " + tipo_esperado +
                                    ", pero se encontro: " + (pos < tokens.size() ? tokens[pos].tipo : "fin de archivo") +
                                    " en la linea " + std::to_string(tokens[pos].linea);
            throw std::runtime_error(error_msg);
        }
    }

    void instruccion()
    {
        if (tokens[pos].tipo == "Identificador")
        {
            emparejar("Identificador");
            emparejar("Simbolo de Asignacion");
            expresion();
            emparejar("Salto de Linea");
        }
        else
        {
            // Manejar otros tipos de instrucciones o errores
            throw std::runtime_error("Error de sintaxis. Se esperaba una instruccion.");
        }
    }

    void expresion()
    {
        // Asume que una expresión comienza con un término
        termino();

        // Mientras el siguiente token sea un operador, procesa otro término
        while (pos < tokens.size() && esOperador(tokens[pos].tipo))
        {
            emparejar(tokens[pos].tipo); // Operador
            termino();
        }
    }

    void termino()
    {
        if (tokens[pos].tipo == "Numero" || tokens[pos].tipo == "Identificador")
        {
            emparejar(tokens[pos].tipo);
        }
        else
        {
            throw std::runtime_error("Error de sintaxis. Se esperaba un termino.");
        }
    }

    bool esOperador(const std::string &tipo)
    {
        return tipo == "Simbolo Suma" || tipo == "Simbolo Resta" ||
               tipo == "Simbolo Multiplicacion" || tipo == "Simbolo Division";
    }
};

void analisisSintactico()
{
    cout << "En el codigo\n\"" << read_file() << "\"\n";
    std::vector<token> tokens = analizadorLexico();
    try
    {
        AnalizadorSintactico analizador(tokens);
        analizador.analizar();
        std::cout << "Analisis sintactico exitoso.\n";
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << "Error durante el analisis sintactico: " << e.what() << "\n";
    }
}

void lexu()
{
    analisisSintactico();
    // std::vector<token> tokens = analizadorLexico();
    // for (token &t : tokens)
    // {

    //     if (t.tipo == "Cadena")
    //     {
    //         t.valor = '"' + t.valor + '"';
    //     }
    //     else
    //     {
    //     }
    //     std::cout << "Tipo de token: " << t.tipo << '\n';
    //     std::cout << "Valor: ";
    //     if (t.valor == "\n")
    //         std::cout << "\\n"
    //                   << '\n';
    //     else
    //         std::cout << t.valor << "\n";
    //     std::cout << "Ubicacion: Linea " << t.linea << ":" << t.caracter << "\n\n";
    // }
}
