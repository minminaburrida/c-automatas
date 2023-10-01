#include<string>


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

