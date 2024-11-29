#include <iostream>
#include <iomanip>
#include <cmath>
#include <fstream>
#include <locale.h>
#include <string>
using namespace std;

bool logando(){
    setlocale(LC_ALL, "pt-br");

    string username, userpass, user, pass;

    /*cout << "Escreva seu nome de usuário: ";
    cin.get();
    getline(cin, username);
    cout << "Escreva sua senha: ";
    getline(cin, userpass);
    cin.ignore();*/

    cout << "Escreva seu nome de usuário: "; cin >> username;
    cout << "Escreva sua senha: "; cin >> userpass;

    ifstream read(username + ".txt");
    getline(read, user);
    getline(read, pass);

    if(user==username && pass==userpass){
        return true;
    }else{
        return false;
    }
}

void registro(){
    setlocale(LC_ALL, "pt-br");

    string username, userpass;

    /*cout << "Escolha um nome de usuário: " << endl;
    cin.get();
    getline(cin, username);
    cout << "Escolha uma senha: " << endl;
    getline(cin, userpass);
    cin.ignore();*/

    cout << "Escolha um nome de usuário: " << endl; cin >> username;
    cout << "Escolha uma senha: " << endl; cin >> userpass;

    ofstream file;
    file.open(username+".txt");
    file << username << endl << userpass;
    file.close();
    
}

int main(){
    setlocale(LC_ALL, "pt-br");

    int escolha;
     
    cout << "Bem vindo ao sistema de avaliação!" << endl << "Para login digite 0, para nova conta digite 1" << endl;
    cin >> escolha;
    if(escolha==1){
        registro();
    }else if(escolha==0){
        bool status = logando();
        if (!status){
            cout << "Informacao incorreta, tente novamente!" << endl;
            system("PAUSE");
            return 0;
        }else{
            cout << "Sucesso no login!" << endl;
            system("PAUSE");
            return 1;
        }
    }else{
        cout << "Input invalido, tente novamente!" << endl;
    }
    

    return 0;

}