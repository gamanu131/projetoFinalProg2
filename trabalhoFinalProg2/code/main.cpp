#include <iostream>
#include <iomanip>
#include <cmath>
#include <fstream>
#include <filesystem>
#include <locale.h>
#include <string>

// defines.
#define MAX_CONTENT 9999

using namespace std;
namespace fs = std::filesystem;

const string users_direct = "users/";
const string contents_direct = "content/";
int totalContents = 0; 

typedef struct {
    string mainName;
    string fullName;
    string sinopse; 
    string tipo; 
    double rating[5];
    int nRatings;
    string comments[5]; 
    int n_comments;
} content;

typedef struct {
    string nick;
    string pass; 
    int adm;
} UserInfo;

UserInfo usuario;

content v_content[MAX_CONTENT];

// protótipos.

void sortReviewsIterative(content Content);

void sortReviewsRecursive(content Content, int n);

int binary_search_recursion_name(string name, string v[], int n[], int ini, int end);

void options();

// funções "definitivas".

void createDirectory() { //função que cria os diretórios para armazenamento de dados de usuários e conteúdo caso estes não existam.
    if(!fs::exists("users")) {
        fs::create_directory("users");
    }
    if(!fs::exists("content")) {
        fs::create_directory("content");
    }
}

void loadContents() { // estou implementando
    string rating, nrating, nn;
    for(const auto& entry : fs::directory_iterator("content")) {
        if(entry.is_regular_file()) {
            ifstream read(entry.path());
            getline(read, v_content[totalContents].mainName);
            getline(read, v_content[totalContents].fullName); 
            getline(read, v_content[totalContents].sinopse);  
            getline(read, v_content[totalContents].tipo);  
            getline(read, nrating);
            v_content[totalContents].nRatings = stoi(nrating);
            for(int i = 0; i < v_content[totalContents].nRatings; i++) {
                getline(read, v_content[totalContents].comments[i]);
            }
            for(int i = 0; i < v_content[totalContents].nRatings; i++) {
                string a;
                getline(read, a);
                v_content[totalContents].rating[i] = stof(a);
            }
        }
        sortReviewsIterative(v_content[totalContents]);
        totalContents++;
    }
}

void saveContents() {
    for(int i = 0; i < totalContents; i++) {
        if(fs::remove(contents_direct + v_content[i].mainName)) {
            /*ofstream file; //outputfilestream, função que cria os arquivos com os dados das novas contas criadas.
            file.open(contents_direct + v_content[i].mainName+".txt");
            file << username << endl << userpass << endl << adm;
            file.close(); */ 
        }     
    }
}

bool addContent(content Content, string mainName, string fullName, string sinopse){ //Adicionar um desenho à database.
    if(mainName.size() > 15) return false;
    if(fullName.size() > 50) return false;
    if(sinopse.size() > 350) return false; 
    Content.fullName = fullName;
    Content.mainName = mainName;
    Content.sinopse = sinopse;
    return true;
}

bool login(){ // função para fazer login dos usuários.
    setlocale(LC_ALL, "Portuguese");

    string username, userpass, user, pass, adm; 
    cout << "Escreva seu nome de usuário: "; cin >> username;
    cout << "Escreva sua senha: "; cin >> userpass;

    ifstream read(users_direct + username + ".txt"); //inputfilestream, lê o conteúdo dos arquivos.
    getline(read, user);    
    getline(read, pass);
    getline(read, adm);
    if(user==username && pass==userpass){
        usuario.nick = username; // coloca os valores das informações em suas respectivas lacunas nas structs. 
        usuario.pass = userpass;
        usuario.adm = stoi(adm); // transforma a string em um inteiro para validar adm. 
        return true;
    }
    return false;
}

void registerAccount(){ //função para registro de novos usuários.
    setlocale(LC_ALL, "Portuguese");

    string username, userpass;
    int adm = 0; // primeira criação de conta, o usuário jamais será adm. (administração poderá ser setada por texto no banco de dados).
    cout << "Escolha um nome de usuário: " << endl; cin >> username;
    cout << "Escolha uma senha: " << endl; cin >> userpass;

    while(fs::exists(username+".txt")){
        cout << "Um usuário com esse nome já existe! Tente novamente!" << endl;
        cout << "Escolha um nome de usuário: " << endl; cin >> username;
        cout << "Escolha uma senha: " << endl; cin >> userpass;  
    }

    ofstream file; //outputfilestream, função que cria os arquivos com os dados das novas contas criadas.
    file.open(users_direct + username+".txt");
    file << username << endl << userpass << endl << adm;
    file.close();
    cout << "Conta criada com sucesso!" << endl;
}   

void sortReviewsIterative(content Content){ //função que recebe as reviews de um desenho e as ordena de menor para maior.
    for (int i=0; i<5-1; i++) { //bubblesort iterativo para ordenar as reviews (notas e comentarios).
        for (int j=0; j<5-i-1; j++) {
            if (Content.rating[j] > Content.rating[j+1]) {

                int aux = 0; string auxS;

                aux = Content.rating[i]; // ordenando os valores.
                Content.rating[i] = Content.rating[j+1];
                Content.rating[j+1] = aux;

                auxS = Content.comments[i]; // ordenando as strings.
                Content.comments[i] = Content.comments[j+1];
                Content.comments[j+1] = auxS;
            }
        }
    }
}

