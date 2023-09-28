#include "Produtos.h"
#include "Estoque.h"
#include "Medicamento.h"
#include "Beleza.h"
#include "HigienePessoal.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>

using namespace std;

// Funcao para dividir uma string em substrings com base em um caractere separador
vector<string> Split(string linha, char divisor) {
    vector<string> info; // Vetor para armazenar as substrings resultantes (uma informação)
    string str; // Variável temporária para construir cada substring
    size_t inicio = 0; // Variável para rastrear o início da próxima substring
    float nota;

    // Percorre a string de entrada caractere por caractere
    for (int i = 0; i < linha.size(); ++i) {

        // Verifica se o caractere atual é igual ao caractere separador
        if (linha[i] == divisor) {
            // Se sim, cria uma substring a partir do último ponto de partida (start) até o caractere atual
            str = linha.substr(inicio, i - inicio);
            // Adiciona a substring ao vetor de dados
            info.push_back(str);
            // Atualiza o ponto de partida para o próximo caractere
            inicio = i + 1;
        }
    }

    // Adiciona a última parte da string (após o último separador) ao vetor de dados
    str = linha.substr(inicio);
    info.push_back(str);

    // Retorna o vetor de substrings resultantes
    return info;
}

void IniciarPrograma(Estoque *estoque){
    ifstream arquivo;
    Produtos *prod;

    arquivo.open("arquivo.txt");

    if(!arquivo.is_open()){
        cout << "Não há produtos no estoque, por favor inserir novos produtos.\n";
        arquivo.close();
        return;
    }

    while(!arquivo.eof()){
        string linha;

        getline(arquivo, linha);
        vector<string> info = Split(linha, ';');

        if (info.empty()) {
            continue; // Ignorar linhas vazias
        }

        int categoria = stoi(info[7]);

        Data validade = Data(stoi(info[4]), stoi(info[5]), stoi(info[6]));

        switch(categoria){
             case 1:{
                prod = new Medicamento(info[0], stof(info[1]), info[2], stoi(info[3]),
                                       validade, categoria, info[8],
                                       info[9], info[10] == "Tem prescrição." ? true : false, info[11]);
                break;
             }

            case 2:{
                prod = new Beleza(info[0], stof(info[1]), info[2], stoi(info[3]),
                                  validade, categoria, info[8],
                                  info[9], info[10]);
                break;
            }

            case 3:{
                prod = new HigienePessoal(info[0], stof(info[1]), info[2], stoi(info[3]),
                                          validade, categoria, info[8],
                                          info[9], info[10]);
                break;
            }

            default: {
                cout << "Tipo de produto desconhecido: " << categoria << endl;
                continue; // Ignorar produtos desconhecidos
            }
        }
        estoque->inserirProduto(prod);
    }
    arquivo.close();
}

void Menu(){
    cout << "\nEscolha uma opção:\n"
            "1- Inserir produto.\n"
            "2- Listar todos os produtos.\n"
            "3- Exibir um produto (pesquisar por nome).\n"
            "4- Alterar um produto (pesquisar por nome).\n"
            "5- Remover um produto (pesquisar por nome).\n"
            "6- Exibir relatório.\n"
            "7- Sair e salvar no arquivo.\n";
}

void MenuAgain() {
    cout << "\nPressione [ENTER] para voltar ao menu principal.";
    string str;
    getline(cin, str);
}

void InserirProduto(Estoque *estoque){
    system("cls");

    cout << "Selecione a categoria do produto que deseja inserir:\n"
          "1 - Medicamento.\n"
          "2 - Itens de beleza.\n"
          "3 - Higiene pessoal.\n";

    int categoria;

    cin >> categoria;
    cin.ignore();

    Produtos *prod = nullptr;

    switch (categoria){
        case 1:{
            prod = new Medicamento();
            break;
        }

        case 2:{
            prod = new Beleza();
            break;
        }

        case 3:{
            prod = new HigienePessoal();
            break;
        }

        default:{
            cout << "Essa categoria não existe no sistema. Digite outra: ";
            cin >> categoria;
            cin.ignore();
        }
    }
    prod->setCategoriaProduto(categoria);
    prod->lerAtributos();
    estoque->inserirProduto(prod);
}

void EncerrarPrograma(Estoque *estoque){
    ofstream arquivo;

    arquivo.open("arquivo.txt");

    if(!arquivo.is_open()){
        cout << "Não foi possível abrir o arquivo.";
        arquivo.close();
        return;
    }

    vector<Produtos *> entraEstoque = estoque->getProdutos();

    for(size_t i = 0; i < entraEstoque.size(); i++){
        arquivo << entraEstoque[i]->toString();

        if(i != entraEstoque.size() - 1){
            arquivo << endl;
        }
    }
    cout << "Programa encerrado. Os dados foram salvos com sucesso. Obrigado por usar o sistema da Farmácia Pague++!\n";
    arquivo.close();
}

int main(){
    Estoque *estoque = new Estoque();
    int escolha;

    cout << "Bem-vindo ao sistema da Farmácia Pague++! ";

    IniciarPrograma(estoque);

    while(1){
        Menu(); //especificação 1
        cin >> escolha;
        cin.ignore();

        switch(escolha){
            case 1:{//inserir
                system("cls");
                InserirProduto(estoque); //especificação 10
                break;
            }

            case 2:{//listar todos
                system ("cls");
                estoque->listarTodos();
                MenuAgain();
                break;
            }

            case 3:{//exibir um
                system ("cls");
                string nome;
                int indNome;

                cout << "Nome dos produtos:\n";
                for (size_t i = 0; i < estoque->getProdutos().size(); i++){
                    cout << "Índice " << i+1 << ": ";
                    cout << (estoque->getProdutos())[i]->getNomeProduto() << endl;
                }

                cout << "\nDigite o nome do produto que deseja exibir: ";
                getline(cin, nome);

                indNome = estoque->pesquisar(nome);
                estoque->exibirUm(indNome);

                MenuAgain();
                break;
            }

            case 4:{//alterar
                system ("cls");
                string nome;
                int indNome;

                cout << "Nome dos produtos:\n";
                for (size_t i = 0; i < estoque->getProdutos().size(); i++){
                    cout << "Índice " << i+1 << ": ";
                    cout << (estoque->getProdutos())[i]->getNomeProduto() << endl;
                }

                cout << "\nDigite o nome do produto que deseja alterar: ";
                getline(cin, nome);

                indNome = estoque->pesquisar(nome);
                estoque->alterarProduto(indNome);

                MenuAgain();
                break;
            }

            case 5:{//remover
                system ("cls");
                string nome;
                int indNome;

                cout << "Nome dos produtos:\n";
                for (size_t i = 0; i < estoque->getProdutos().size(); i++){
                    cout << "Índice " << i+1 << ": ";
                    cout << (estoque->getProdutos())[i]->getNomeProduto() << endl;
                }

                cout << "\nDigite o nome do produto que deseja remover: ";
                getline(cin, nome);

                indNome = estoque->pesquisar(nome);
                estoque->removerProduto(indNome);

                MenuAgain();
                break;
            }

            case 6:{//exibir relatório
                estoque->exibirRelatorio();
                MenuAgain();
                break;
            }

            case 7:{//sair
                EncerrarPrograma(estoque); //especificação 10
                return 0; //especificação 9
            }

            default:{
                system("cls");
                cout << "Opção inválida. ";
                MenuAgain();

                break;
            }
        }
    }

    return 0;
}