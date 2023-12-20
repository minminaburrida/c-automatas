// FIXME: Ciclo; For / Fixed
// FIXME: IF; Else / Fixed
// FIXME: Mensajes de error / Fixed
// FIXME: Estructura de codigo / Fixed
// FIXME: Matchear el documento con esto / Not started
// FIXME: Operacion Unaria / Fixed

// Issues: No detecta correctamente los operadores

// Historial de esta tonteria:
/*
    20 sep - la kinvelI sacó la primera version del documento
    29 sep - la ibi 🏳️‍⚧️ terminó de analizarlo y le dió feedback de como mejorar el lenguaje
    07 oct - la kinvelI & el greñas farmearon todo el documento
    12 oct - la kinvelI & el greñas se farmearon el lexico
    15 oct - la ibi 🏳️‍⚧️ corrigio las issues ocurridas en el lexico
    01 nov - la ibi 🏳️‍⚧️ se echó la sintaxis como pudo
    03 dec - la ibi 🏳️‍⚧️ terminó la sintaxis pero quedó fea, aun le siguio moviendo
    18 dec - la kinvelI ayudó con el puerco de programa (Programa <name>\n<cuerpo>\n Fin Programa)
    19 dec - el greñas arreglo la detección de numeros decimales (30.2)
    19 dec - la kinvelI arregló las sentencias unarias y operaciones directas (a++ --, a+=42)
    19 dec - la kinvelI mejoró la detección de los ;
    20 dec - la ibi 🏳️‍⚧️ implementó el ciclo for
*/

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
struct ErrorMsg
{
    int line;
    int character;
    std::string expected;
    std::string received;
    std::string errorType;
};
std::vector<token> tokens;
void crearToken(std::vector<token> &tokens, int &line, int &_char, const std::string &tipo, const std::string &tipo_short, const std::string &valor)
{
    token nuevoToken;
    nuevoToken.linea = line;
    nuevoToken.caracter = _char - valor.length();
    nuevoToken.tipo = tipo;
    nuevoToken.tipo_short = tipo_short;
    nuevoToken.valor = valor;
    tokens.push_back(nuevoToken);
}
std::vector<std::string> tiposDeDato = {"Entero", "Decimal", "Cadena", "Bit", "Caracter"};
std::vector<std::string> palabrasReservadas = {"Si", "Sino", "Mientras", "Para", "Hacer", "Entonces", "Fin",
                                               "Retornar", "Romper", "Repetir", "Nuevo", "Fin", "Programa", "Imprimir", "Leer", "Hasta", "Funcion"};
