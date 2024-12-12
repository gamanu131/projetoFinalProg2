#include <iostream>
#include <iomanip>
#include <cmath>
#include <fstream>
#include <filesystem>
#include <locale.h>
#include <string>
#include <cassert>
#include <cstdio>

// defines.
#define MAX_CONTENT 250
#define MAX_COMMENTS 50

using namespace std;
namespace fs = filesystem;

const string users_direct = "users/";
const string contents_direct = "content/";
int totalContents = 0; 

typedef struct { //struct que armazena todas as informações do usuário.
    string mainName;
    string fullName;
    string sinopse; 
    string tipo; 
    double rating[MAX_COMMENTS];
    int nRatings;
    double rate;
    string comments[MAX_COMMENTS]; 
    int n_comments;
} content;

typedef struct { //Struct que armazena as informações de usuários
    string nick;
    string pass; 
    int adm;
} UserInfo;

UserInfo usuario;

content v_content[MAX_CONTENT];

// protótipos.

void sortReviewsRecursive(content *Content, int n);

int binary_search_recursion_name(string name, string v[], int n[], int ini, int end);

void options();

void clearChat(int n);

void showEverything();

// funções "definitivas".

void showEverything() { //Função que printa os nomes de todos os desenhos cadastrados.
    for(int i = 0; i < totalContents; i++) {
        cout << v_content[i].mainName << endl;
        cout << v_content[i].tipo << endl;
    }
}

void createDirectory() { //função que cria os diretórios para armazenamento de dados de usuários e conteúdo caso estes não existam.
    if(!fs::exists("users")) {
        fs::create_directory("users");
    }
    if(!fs::exists("content")) {
        fs::create_directory("content");
    }
}

void loadContents() { //Carrega os arquivos do conteúdo.
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
        sortReviewsRecursive(&v_content[totalContents], v_content[totalContents].nRatings);
        totalContents++;
    }
}

string formatWithComma(double number) { //Tirar o ponto e colocar uma vírgula para salvar a nota no padrão brasileiro.
    stringstream ss;
    ss << fixed << setprecision(1) << number; // Converte o número para string com uma casa decimal
    string numStr = ss.str();
    size_t pos = numStr.find('.'); // Encontra o ponto decimal
    if (pos != string::npos) {
        numStr[pos] = ','; // Substitui o ponto por vírgula
    }
    return numStr;
}

