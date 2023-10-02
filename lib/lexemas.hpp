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
    return (c == ':' || c == '*' || c == '(' || c == ')' || c == '[' || c == ']');
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
string tipo_id(string s){
      if (is_keyword(s)) return "Palabra Reservada";
      return "Identificador";
}
void lexu()
{

    /*
    TODO:
        quiero que diga por cada token:
    tipo de token: <toktype> // meter una condicional si entra en palabras reservadas (las que vienen ahi + algunas que se puedan meter) 
    valor: <valor> // valor absoluto del token (escribir, 3, "soy vegetta")
    ubicacion: <linea>:<numero de caracter> // Ubicacion exacta del token (23:33)
    \n\n (2 saltos de linea)

    */
    std::string programText = read_file();
    bool inIdentifier = false;
    bool inStringLiteral = false;
    bool inNumber = false;
    bool escapeNextChar = false;
    // Linea y caracter
    int line=1, _char=1,token=1;
    std::string currentString;

    for (char c : programText)
    {
        if (c == '\n' && !inStringLiteral)
        {
            // Detectar salto de línea
            std::cout << "\nSalto de linea";
            inIdentifier = false;
            inStringLiteral = false;
            inNumber = false;
            _char=1;line++;

        }
        else if (c == '"' && !inStringLiteral && !inNumber)
        {
            // Inicio de cadena
            inStringLiteral = true;
            currentString.clear();
            std::cout << "\n\nToken #"<<token<<"\nTipo: Cadena\nUbicacion: Linea"<<line<<", Caracter"<<_char<<"\nValor: \"";
            token++;
        }
        else if (c == '"' && inStringLiteral && !escapeNextChar)
        {
            // Fin de cadena
            inStringLiteral = false;
            std::cout << currentString << "\"";
            currentString.clear();
        }
        else if (inStringLiteral)
        {
            // Construir cadena
            if (c == '\\' && !escapeNextChar)
            {
                escapeNextChar = true;
            }
            else
            {
                currentString += c;
                escapeNextChar = false;
            }
        }
        else if (isOperator(c))
        {
            // Detectar operadores
            if (inIdentifier)
            {
                inIdentifier = false;
            }
            std::cout << "\n\nToken #"<<token<<"\nTipo: Operador\nUbicacion: Linea"<<line<<", Caracter"<<_char<<"\nValor: " << c;
            token++;
        }
        else if (isLetter(c) || (inIdentifier && isUnderscore(c)))
        {
            // Detectar identificadores
            if (!inIdentifier)
            {
                std::cout << "\n\nToken #"<<token<<"\nTipo: " << tipo_id(currentString) << "\nUbicacion: Linea "<<line<<", Caracter "<<_char<<"\nValor: ";
                token++;
                std::cout << c;
                inIdentifier = true;
            }
            else
            {
                std::cout << c;
            }
        }
        else if (isDigit(c) && !inStringLiteral)
        {
            // Detectar números enteros y decimales
            if (!inNumber)
            {
                std::cout << "\n\nToken #"<<token<<"\nTipo: Numero\nUbicacion: Linea"<<line<<", Caracter"<<_char<<"\nValor: ";
                token++;
                std::cout << c;
                inNumber = true;
            }
            else
            {
                std::cout << c;
            }
        }
        else
        {
            inIdentifier = false;
            inStringLiteral = false;
            inNumber = false;
        }
        _char++;
    }
}