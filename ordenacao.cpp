/*AVISO!! AO ORDENAR O ARQUIVO JA ORDENADO, PODE CAUSAR UMA GRANDE PERDA DE EFICIENCIA DEVIDO A NATUREZA DO QUICKSORT

Alex Cyrillo de Sousa Borges
Gabriel Furtado Teixeira
Mardem Arantes de Castro

GAC-108 - Estrutura de Dados
*/
#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

struct pessoa
{
	char id[10] = {' '};
	char nome[100] = {' '};
	char emprego[100] = {' '};
	char salarioBase[20] = {' '};
	char salarioExtra[20] = {' '};
	char salarioOutros[20] = {' '};
	char beneficios[20] = {' '};
	char totalS[20] = {' '};
	char totalSb[20] = {' '};
	char ano[20] = {' '};
};

//Menu utilizado por estética
void menu(){
    cout << "Escolha uma das opções abaixo:" << endl;
    cout << "(i) - imprimir o arquivo binário" << endl;
    cout << "(o) - ordenar o arquivo binário" << endl;
    cout << "(s) - sair e encerrar" << endl;
}

//Essa função será usada para trocar dois elementos de posição no arquivo
void trocar(int pos,int pos1)
//Primeiramente, é lido os registros das duas posições.
//Segundamente então, suas posições trocam e é feito a escrita.
{
	pessoa aux,aux1;
	fstream binario;
	binario.open("san_francisco_payroll_dataset.bin",ios::in|ios::out);
	binario.seekg(pos * sizeof(pessoa));
	binario.read((char *)(&aux),sizeof(pessoa));
	binario.seekg(pos1 * sizeof(pessoa));
	binario.read((char *)(&aux1),sizeof(pessoa));
	binario.seekp(pos1 * sizeof(pessoa));
	binario.write((const char *)(&aux),sizeof(pessoa));
	binario.seekp(pos * sizeof(pessoa));
	binario.write((const char *)(&aux1),sizeof(pessoa));
	binario.close();
}
//Função básica utilizada para imprimir o arquivo inteiro, apenas para fins de análise da ordenação
void imprimir(){
    fstream binario;
    binario.open("san_francisco_payroll_dataset.bin",ios::out|ios::in);
	binario.seekp(0, binario.end);
	int tam_byte = binario.tellp();
	binario.seekp(0, binario.beg);
	long int quantidade_registro = (tam_byte / sizeof(pessoa));
	pessoa registro;
	cout << "Id    Nome do Empregado    Cargo     Salário Base    Salário Extra    Outros Pagamentos    Benefícios    Pagamento Total    Pagamento Total e Benefícios    Ano\n";
    for(int pos = 0; pos < quantidade_registro; pos++){
		binario.seekg(pos * sizeof(pessoa));
		binario.read((char *)(&registro),sizeof(pessoa));
		cout << registro.id << "  " << registro.nome << "  " << registro.emprego << "  " << registro.salarioBase << "  " << registro.salarioExtra << "  " << registro.salarioOutros << "  " << registro.beneficios << "  " << registro.totalS << "  " << registro.totalSb << "  " << registro.ano << endl;
    }
    binario.close();
}

/* Essa função pega o ultimo elemento do arquivo, coloca na sua posição correta baseada no arquivo ja ordenado, menores elementos à esquerda e maiores elementos à direita*/
int partição (fstream& binario, int inicio, int fim) { 
    pessoa pivot,regI; 
    binario.seekp(fim * sizeof(pessoa));
    binario.read((char*) &pivot, sizeof(pessoa));

    int i = (inicio - 1); 
  
    for (int j = inicio; j <= fim - 1; j++) 
    { 
        binario.seekp(j * sizeof(pessoa));
		binario.read((char*) &regI, sizeof(pessoa));
        if(regI.nome[0]=='"'){
            int tam = strlen(regI.nome);
            string palavra = "";
            for(int i = 0;i< tam; i++){
                if(i!=0)
                    palavra+=regI.nome[i];
            }
            if (string(palavra) > string(pivot.nome)) 
            { 
                i++; 
                trocar(i,j);
            }
            else if(string(palavra) == string(pivot.nome))
            {
                
                if(atoi(regI.id) >= atoi(pivot.id)){
                    i++; 
                    trocar(i,j);
                }
            }
        }
        else if (string(regI.nome) > string(pivot.nome)) 
        { 
            i++; 
            trocar(i,j);
        }
        else if(string(regI.nome) == string(pivot.nome)){
            if(atoi(regI.id) >= atoi(pivot.id)){
                i++; 
                trocar(i,j);
            }
        }
    } 
    trocar(i + 1,fim);
    return (i + 1); 
} 
//Função recursiva principal do quicksort, chama a função de partição para todos as "divisões" do arquivo
void quickSort(fstream &binario, int inicio, int fim) 
{ 
    if (inicio < fim) 
    {
        int pi = partição(binario, inicio, fim); 
        //Pega a primeira metade do arquivo principal
        quickSort(binario, inicio, pi - 1);
        //Pega a segunda metade do arquivo principal
        quickSort(binario, pi + 1, fim); 
    } 
}

int main(){
    fstream binario;
    binario.open("san_francisco_payroll_dataset.bin",ios::out|ios::in);

    binario.seekp(0, binario.end);
	int tam_byte = binario.tellp();
	binario.seekp(0, binario.beg);
	int quantidade_registro = (tam_byte / sizeof(pessoa));

    char opcao;
    
    do{
        menu();
        cin >> opcao;
        switch (opcao){
            case 'i':
                imprimir();
                break;
            case 'o':
                cout<<"Ordenando, aguarde..."<<endl;
                quickSort(binario,0,quantidade_registro-1);
                cout<<"Arquivo Ordenado com Sucesso!!"<<endl;
                break;
            case 's':
                break;
            default:
                cout << "OPÇÃO INVÁLIDA" << endl;
                break;
        }
    }while(opcao!='s');

    binario.close();

    return 0;
}