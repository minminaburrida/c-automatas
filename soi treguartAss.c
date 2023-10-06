#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <limits.h>
#include <math.h>

//DEFINICION DE MACROS
//La macro NELEMS, se utiliza para calcular el número de elementos en un arreglo (array) 
#define NELEMS(arr) (sizeof(arr) / sizeof(arr[0]))

/* Permite declarar una variable tipo puntero del tipo especificado y también declarar dos variables  enteras
 adicionales que se utilizan para realizar un seguimiento de la posición y el valor máximo, las variables con
  _p y _max se realiza para evitar colisiones de nombres en diferentes contextos donde se utilice esta macro.*/
#define da_dim(name, type)  type *name = NULL;          \
                            int _qy_ ## name ## _p = 0;  \
                            int _qy_ ## name ## _max = 0

/* Esta macro se utiliza para restablecer la posición de seguimiento de alguna variable previamente declarado
 utilizando la macro da_dim */
#define da_rewind(name)     _qy_ ## name ## _p = 0

/* Esta macro se utiliza para redimensionar un arreglo dinámico que ya ha sido previamente declarado y que se
 gestiona a través de un conjunto de variables de seguimiento */
#define da_redim(name)      do {if (_qy_ ## name ## _p >= _qy_ ## name ## _max) \
                                name = realloc(name, (_qy_ ## name ## _max += 32) * sizeof(name[0]));} while (0)

/* Esta macro se utiliza para agregar un elemento al final de un arreglo dinámico que ya ha sido declarado y 
 gestionado a través de un conjunto de variables de seguimiento.  */
#define da_append(name, x)  do {da_redim(name); name[_qy_ ## name ## _p++] = x;} while (0)

/* Esta macro se utiliza para obtener la longitud actual de un arreglo dinámico que ha sido previamente declarado
 y gestionado a través de un conjunto de variables de seguimiento */
#define da_len(name)        _qy_ ## name ## _p

/* Esta estructura se utiliza para representar tokens en el analizador léxico, que escanea el código fuente y lo divide
 en unidades más pequeñas llamadas tokens */
typedef enum {
    tk_EOI, tk_Mul, tk_Div, tk_Mod, tk_Add, tk_Sub, tk_Negate, tk_Not, tk_Lss, tk_Leq,
    tk_Gtr, tk_Geq, tk_Eq, tk_Neq, tk_Assign, tk_And, tk_Or, tk_If, tk_Else, tk_While,
    tk_Begin, tk_End, tk_Print, tk_Putc, tk_Lparen, tk_Rparen, tk_Lbrace, tk_Rbrace,
    tk_Semi, tk_Comma, tk_Ident, tk_Integer, tk_String, tk_Float, tk_Repeat, tk_Do,
} TokenType;

/* la estructura tok_s se utiliza para representar tokens en el analizador léxico, proporcionando 
 información sobre el tipo del token, su ubicación en el código fuente y su valor asociado */
typedef struct {
    TokenType tok;
    int err_ln, err_col;
    union {
        int n;                  /* para enteros */
        double f;        // para números flotantes
        char *text;             /* para identificadores */
    };
} tok_s;

/* la estructura TokenNode se utiliza para implementar una lista enlazada de tokens en el análisis de 
código fuente, lo que facilita la manipulación y el procesamiento de los tokens. Cada nodo de la lista 
contiene un token y un enlace al siguiente nodo en la secuencia.*/
typedef struct TokenNode {
    tok_s token;
    struct TokenNode* next;
} TokenNode;

/* Esta estructura TokenList se utiliza para mantener información sobre una lista enlazada de  tokens o elementos */
typedef struct TokenList {
    TokenNode* head;
    TokenNode* tail;
} TokenList;

/* función addToLinkedList tiene como objetivo agregar un nuevo nodo a una lista enlazada de tokens (TokenNode) 
y devolver la nueva cabeza de la lista actualizada. */
TokenNode* addToLinkedList(TokenNode* head, tok_s token) {
    TokenNode* newNode = (TokenNode*)malloc(sizeof(TokenNode));
    if (newNode == NULL) {
        fprintf(stderr, "Error: No se pudo asignar memoria para el nodo.\n");
        exit(1);
    }

    newNode->token = token;
    newNode->next = NULL;

    if (head == NULL) {
        // Si la lista está vacía, el nuevo nodo se convierte en la cabeza.
        return newNode;
    } else {
        // Si la lista no está vacía, agregamos el nuevo nodo al final.
        TokenNode* current = head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
        return head;
    }
}
/* definir e inicializar variables y punteros que se utilizarán en el programa */
 
 //se declaran dos punteros a objetos de tipo FILE, source_fp y dest_fp, como variables estáticas
static FILE *source_fp, *dest_fp; 
 //se declaran e inicializan tres variables enteras estáticas: line, col, y the_ch. Se utilizan para realizar un seguimiento de la posición actual en el código fuente que se está analizando

static int line = 1, col = 0, the_ch = ' ';

//define una macro llamada da_dim que se utiliza para declarar y dimensionar dinámicamente un array de caracteres llamado text
da_dim(text, char); 

//declara una función llamada gettok que se utiliza para obtener y devolver un token (tok_s).
tok_s gettok();

/* función error se utiliza para imprimir mensajes de error formateados, incluyendo información sobre la 
ubicación del error en el código fuente */
static void error(int err_line, int err_col, const char *fmt, ... ) {
    char buf[1000];
    va_list ap;

    va_start(ap, fmt);
    vsprintf(buf, fmt, ap);
    va_end(ap);
    printf("(%d,%d) error: %s\n", err_line, err_col, buf);
    exit(1);
}

/* unción next_ch se utiliza para obtener el siguiente caracter del flujo de entrada (archivo de origen)
 en un programa */
static int next_ch() {     
    the_ch = getc(source_fp);
    ++col;
    if (the_ch == '\n') {
        ++line;
        col = 0;
    }
    return the_ch;
}

/* analizar y procesar constantes de caracteres, teniendo en cuenta las secuencias de escape válidas y mostrando
 mensajes de error en caso de que se encuentren constantes de caracteres mal formadas */
static tok_s char_lit(int n, int err_line, int err_col) {   /* 'x' */
    if (the_ch == '\'')
        error(err_line, err_col, "gettok: constante de caracter vacio");
    if (the_ch == '\\') {
        next_ch();
        if (the_ch == 'n')
            n = 10;
        else if (the_ch == '\\')
            n = '\\';
        else error(err_line, err_col, "gettok: secuencia de escape desconocida \\%c", the_ch);
    }
    if (next_ch() != '\'')
        error(err_line, err_col, "contstante multi-caracter");
    next_ch();
    return (tok_s){tk_Integer, err_line, err_col, {n}};
}

/* La función div_or_cmt se encarga de procesar la barra diagonal (/) en el código fuente, ya sea como operador 
de división o como el inicio de un comentario  */
static tok_s div_or_cmt(int err_line, int err_col) { /* procesa dividir o comentario */
    if (the_ch != '*')
        return (tok_s){tk_Div, err_line, err_col, {0}};

    /* comentario encontrado */
    next_ch();
    for (;;) {
        if (the_ch == '*') {
            if (next_ch() == '/') {
                next_ch();
                return gettok();
            }
        } else if (the_ch == EOF)
            error(err_line, err_col, "EOF en comentario");
        else
            next_ch();
    }
}

/* La función string_lit se encarga de procesar literales de cadena en el código fuente, 
secuencias de caracteres encerradas entre comillas dobles ("  */
static tok_s string_lit(int start, int err_line, int err_col) { /* "st" */
    da_rewind(text);

    while (next_ch() != start) {
        if (the_ch == '\n') error(err_line, err_col, "EOL en cadena");
        if (the_ch == EOF)  error(err_line, err_col, "EOF en cadena");
        da_append(text, (char)the_ch);
    }
    da_append(text, '\0');

    next_ch();
    return (tok_s){tk_String, err_line, err_col, {.text=text}};
}

//static int kwd_cmp(const void *p1, const void *p2) {
//    return strcmp(*(char **)p1, *(char **)p2);
//}

/* esta función identifica si un identificador es una palabra reservada (como "si", "sino", "imprimir", etc.) 
o un identificador regular y devuelve el tipo de token correspondiente */
static TokenType get_ident_type(const char *ident) {
    static struct {
        const char *s;
        TokenType sym;
    } kwds[] = {
        {"sino",  tk_Else},
        {"si",    tk_If},
        {"imprimir", tk_Print},
        {"leer",  tk_Putc},
        {"mientras", tk_While},
        {"inicio", tk_Begin},
        {"fin",   tk_End},
        {"repetir", tk_Repeat}, 
        {"hacer", tk_Do},       
    }, *kwp;

    for (kwp = kwds; kwp < kwds + NELEMS(kwds); ++kwp) {
        if (strcmp(ident, kwp->s) == 0) {
            return kwp->sym;
        }
    }

    return tk_Ident; 
}

/* Esta función se encarga de reconocer y clasificar tokens que pueden ser identificadores 
(variables o palabras clave) o números enteros/números flotantes en el código fuente */
static tok_s ident_or_int(int err_line, int err_col) {
    int is_number = true;
    int is_float = false;  // Variable para indicar si es un número flotante

    /* para analizar caracteres en un flujo de entrada y construir una cadena "text" que representa 
    un identificador, número o parte de uno. Durante este proceso, se pueden establecer las variables 
    is_number y is_float para ayudar a determinar si lo que se está analizando es un número y si es un número 
    decimal o flotante. */
    da_rewind(text);
    while (isalnum(the_ch) || the_ch == '_' || the_ch == '.') {
        da_append(text, (char)the_ch);
        if (!isdigit(the_ch) && the_ch != '.')
            is_number = false;
        if (the_ch == '.')
            is_float = true;
        next_ch();
    }
    
    /* para manejar situaciones en las que no se ha reconocido un caracter válido durante el proceso de análisis */
    if (da_len(text) == 0)
        error(err_line, err_col, "gettok: caracter no reconocido (%d) '%c'\n", the_ch, the_ch);
    da_append(text, '\0');
    
    /* se utiliza para determinar si la cadena text representa un número (ya sea entero o flotante) y, en caso 
    afirmativo, convertirlo al tipo de token correspondiente */
    if (is_number) {
        if (is_float) {
            double f = strtod(text, NULL);
            if (f == HUGE_VAL)
                error(err_line, err_col, "Número flotante fuera de rango");
            return (tok_s){tk_Float, err_line, err_col, {.f = f}};
        } else {
            int n = strtol(text, NULL, 0);
            if (n == LONG_MAX && errno == ERANGE)
                error(err_line, err_col, "Número entero fuera de rango");
            return (tok_s){tk_Integer, err_line, err_col, {.n = n}};
        }
    }
    
    return (tok_s){get_ident_type(text), err_line, err_col, {.text=text}};
}

/* La función follow tiene la función de verificar el siguiente caracter en el código fuente 
y tomar una decisión basada en si coincide con el carácter esperado "expect" */
static tok_s follow(int expect, TokenType ifyes, TokenType ifno, int err_line, int err_col) {   /* ver adelante para el prox caracter '>=', etc. */
    if (the_ch == expect) {
        next_ch();
        return (tok_s){ifyes, err_line, err_col, {0}};
    }
    if (ifno == tk_EOI)
        error(err_line, err_col, " follow: caracter no reconocido '%c' (%d)\n", the_ch, the_ch);
    return (tok_s){ifno, err_line, err_col, {0}};
}

/* La función gettok tiene la función de analizar el código fuente para identificar y retornar el tipo de 
token correspondiente al siguiente elemento léxico encontrado */
tok_s gettok() {            /* regresa el tipo de token */
    /* saltar espacio en blanco */
    while (isspace(the_ch))
        next_ch();
    int err_line = line;
    int err_col  = col;
    switch (the_ch) {
        case '{':  next_ch(); return (tok_s){tk_Lbrace, err_line, err_col, {0}};
        case '}':  next_ch(); return (tok_s){tk_Rbrace, err_line, err_col, {0}};
        case '(':  next_ch(); return (tok_s){tk_Lparen, err_line, err_col, {0}};
        case ')':  next_ch(); return (tok_s){tk_Rparen, err_line, err_col, {0}};
        case '+':  next_ch(); return (tok_s){tk_Add, err_line, err_col, {0}};
        case '-':  next_ch(); return (tok_s){tk_Sub, err_line, err_col, {0}};
        case '*':  next_ch(); return (tok_s){tk_Mul, err_line, err_col, {0}};
        case '%':  next_ch(); return (tok_s){tk_Mod, err_line, err_col, {0}};
        case ';':  next_ch(); return (tok_s){tk_Semi, err_line, err_col, {0}};
        case ',':  next_ch(); return (tok_s){tk_Comma,err_line, err_col, {0}};
        case '/':  next_ch(); return div_or_cmt(err_line, err_col);
        case '\'': next_ch(); return char_lit(the_ch, err_line, err_col);
        case '<':  next_ch(); return follow('=', tk_Leq, tk_Lss,    err_line, err_col);
        case '>':  next_ch(); return follow('=', tk_Geq, tk_Gtr,    err_line, err_col);
        case '=':  next_ch(); return follow('=', tk_Eq,  tk_Assign, err_line, err_col);
        case '!':  next_ch(); return follow('=', tk_Neq, tk_Not,    err_line, err_col);
        case '&':  next_ch(); return follow('&', tk_And, tk_EOI,    err_line, err_col);
        case '|':  next_ch(); return follow('|', tk_Or,  tk_EOI,    err_line, err_col);
        case '"' : return string_lit(the_ch, err_line, err_col);
        default:   return ident_or_int(err_line, err_col);
        case EOF:  return (tok_s){tk_EOI, err_line, err_col, {0}};
    }
}

/* función run se encarga de analizar el código fuente y obtener tokens, imprimir información sobre los 
tokens en la consola y agregar los tokens a una lista enlazada TokenList */
void run(TokenList *tokenList) {  // Modificar la firma de la función
    tok_s tok;
    do {
        tok = gettok();

        // Imprime los tokens en la consola
        printf("(%d,%d) %.15s",
                tok.err_ln, tok.err_col,
                &"Fin_de_entrada  Op_multiplicar  Op_dividi       Op_mod          Op_sumar        "
                 "Op_restar       Op_negar        Op_not          Op_menorQue     Op_menorIgual   "
                 "Op_mayorque     Op_mayorIgual   Op_igual        Op_diferente    Op_asignacion   "
                 "Op_and          Op_or           PalaReser_si    PalaReser_sino  PalaReser_mient "
                 "PalaRese_inici  PalaRese_fin    PalaRese_impr   PalaRese_leer   AbrirParen      "
                 "CerrarParen     AbrirLlave      CerrarLlave     Puntocoma       Coma            "
                 "Identificador   Entero          Cadena          Num_flotante    "
                [tok.tok * 16]);
        if (tok.tok == tk_Integer)     printf("  %4d",   tok.n);
        else if (tok.tok == tk_Float)  printf(" %.2f", tok.f); 
        else if (tok.tok == tk_Ident)  printf(" %s",     tok.text);
        else if (tok.tok == tk_String) printf(" \"%s\"", tok.text);
        printf("\n");


        // Agrega el token a la lista enlazada
        tokenList->head = addToLinkedList(tokenList->head, tok);
    } while (tok.tok != tk_EOI);
    
}

/* La función init_io se utiliza para inicializar un puntero a un archivo (FILE) y abrir un 
archivo en un modo específico */
void init_io(FILE **fp, const char mode[], const char fn[]) {
    if ((*fp = fopen(fn, mode)) == NULL) {
        perror("No se puede abrir el archivo");
        exit(1);
    }
}

/* funcion main procesa un archivo de código fuente y crea una lista enlazada de tokens a partir
 de ese código fuente. */
int main(int argc, char* argv[]) {
    init_io(&source_fp, "r", "code.txt"); // Abre el archivo code.txt

    if (argc > 1) {
        init_io(&dest_fp, "wb", argv[1]);
    } else {
        dest_fp = stdout;
    }

    TokenList tokenList;  // Define la estructura TokenList en el main
    tokenList.head = NULL;
    tokenList.tail = NULL;

    run(&tokenList);  // Pasa la estructura TokenList como argumento

    // Ahora puedes procesar la lista enlazada de tokens según tus necesidades
    TokenNode* current = tokenList.head;
    while (current != NULL) {
        // Procesar el token actual, por ejemplo, imprimirlo
        //printf("Token: %d\n", current->token.tok);

        // Avanzar al siguiente nodo
        current = current->next;
    }

    fclose(source_fp);
    if (dest_fp != stdout) {
        fclose(dest_fp);
    }

    system("pause");
    return 0;
}