#include <iostream>
#include <iomanip>
#include <cmath>
#include <fstream>
#include <locale.h>

using namespace std;

bool logando(){
    string username, userpass, user, pass;

    cout << "Escreva seu nome de usuário: "; getline(cin, username);
    cout << endl;
    cout << "Escreva sua senha: "; getline(cin, userpass);
    cout << endl;

    ifstream read(username + ".txt");
    getline(read, user);
    getline(read, pass);

    if(user == username && pass == userpass){
        return true;
    }else{
        return false;
    }
}

int registro(){
    string username, userpass;
    cout << "Escolha um nome de usuário: " << endl;
    cin.get();
    getline(cin, username);
    cout << endl;
    cout << "Escolha uma senha :" << endl;
    cin.get();
    getline(cin, userpass);
    cout << endl;

    ofstream file;
    file.open(username + ".txt");
    file << username << endl << userpass;
    file.close();

    bool status = logando();
    if(!status){
        cout << "Informação Incorreta" << endl;
        system("PAUSE");
        return 0;
    }else{
        cout << "Sucesso!" << endl;
        system("PAUSE");
        return 0;
    }
}

void primeiraTela(){
    char jatemconta;
    cout << "Bem vindo ao sistema de avaliação, você já tem uma conta? (y/n)" << endl;
    cin >> jatemconta;
    switch (jatemconta){
    case 'y':
        logando();
        break;

    case 'n':
        registro();
        break;
    default:
        primeiraTela();
    }
}

int main(){
    setlocale(LC_ALL, "portuguese");
    primeiraTela();
    

}