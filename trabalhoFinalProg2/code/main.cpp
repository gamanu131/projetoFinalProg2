#include <iostream>
#include <iomanip>
#include <cmath>
#include <fstream>
#include <locale.h>
#include <string>
using namespace std;

const string users_direct = "users";
int totalContents = 0;
const char *path="../users";

typedef struct {
    string mainName;
    string fullName;
    string sinopse; 
    float rating;
    int nRatings;
    string comments[5];
} content;

typedef struct {
    string nick;
    string pass; 
    int adm;
} UserInfo;

UserInfo usuario;

void saveContents() {

}

bool addContent(content Content, string mainName, string fullName, string sinopse) {
    if(mainName.size() > 15) return false;
    if(fullName.size() > 50) return false;
    if(sinopse.size() > 350) return false; 
    Content.fullName = fullName;
    Content.mainName = mainName;
    Content.sinopse = sinopse;
    return true;
}

bool logando(){ // função para fazer login dos usuários.
    setlocale(LC_ALL, "pt-br");

    string username, userpass, user, pass, adm; 
    cout << "Escreva seu nome de usuário: "; cin >> username;
    cout << "Escreva sua senha: "; cin >> userpass;

    ifstream read(username + ".txt"); //inputfilestream, lê o conteúdo dos arquivos.
    getline(read, user);    
    getline(read, pass);
    getline(read, adm);
    if(user==username && pass==userpass){
        usuario.nick = username; // coloca os valores das informações em suas respectivas lacunas nas structs. 
        usuario.pass = userpass;
        usuario.adm = stoi(adm); // transforma a string em um inteiro para validar adm. 
        return true;
    }else{
        return false;
    }
}

void registro(){ //função para registro de novos usuários.
    setlocale(LC_ALL, "pt-br");

    string username, userpass;
    int adm = 0; // primeira criação de conta, o usuário jamais será adm. (administração poderá ser setada por texto no banco de dados).
    cout << "Escolha um nome de usuário: " << endl; cin >> username;
    cout << "Escolha uma senha: " << endl; cin >> userpass;

    ofstream file; //outputfilestream, função que cria os arquivos com os dados das novas contas criadas.
    file.open(username+".txt");
    file << username << endl << userpass << endl << adm;
    file.close();   
}

int main(){
    setlocale(LC_ALL, "pt-br");

    int escolha = 0;
     
    cout << "Bem vindo ao sistema de avaliação!" << endl << "Para login digite 0, para nova conta digite 1" << endl;
    cin >> escolha;
    while(escolha != 0 && escolha != 1) {
        cout << "Input invalido, tente novamente!" << endl;
        cout << "Para login digite 0, para nova conta digite 1" << endl;
        cin >> escolha;
    }
    switch(escolha) {
        case 1:
            registro();
            break;
        case 0:
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
            break;
    }
    return 0;

}