void sortReviewsRecursive(content Content, int n){ //bubblesort recursivo para ordenar as reviews (notas e comentarios).
                                             
    if(n==1){ // Se o tamanho do array fica 1, a ordenação foi concluída.
        return; 
    }

    for (int i = 0; i < n - 1; i++) {
        if (Content.rating[i] > Content.rating[i + 1]) {
            
            int aux = 0; string auxS;            

            aux = Content.rating[i]; // ordenando os valores.
            Content.rating[i] = Content.rating[i + 1];
            Content.rating[i + 1] = aux;

            auxS = Content.comments[i]; // ordenando as strings.
            Content.comments[i] = Content.comments[i + 1];
            Content.comments[i + 1] = auxS;
        }
    }

    sortReviewsRecursive(Content, n-1);  // Chamada recursiva com tamanho reduzido.
}

void showAllContents() { //Imprime a lista de todos os desenhos registrados no sistema.
    setlocale(LC_ALL, "Portuguese");
    for(int i = 0; i < totalContents; i++) {
        cout << i + 1 << v_content[i].mainName << endl;
    }
}

void swapString(string *a, string *b) { //Muda os valores de duas strings entre si.
    string aux;
    aux = *a;
    *a = *b;
    *b = aux;
}

void swapDouble(double *a, double *b) { //Muda os valores de dois doubles entre si.
    double aux;
    aux = *a;
    *a = *b;
    *b = aux;
}

void swapInt(int *a, int *b) {  //Muda os valores de dois inteiros entre si.
    int aux;
    aux = *a;
    *a = *b;
    *b = aux;
}

int bubble_sort_strings(string name) {
    setlocale(LC_ALL, "Portuguese");
    int i = 0, j = 0; 
    string *v = new(nothrow) string[totalContents];
    int *n = new(nothrow) int[totalContents];
    if(v == nullptr) {
        cout << "Falha na alocação dinâmica de memória" << endl;
        return -1;
    }
    if(n == nullptr) {
        cout << "Falha na alocação dinâmica de memória" << endl;
        return -1;
    }
    for(int i = 0; i < totalContents; i++) {
        v[i] = v_content[i].mainName;
        n[i] = i;
    }
    for(i = totalContents - 1; i > 0; i--) {
        for(j = 0; j < totalContents; j++) {
            if(v[j] > v[j+1]) {
                string aux; int auxn = 0;
                aux = v[j];
                v[i] = v[j+1];
                v[j+1] = aux;
                aux = n[j];
                n[j] = n[j+1];
                n[j+1] = auxn;
            }
        }
    }
    binary_search_recursion_name(name, v, n, 0, totalContents - 1);
    delete[] v;
    v = nullptr;
    delete[] n;
    n = nullptr;
}

int binary_search_recursion_name(string name, string v[], int n[], int ini, int end) { //Pesquisa dos desenhos por nome.
    if(ini > end) { 
        return -1;
    }
    int i = (ini + end)/2;
    if(name == v[i]) {
        cout << "O conteúdo solicitado possui índice: " << i << endl;
        return i;
    }
    if(name < v[i]) {
        binary_search_recursion_name(name, v, n, ini, i - 1);
    } else {
        binary_search_recursion_name(name, v, n, i + 1, end);
    }
    return 0;
}

void chooseOptions() {
    setlocale(LC_ALL, "Portuguese");
    switch(usuario.adm) {
        case 0:{
            int escolha = 0;
            cin >> escolha;
            while(escolha < 1 && escolha > 6) {
                cout << "Opção invalida! Digite uma nova opção: " << endl;
                cin >> escolha;
            }
            switch(escolha) {
                case 1: 
                    showAllContents();
                    break;
                case 2:
                    break;
            }
            options();
            }break;
        case 1:{
            }break;
            
    }
}

void options() {
    setlocale(LC_ALL, "Portuguese");
    switch(usuario.adm) {
        case 0: 
            cout << "Seja bem-vindo(a): " << usuario.nick << endl << "1 - Ver o catalogo [filmes/animes/desenhos]" << "2 - Pesquisa por nome" << "3 - Pesquisa por gênero" << "4 - Ranking dos melhores animes" << "5 - Mudar senha" << "6 - Sair" << endl;
            chooseOptions();
            break;
        case 1:
            break;
    }
}

void start() { //Introdução ao sistema e escolha inicial de login ou registro de conta.
    setlocale(LC_ALL, "Portuguese");
    int escolha = 0;
    cout << "Bem vindo ao sistema de avaliação!" << endl << "Digite as seguintes opções: \n0 - LOGIN\n1 - REGISTRO" << endl;
    cin >> escolha;
    while(escolha != 0 && escolha != 1){
        cout << "Input invalido, tente novamente!" << endl;
        cout << "Bem vindo ao sistema de avaliação!" << endl << "Digite as seguintes opções: \n0 - LOGIN\n1 - REGISTRO" << endl;
        cin >> escolha;
    }
    switch(escolha) {
        case 1:
            registerAccount();
            break;
        case 0:
            bool status = login();
            while(!status){
                cout << "Informacao incorreta, tente novamente!" << endl;
                status = login();
            }
            cout << "Sucesso no login!" << endl;
            break;
    }
}

int main(){ //função principal do código que contém majoritariamente chamadas de outras funções.
    setlocale(LC_ALL, "Portuguese");
    createDirectory(); //função que cria os diretórios dos desenhos e dos usuários caso eles não existam ainda.
    loadContents(); //
    bubble_sort_strings("Re:Zero");
    start(); //função de introdução.
    return 0;

}