#include<cstring>
#include<string>
#include<fstream>
// #include<iostream>

std::string read() {
    std::string cadena;
    std::string __;
    std::ifstream _("./read.txt");
    // std::filesystem::current_path();
    if (!_.is_open()) {return "No se pudo abrir el archivo.\n";}
  
    while (getline(_, __)) {cadena+=__+"\n";}

    _.close();
    return cadena;
}




ifstream inFile;      // Archivo de entrada
ofstream outFile;     // Archivo de salida
int idxLine = 0;
string srcLine;       // Línea leída actualmente
int srcRow = 0;       // Número de línea actual
int srcChar = 0;      // Carácter leído actualmente

int next_is_EOL() {
    if (idxLine >= srcLine.length()) {
        return 1;
    } else {
        return 0;
    }
}

int next_is_EOF() {
    if (inFile.eof()) {
        // Ya no hay más líneas pero aún hay que asegurarse de que estamos al
        // final de la línea anterior.
        if (next_is_EOL() == 1) {
            return 1;
        } else {
            return 0;
        }
    } else {
        return 0;
    }
}

void NextLine() {
    if (inFile.eof()) {
        return;
    }
    getline(inFile, srcLine); // Lee nueva línea
    srcRow++;
    idxLine = 0; // Apunta al primer carácter
}

void NextChar() {
    idxLine++; // Pasa al siguiente carácter
    // Actualiza srcChar
    if (next_is_EOL() == 1) {
        srcChar = 0;
    } else {
        srcChar = srcLine[idxLine];
    }
}
void read_file(){
    string filename = "read.txt";
    ifstream inputFile(filename);

    if (!inputFile.is_open()) {
        cerr << "Error al abrir el archivo: " << filename << endl;
        return;
    }

    string line;
    while (getline(inputFile, line)) {
        size_t commentPos = line.find('#'); // Busca un comentario en la línea

        if (commentPos != string::npos) {
            // Si se encuentra un comentario, imprime solo el contenido antes del comentario
            cout << line.substr(0, commentPos) << endl;
        } else {
            // Si no hay comentario, imprime la línea completa
            cout << line << endl;
        }
    }

    inputFile.close();
    // return 0;
}