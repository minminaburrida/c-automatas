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

void read_file(){

}