string id_or_keyword(string _)
{
    for (const std::string &palabra : tiposDeDato)
    {
        if (_ == palabra)
            return "Palabra Reservada";
    }
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
    bool enDecimal = false;
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
                    crearToken(tokens, line, _char, enDecimal ? "Numero Decimal" : "Numero", "NUM", cadena);
                    enNum = false;
                    cadena.clear();
                }
                if (opLogico)
                {
                    if (cadena == "&&")
                    {
                        crearToken(tokens, line, _char, "Operador logico AND", "LOP", "&&");
                    }
                    if (cadena == "||")
                    {
                        crearToken(tokens, line, _char, "Operador logico OR", "LOP", "||");
                    }
                    cadena.clear();
                    opLogico = false;
                }
                break;
            case ';':
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
                        crearToken(tokens, line, _char, "Operador logico AND", "LOP", "&&");
                    }
                    if (cadena == "||")
                    {
                        crearToken(tokens, line, _char, "Operador logico OR", "LOP", "||");
                    }
                    cadena.clear();
                    opLogico = false;
                }
                crearToken(tokens, line, _char, "Separacion", "ENDS", ";");
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

                crearToken(tokens, line, _char, "Simbolo para Tipado", "TYPE", ":");
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
                else if (is_number(c) || (c == '.' && is_number(programText[i + 1])))
                {
                    if (!enNum)
                    {
                        if (c == '0')
                        {
                            // puede ser un numero hex/bin con x, o b mayus/minus
                        }
                        enNum = true;
                        enDecimal = false;
                        cadena.clear();
                    }

                    if (c == '.')
                    {
                        if (enDecimal)
                        {
                            crearToken(tokens, line, _char, "Error: Número Decimal Inválido", "ERROR", cadena);
                            enNum = false;
                            cadena.clear();
                        }
                        else
                        {
                            enDecimal = true;
                            cadena += c;
                        }
                    }
                    else
                    {
                        cadena += c;
                    }
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
        // Verificar el inicio del programa
        if (tokens[pos].valor == "Programa")
        {
            _emparejar("PR");
            _emparejar("ID");
            _emparejar("ENDL");
        }
        else
        {
            throw ErrorMsg{tokens[pos].linea, tokens[pos].caracter, "Programa",
                           pos < tokens.size() ? tokens[pos].tipo : "fin de archivo",
                           "Cuerpo de programa no valido"};
        }

        // Analizar instrucciones dentro del programa
        while (pos < tokens.size() && tokens[pos].valor != "Fin" && tokens[pos + 1].valor != "Programa")
        {
            instruccion();
            _emparejar("ENDL");
        }

        // Verificar el final del programa
        if (tokens[pos].valor == "Fin" && tokens[pos + 1].valor == "Programa")
        {
            _emparejar("PR");
            _emparejar("PR");
            _emparejar("ENDL");
        }
        else
        {
            throw ErrorMsg{tokens[pos].linea, tokens[pos].caracter, "Fin Programa",
                           pos < tokens.size() ? tokens[pos].tipo : "fin de archivo",
                           "Fin de programa no valido"};
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
            throw ErrorMsg{tokens[pos].linea, tokens[pos].caracter, tipo_esperado,
                           pos < tokens.size() ? tokens[pos].tipo : "fin de archivo",
                           "Error de sintaxis"};
        }
    }
    void _emparejar(const std::string &tipo_esperado)
    {
        cout << "\n_" << tokens[pos].valor << " " << tokens[pos].tipo_short;
        if (pos < tokens.size() && tokens[pos].tipo_short == tipo_esperado)
        {
            ++pos;
        }
        else
        {
            throw ErrorMsg{tokens[pos].linea, tokens[pos].caracter, tipo_esperado,
                           pos < tokens.size() ? tokens[pos].tipo : "fin de archivo",
                           "Error de sintaxis"};
        }
    }
    void match(const std::string &tipo_esperado, const std::string &valor)
    {
        cout << "\n_" << tokens[pos].valor << " " << tokens[pos].tipo_short;
        if (pos < tokens.size() && tokens[pos].tipo_short == tipo_esperado && tokens[pos].valor == valor)
        {
            ++pos;
        }
        else
        {
            throw ErrorMsg{tokens[pos].linea, tokens[pos].caracter, valor,
                           pos < tokens.size() ? tokens[pos].tipo : "fin de archivo",
                           "Error de sintaxis"};
        }
    }
    void instruccion()
    {
        // string _instruccion
        // Si es Palabra Reservada (Keyword)
        if (tokens[pos].tipo_short == "PR")
        {
            if (tokens[pos].valor == "Si")
            {

                match("PR", "Si");
                _emparejar("(");
                expresion(true);
                _emparejar(")");

                if (tokens[pos].valor == "Entonces")
                    match("PR", "Entonces");
                _emparejar("ENDL");

                // Bloque de código para el Si
                while (tokens[pos].valor != "Fin" && tokens[pos].valor != "Sino")
                {
                    instruccion();
                    _emparejar("ENDL");
                }

                // Sino
                if (tokens[pos].valor == "Sino")
                {
                    match("PR", "Sino");
                    _emparejar("ENDL");

                    // Bloque de código para el Sino
                    while (tokens[pos].valor != "Fin")
                    {
                        instruccion();
                        _emparejar("ENDL");
                    }
                }

                // Fin Si
                match("PR", "Fin");
                match("PR", "Si");
            }

            // Senencia Leer
            else if (tokens[pos].valor == "Leer")
            {
                match("PR", "Leer");
                _emparejar("(");
                _emparejar("ID");
                _emparejar(")");
            }
            // Senencia Imprimir
            else if (tokens[pos].valor == "Imprimir")
            {
                match("PR", "Imprimir");
                _emparejar("(");
                expresion(true);
                _emparejar(")");
            }
            else if (tokens[pos].valor == "Para")
            {
                match("PR", "Para");
                _emparejar("(");
                asignacionFor();
                expresion();
                _emparejar("ENDS");
                instruccion();
                _emparejar(")");
                _emparejar("ENDL");

                while (tokens[pos].valor != "Fin")
                {
                    instruccion();
                    _emparejar("ENDL");
                }

                // Fin CabiPara
                match("PR", "Fin");
                match("PR", "Para");
            }

            else
            {
            }
            // _emparejar("ENDL");
        }
        // Si es Identificador (Variable/ID)
        else if (tokens[pos].tipo_short == "ID")
        {

            asignacion();
            // _emparejar("ENDL");
        }
        else if (tokens[pos].tipo_short == "ENDL")
            emparejar("ENDL");
        else
        {
            // Manejar otros tipos de instrucciones o errores
            std::cerr << "Error en el índice " << pos << ": ";
            throw ErrorMsg{tokens[pos].linea, tokens[pos].caracter, "Un token valido",
                           pos < tokens.size() ? tokens[pos].tipo : "fin de archivo",
                           "Token no valido"};
        }
    }
    void asignacionFor()
    {
        asignacion();       // Procesa la asignación
        _emparejar("ENDS"); // Asegúrate de manejar el punto y coma
    }

    void asignacion()
    {
        _emparejar("ID");

        if (tokens[pos].tipo_short == "UOP")
        {
            // Unary operation (a++, a--)
            _emparejar("UOP");
            return;
        }
        if (tokens[pos].tipo_short == "DOP")
        {
            // Unary operation (a++, a--)
            _emparejar("DOP");
            expresion();
            return;
        }
        else if (tokens[pos].tipo_short == "EQUAL")
        {
            // Direct assignment (a = <valor>)
            _emparejar("EQUAL");
            expresion();
        }
        else if (tokens[pos].tipo_short == "PR" || tokens[pos].tipo_short == "TYPE")
        {
            // Declaration (declaracion(a:<Tipo de dato> = <valor>))
            _emparejar("PR");
            _emparejar("(");
            _emparejar("ID");
            _emparejar("TYPE");
            if (tipoDeDato(tokens[pos].valor))
            {
                _emparejar(tokens[pos].tipo_short);
            }
            else
            {
                std::cerr << "Error en el índice " << pos << ": ";
                throw ErrorMsg{tokens[pos].linea, tokens[pos].caracter, "Un tipo de dato",
                               pos < tokens.size() ? tokens[pos].tipo : "fin de archivo",
                               "Tipo de dato no valido"};
            }
            _emparejar("EQUAL");
            valorAsignado();
            _emparejar(")");
        }
        // else if (tokens[pos].valor == "funcion")
        // {
        //     // Function call (funcion(a(<parametros separados por comas>)))
        //     _emparejar("PR");
        //     _emparejar("(");
        //     // Procesar llamada a función
        //     procesarLlamadaFuncion();
        //     _emparejar(")");
        // }
        else
        {
            // Handle other cases or raise an error if needed
            std::cerr << "Error en el índice " << pos << ": ";
            throw ErrorMsg{tokens[pos].linea, tokens[pos].caracter, "una asignacion",
                           pos < tokens.size() ? tokens[pos].tipo : "fin de archivo",
                           "Asignacion no reconocida"};
        }
    }

    void expresion(bool cierre = false)
    {
        // Asume que una expresión comienza con un término
        valorAsignado(cierre);

        // Mientras el siguiente token sea un operador lógico o punto y coma, procesa otro término
        while (pos < tokens.size() && (esOperadorLogico(tokens[pos].tipo) || tokens[pos].tipo_short == "ENDS") && tokens[pos].tipo_short != "ENDL")
        {
            if (tokens[pos].tipo_short == "ENDS" && !cierre)
            {
                // Procesar punto y coma
                // _emparejar("ENDS");
                break;
            }

            emparejar(tokens[pos].tipo); // Operador lógico
            valorAsignado(cierre);
        }
    }

    bool tipoDeDato(string _)
    {
        for (const std::string &palabra : tiposDeDato)
        {
            if (_ == palabra)
                return true;
        }
        return false;
    }
    /*


    */
    bool esOperador(const std::string &tipo)
    {
        return tipo == "OP" ||  // Operadores aritméticos y otros
               tipo == "ROP" || // Operadores de relación/comparación
               tipo == "LOP" || // Operadores lógicos
               tipo == "DOP";   // Operadores de asignación directa (p.ej., +=, -=)
        // Agrega aquí otros tipos de operadores según sea necesario
    }
    bool esOperando(const std::string &tipo)
    {
        return tipo == "NUM" || // Números
               tipo == "ID" ||  // Identificadores (nombres de variables)
               tipo == "STR";   // Cadenas

        // Agrega aquí otros tipos de operandos según sea necesario
    }
    /*

    }
    */
    void valorAsignado(bool cierre = false)
    {
        int grupos = cierre ? 1 : 0;

        while (((grupos >= 0 && !cierre) || (grupos != 0 && cierre)) && tokens[pos].tipo_short != "ENDL")
        {
            if (tokens[pos].tipo_short == "(")
            {
                grupos++;
                _emparejar("(");
            }
            else if (tokens[pos].tipo_short == ")")
            {
                grupos--;
                _emparejar(")");
                if (grupos == 0 && cierre)
                {
                    --pos; // Decrementa la posición si estamos manejando un cierre extra
                    break;
                }
            }
            else if (tokens[pos].tipo_short == "ENDS" && !cierre)
            {
                // Procesar punto y coma
                // _emparejar("ENDS");
                break;
            }
            else if (esOperador(tokens[pos].tipo_short) || esOperando(tokens[pos].tipo_short))
            {
                _emparejar(tokens[pos].tipo_short);
            }
            else
            {
                throw ErrorMsg{tokens[pos].linea, tokens[pos].caracter, "una asignacion",
                               pos < tokens.size() ? tokens[pos].tipo : "fin de archivo",
                               "Token Inesperado"};
            }
        }

        if (grupos != 0)
        {
            throw ErrorMsg{tokens[pos].linea, tokens[pos].caracter, ")",
                           pos < tokens.size() ? tokens[pos].tipo : "fin de archivo",
                           "Parentesis no balanceados"};
        }
    }

    bool esOperadorLogico(const std::string &tipo)
    {
        return tipo == "Operador Comparacion" || tipo == "Operador logico Igual Que" ||
               tipo == "Operador logico Diferente de" || tipo == "Operador logico AND";
        /* Otros operadores lógicos */
    }

    void termino()
    {
        if (tokens[pos].tipo_short == "NUM" || tokens[pos].tipo_short == "ID" || tokens[pos].tipo == "Operador Comparacion")
        {
            cout << "\n_" << tokens[pos].valor << " " << tokens[pos].tipo_short;
            emparejar(tokens[pos].tipo);
        }
        else
        {
            throw ErrorMsg{tokens[pos].linea, tokens[pos].caracter, "un Termino",
                           pos < tokens.size() ? tokens[pos].tipo : "fin de archivo",
                           "Termino no valido"};
        }
    }
};