void saveContents() { //Salva os arquivos do conteúdo. 
    for(int i = 0; i < totalContents; i++) {
        string path = contents_direct + v_content[i].mainName + ".txt";
        cout << path << endl;      
        if(fs::remove(path)) {
            ofstream file; //outputfilestream, função que cria os arquivos com os dados das novas contas criadas.
            file.open(contents_direct + v_content[i].mainName+".txt");
            file << v_content[i].mainName << endl << v_content[i].fullName << endl << v_content[i].sinopse << endl << v_content[i].tipo << endl << v_content[i].nRatings << endl;
            for(int j = 0; j < v_content[i].nRatings; j++) {
                
                file << v_content[i].comments[j] << endl;
            }
            for(int j = 0; j < v_content[i].nRatings; j++) {
                file << formatWithComma(v_content[i].rating[j]) << endl;
            }
            file.close();
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

bool rateContent(content *Content) { //Função para salvar a nota e o comentário para um conteúdo.
    cout << Content->fullName << " nota(" << Content->rate << ")" << endl;
    cout << Content->tipo << endl;
    clearChat(1);
    for(int i = 0; i < Content->nRatings; i++) {
        cout << Content->comments[i] << " nota(" << Content->rating[i] << endl;
    }
    clearChat(1);
    cout << "Digite um comentário: ";
    cin.get();
    string comment;
    getline(cin, comment);
    Content->comments[Content->nRatings] = comment;
    cout << "Digite uma nota: " << endl;
    double rate; 
    cin >> rate;
    while(rate > 10 && rate < 0.0) {
        cout << "Digite uma nota valida: ";
        cin >> rate;
    }
    Content->rating[Content->nRatings] = rate;
    Content->nRatings++;
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


void sortReviewsRecursive(content *Content, int n) { // Passa ponteiro para a struct
    if (n == 1) { // Caso base: apenas 1 elemento, já está ordenado
        return;
    }

    // Percorre o array até o penúltimo elemento
    for (int i = 0; i < n - 1; i++) {
        // Troca se o elemento atual for menor que o próximo (para ordem decrescente)
        if (Content->rating[i] < Content->rating[i + 1]) { 
            double aux = 0;
            string auxS;

            // Troca os valores das notas
            aux = Content->rating[i];
            Content->rating[i] = Content->rating[i + 1];
            Content->rating[i + 1] = aux;

            // Troca os comentários correspondentes
            auxS = Content->comments[i];
            Content->comments[i] = Content->comments[i + 1];
            Content->comments[i + 1] = auxS;
        }
    }

    // Chamada recursiva para ordenar os elementos restantes
    sortReviewsRecursive(Content, n - 1);
}


void createRatesforRank() { //Faz a média aritmética das reviews do desenho.
    for(int i = 0; i < totalContents; i++) {
        double media = 0;
        for(int j = 0; j < v_content[i].nRatings; j++) {
            media+= v_content[i].rating[j];
        }
        v_content[i].rate = (double)media/v_content[i].nRatings;
        media = 0;
    }
}

int showContentForRank() { //Mostra o rank do desenho.
    if (totalContents <= 0) {
        cout << "Nenhum conteúdo disponível para classificar." << endl;
        return -1;
    }

    // Alocação dinâmica de memória com verificação
    double *v = new(nothrow) double[totalContents];
    int *n = new(nothrow) int[totalContents];

    if (v == nullptr || n == nullptr) {
        cout << "Falha na alocação dinâmica de memória" << endl;
        delete[] v; // Libera memória caso apenas um array tenha sido alocado
        delete[] n;
        return -1;
    }

    // Copiar dados de `v_content` para os arrays auxiliares
    for (int i = 0; i < totalContents; i++) {
        v[i] = v_content[i].rate;
        n[i] = i;
    }

    // Bubble sort para ordenar os índices com base nas notas (ordem decrescente)
    for (int i = totalContents - 1; i > 0; i--) {
        for (int j = 0; j < i; j++) {
            if (v[j] < v[j + 1]) { // Troca se o valor atual for menor que o próximo
                // Troca os valores no array de notas
                double aux = v[j];
                v[j] = v[j + 1];
                v[j + 1] = aux;

                // Troca os índices correspondentes
                int auxn = n[j];
                n[j] = n[j + 1];
                n[j + 1] = auxn;
            }
        }
    }

    // Exibir o ranking baseado nos índices ordenados
    cout << "Rank dos melhores no catálogo (ordem decrescente):" << endl;
    for (int i = 0; i < totalContents; i++) {
        cout << v_content[n[i]].mainName << " nota: " << fixed << setprecision(1) << v[i] << endl;
    }

    // Liberação de memória
    delete[] v;
    delete[] n;

    return 0;
}


void showAllContents() { //Imprime a lista de todos os desenhos registrados no sistema.
    setlocale(LC_ALL, "Portuguese");
    for(int i = 0; i < totalContents; i++) {
        cout << i + 1 << " - " << v_content[i].mainName << endl;
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

void clearChat(int n) { //Limpa o espaço do terminal.
    for(int i = 0; i < n; i++) {
        cout << endl;
    }
}

int bubble_sort_strings(string name) { //Ordena os nomes dos desenhos.
    setlocale(LC_ALL, "Portuguese");
    int i = 0, j = 0; 
    string *v = new(nothrow) string[totalContents];
    int *n = new(nothrow) int[totalContents];

    if(v == nullptr || n == nullptr) {
        cout << "Falha na alocação dinâmica de memória" << endl;
        return -1;
    }

    for(int i = 0; i < totalContents; i++) {
        v[i] = v_content[i].mainName;
        n[i] = i;
    }

    for(i = totalContents - 1; i > 0; i--) {
        for(j = 0; j < i; j++) { // Alterado para evitar acesso fora do limite
            if(v[j] > v[j+1]) {
                string aux = v[j];
                int auxn = n[j];
                v[j] = v[j+1];
                v[j+1] = aux;
                n[j] = n[j+1];
                n[j+1] = auxn;
            }
        }
    }

    binary_search_recursion_name(name, v, n, 0, totalContents - 1);

    delete[] v;
    delete[] n;

    return 0;
}

void search_type_content(string type) { //Pesquisa desenhos por gênero/categoria.
    for(int i = 0; i < totalContents; i++) {
        if(v_content[i].tipo == type) {
            cout << v_content[i].mainName << " " << "(" << type << ")" << endl;
        }
    }
}

int binary_search_recursion_name(string name, string v[], int n[], int ini, int end) { //Função de busca dos nomes dos desenhos.
    if(ini > end) { 
        cout << "O conteúdo não existe!" << endl;
        return -1;
    }
    int i = (ini + end) / 2;
    if(name == v[i]) {
        cout << i+1 << " -" << v_content[n[i]].fullName << " nota("<< fixed << setprecision(1)<< v_content[n[i]].rate <<  ")" << endl;
        return n[i];
    }
    if(name < v[i]) {
        return binary_search_recursion_name(name, v, n, ini, i - 1);
    } else {
        return binary_search_recursion_name(name, v, n, i + 1, end);
    }
}

void changePass(){ //Função para mudança de senha do usuário logado.
    string senhaAtual, senhaNova;
    cout << "Digite sua senha atual: "; cin >> senhaAtual;
    while(senhaAtual != usuario.pass){
        cout << "Senha atual incorreta!" << endl;
        cout << "Digite sua senha atual novamente: "; cin >> senhaAtual;
    }
    cout << "Digite sua nova senha: "; cin >> senhaNova;
    usuario.pass = senhaNova;
    fs::remove(users_direct + usuario.nick + ".txt");
    ofstream file;
    file.open(users_direct + usuario.nick + ".txt");
    file << usuario.nick << endl << usuario.pass << endl << usuario.adm;
    file.close();
    cout << "Senha atualizada com sucesso!" << endl;  
    options();
}

void ADMINchangeUsername(){ //função para o ADM atualizar seu nome de usuário.
    string novoNick;
    cout << "Escreva o novo nick desejado: "; cin >> novoNick;
    fs::remove(users_direct + novoNick + ".txt"); //ADM tem prioridade quanto aos nicks de escolha.
    fs::remove(users_direct + usuario.nick + ".txt");
    usuario.nick = novoNick;
    ofstream file;
    file.open(users_direct + usuario.nick + ".txt");
    file << usuario.nick << endl << usuario.pass << endl << usuario.adm;
    file.close();
    system("pause");
    cout << "Nick do ADM foi atualizado!" << endl;
    options();
}

void saveUserInfo(){
    ofstream file;
    file.open(users_direct + usuario.nick + ".txt");
    file << usuario.nick << endl << usuario.pass << endl << usuario.adm;
    file.close();
}

void chooseOptions() { //Menu para selecionar uma ação para tomar dentro do programa.
    setlocale(LC_ALL, "Portuguese");
    switch(usuario.adm) {
        case 0:{
            int escolha = 0;
            cin >> escolha;
            while(escolha < 1 && escolha > 7) {
                cout << "Opção invalida! Digite uma nova opção: " << endl;
                cin >> escolha;
            }
            switch(escolha) {
                case 1: {
                    showAllContents();
                    break;
                }
                case 2: {
                    cout << "Digite o nome para pesquisa: "; 
                    string name;
                    cin.get();
                    getline(cin, name);
                    bubble_sort_strings(name); 
                    break;
                }
                case 3: {
                    cout << "Digite o um gênero para pesquisa: "; 
                    string type;
                    cin.get();
                    getline(cin, type);
                    cout << type << endl;
                    search_type_content(type); 
                    break;
                }
                case 4: {
                    showContentForRank();
                    break;
                }
                case 5: {
                    showAllContents();
                    clearChat(1);
                    cout << "Digite qual vocẽ deseja avaliar: ";
                    int escolha = 0;
                    cin >> escolha;
                    rateContent(&v_content[escolha-1]);
                    break;
                }
                case 6: { // mudar senha.
                    changePass();
                    break;
                }
                case 7: {
                    saveContents();
                    saveUserInfo();
                    exit(0);
                    break;
                }
                default:
                    cout << "Opção inválida!" << endl;
                    break;
            }
            options();
            }break;
        case 1:{
            int escolha = 0;
            cin >> escolha;
            while(escolha < 1 && escolha > 8) {
                cout << "Opção invalida! Digite uma nova opção: " << endl;
                cin >> escolha;
            }
            switch(escolha) {
                case 1: {
                    showAllContents();
                    break;
                }
                case 2: {
                    cout << "Digite o nome para pesquisa: "; 
                    string name;
                    cin.get();
                    getline(cin, name);
                    bubble_sort_strings(name); 
                    break;
                }
                case 3: {
                    cout << "Digite o um gênero para pesquisa: "; 
                    string type;
                    cin.get();
                    getline(cin, type);
                    cout << type << endl;
                    search_type_content(type); 
                    break;
                }
                case 4: {
                    showContentForRank();
                    break;
                }
                case 5: {
                    showAllContents();
                    clearChat(1);
                    cout << "Digite qual vocẽ deseja avaliar";
                    break;
                }
                case 6: { // mudar senha.
                    changePass();
                    break;
                }
                case 7: {
                    saveContents();
                    saveUserInfo();
                    exit(0);
                    break;
                }
                case 8: {
                    ADMINchangeUsername();
                    break;
                }
                default:
                    cout << "Opção inválida!" << endl;
                    break;
            }
            options();
            }break;
            
    }
}

void options() { //Lista de opções que podem ser escolhidas.
    setlocale(LC_ALL, "Portuguese");
    clearChat(5);
    switch(usuario.adm) {
        case 0: 
            cout << "Seja bem-vindo(a): " << usuario.nick << endl << "1 - Ver o catalogo [filmes/animes/desenhos]" << "\n2 - Pesquisa por nome" << "\n3 - Pesquisa por gênero" << "\n4 - Ranking dos melhores" << "\n5 - Avaliar conteudo. " << "\n6 - Mudar senha" << "\n7 - Sair" << endl;
            chooseOptions();
            break;
        case 1:
            cout << "Seja bem-vindo(a): " << usuario.nick << endl << "1 - Ver o catalogo [filmes/animes/desenhos]" << "\n2 - Pesquisa por nome" << "\n3 - Pesquisa por gênero" << "\n4 - Ranking dos melhores" << "\n5 - Avaliar conteudo. " << "\n6 - Mudar senha" << "\n7 - Sair" << "\n8 - Mudar nome (ADM only)" << endl;
            chooseOptions();
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
            options();
            break;
    }
}

int main(){ //função principal do código que contém majoritariamente chamadas de outras funções.
    setlocale(LC_ALL, "Portuguese");
    createDirectory(); //função que cria os diretórios dos desenhos e dos usuários caso eles não existam ainda.
    loadContents(); //carrega todos os arquivos .txt (conteúdos).
    createRatesforRank(); // cria o ranking de todos os conteúdos criados. 
    start(); //função de introdução.
    return 0;

}