void analisisSintactico(const std::vector<token> &tokens)
{
    std::cout << "En el codigo\n\"" << read_file() << "\"\n";
    try
    {
        AnalizadorSintactico analizador(tokens);
        analizador.analizar();
        std::cout << "\nAnalisis sintactico exitoso.\n";
    }
    catch (const ErrorMsg &e)
    {
        std::cerr << "\n\nError durante el analisis:\nLinea " << e.line
                  << "\nCaracter " << e.character
                  << "\nTipo de error: " << e.errorType
                  << "\nSe esperaba: " << (e.expected == "ENDL" ? "Salto de Linea" : e.expected)
                  << "\npero se encontro: " << e.received << "\n";
    }
}
void lexu(char value)
{
    std::vector<token> tokens = analizadorLexico();
    if (value == '1')

        for (token &t : tokens)
        {
            if (t.tipo == "Cadena")
            {
                t.valor = '"' + t.valor + '"';
            }
            else
            {
            }
            std::cout << "Tipo de token: " << t.tipo << '\n';
            std::cout << "Valor: ";
            if (t.valor == "\n")
                std::cout << "\\n"
                          << '\n';
            else
                std::cout << t.valor << "\n";
            std::cout << "Ubicacion: Linea " << t.linea << ":" << t.caracter << "\n\n";
        }
    else
        analisisSintactico(tokens);
    // std::vector<token> tokens = analizadorLexico();
    //
}
