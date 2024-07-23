#include <stdio.h> //Biblioteca de comunicação com o usuário
#include <stdlib.h> //Biblioteca de alocação de memória (a entender ainda)
#include <locale.h> //Biblioteca de alocação de texto por região. Ou seja, usada para poder definir os caracteres de um idioma a ser mostrado ao e usado pelo usuário.
#include <string.h> //Biblioteca para trabalhar com strings
#include <ctype.h> //Biblioteca para trabalhar caracteres

#define OPCAO_S 'S'
#define OPCAO_N 'N'
#define TAMANHO_CPF 11

// Função para remover o caracter de quebra de linha da string e substituir pelo caractere nulo
void remover_nova_linha (char *str)
{
	if(str[TAMANHO_CPF + 1] == '\n'){ 
			str[TAMANHO_CPF + 1] = '\0';
	}
	else
	{
		int i;
		while((i = getchar()) != '\n' && i != EOF);
	}
}

//Função para verificar se o CPF informado contém 11 caracteres e é formada apenas por números
int verifica_cpf(const char *cpf) 
{
	int i;
	
	if(strlen(cpf) != TAMANHO_CPF) //Verifica se a string tem 11 dígitos
	{
        return 0;
    }
    for(i = 0 ; i < TAMANHO_CPF; i++) //Verifica se a string contém somente números
	{
		if(!isdigit(cpf[i])) 
		{
	        return 0;
	    }
	}
    return 1;
}

//Função para inserir novos registros no banco de dados
int cadastrar()
{
	char nome_arquivo[TAMANHO_CPF+1], cpf[TAMANHO_CPF+1], nome[60], sobrenome[60], cargo [20];
	
	setlocale(LC_ALL,"Portuguese");
	
	do
	{
		printf("CPF (Sem pontos e traços): ");
		scanf("%s", cpf);
							
		if(verifica_cpf(cpf) == 0) // Verifica se o CPF contém exatamente 11 dígitos e se é composto apenas por números
		{
			printf("CPF inválido.\n\n");
			system("pause");
			system("cls");
			printf("--- REGISTRO DE NOVO CADASTRO ---\n\n\n");	
		}
	} while (verifica_cpf(cpf) == 0);
	
	remover_nova_linha(cpf);
		
	strcpy(nome_arquivo, cpf);
	
	FILE *file;
	file = fopen(nome_arquivo, "w");
	fprintf(file, cpf);
	fclose(file);
	
	file = fopen(nome_arquivo, "a");
	fprintf(file, ",");
	fclose(file);
	
	printf("Nome: ");
	fgets(nome, sizeof(nome), stdin); //fgets para poder receber espaços
	nome[strcspn(nome, "\n")] = '\0'; //Função para remover o caractere de quebra de linha e substituir pelo caractere nulo
			
	file = fopen(nome_arquivo, "a");
	fprintf(file, nome);
	fclose(file);
	
	file = fopen(nome_arquivo, "a");
	fprintf(file, ",");
	fclose(file);

	printf("Sobrenome: ");
	fgets(sobrenome, sizeof(sobrenome), stdin);
	sobrenome[strcspn(sobrenome, "\n")] = '\0';
	
	file = fopen(nome_arquivo, "a");
	fprintf(file, sobrenome);
	fclose(file);
	
	file = fopen(nome_arquivo, "a");
	fprintf(file, ",");
	fclose(file);
	
	printf("Cargo: ");
	fgets(cargo, sizeof(cargo), stdin);
	cargo[strcspn(cargo, "\n")] = '\0';
	
	file = fopen(nome_arquivo, "a");
	fprintf(file, cargo);
	fclose(file);
	
	printf("\n\nUsuário cadastrado com sucesso.\n");		
	system("pause");
}

//Função para buscar e visualizar informações armazenadas em banco de dados a partir de uma chave-primária
int consultar()
{
	setlocale(LC_ALL,"Portuguese");
	
	char cpf[TAMANHO_CPF+1], conteudo[100], *token;
	
	printf("Informe o CPF para buscar o cadastro: ");
	scanf("%s", cpf);
	
	FILE *file;
	file = fopen(cpf,"r");
	
	if (file == NULL)
	{
		printf("Cadastro não localizado.\n\n");
		system("pause");
	}
	
	while(fgets(conteudo, 200, file) != NULL)
	{
		printf("\nCadastro localizado:\n\n");
		token = strtok(conteudo, ",");
		printf("\tCPF:       %s\n", token);
		token = strtok(NULL, ",");
		printf("\tNome:      %s\n", token);
		token = strtok(NULL, ",");
		printf("\tSobrenome: %s\n", token);
		token = strtok(NULL, ",");
		printf("\tCargo:     %s\n", token);
		printf("\n");
	}
	system("pause");
}

//Função para excluir registros do banco de dados
int deletar()
{
	char cpf[TAMANHO_CPF + 1], conteudo[100], virgula[2] = ",", *token, opcao;
	int erro = 1;
	
	setlocale(LC_ALL,"Portuguese");
	
	printf("Informe o CPF para buscar o cadastro: ");
	scanf("%s", cpf);
	
	remover_nova_linha(cpf);
	
	//Busca do cadastro no banco de dados
	FILE *file;
	file = fopen(cpf,"r");
	
	if (file == NULL) //Mensagem caso não seja localizado o cadastro
	{
		printf("Cadastro não localizado.\n\n");
		system("pause");
	}
	else //Exibir o cadastro localizado
	{
		while(fgets(conteudo, 200, file) != NULL)
		{
			printf("\nCadastro localizado:\n\n");
			token = strtok(conteudo, virgula);
			printf("\tCPF:       %s\n", token);
			token = strtok(NULL, virgula);
			printf("\tNome:      %s\n", token);
			token = strtok(NULL, virgula);
			printf("\tSobrenome: %s\n", token);
			token = strtok(NULL, virgula);
			printf("\tCargo:     %s\n", token);
			printf("\n\n");
		}
		
		do //Ações a ser tomadas com o cadastro encontrado
		{
			printf("Deseja excluir este cadastro? S/N: ");
			scanf("%c", &opcao);
			getchar();
			
			opcao = toupper(opcao); //Função para deixar o caractere em caixa alta
					
			switch(opcao)
			{
				case OPCAO_S:
					fclose(file); //Precisa fechar o arquivo para poder deletá-lo
					remove(cpf); //Remove o arquivo salvo
					printf("\nCadastro excluído com sucesso.\n");
					system("pause");
					erro = 0;
				break;
				
				case OPCAO_N:
					printf("\nRetornando ao menu principal.\n");
					system("pause");
					erro = 0;
				break;
				
				default:
					printf("\nOpção inválida. Digite 'S' para excluir e 'N' para retornar ao menu principal.\n");
			}
		} while(erro == 1);
	}
}

//Função para sair do sistema
int encerrar()
{
	char encerrar_programa;
	
	do
	{
		printf("Deseja encerrar o programa? S/N: ");
		scanf("%c", &encerrar_programa);
		getchar();
		
		encerrar_programa = toupper(encerrar_programa); //Função para deixar o caractere em caixa alta
		
		switch(encerrar_programa)
		{
			case OPCAO_S:
				printf("\nO programa será encerrado.\n");
				system("pause");
				return 1;
			break;
			
			case OPCAO_N:
				printf("\nRetornando ao menu principal.\n");
				system("pause");
				return 0;
			break;
			
			default:
				printf("\nOpção inválida. Digite 'S' para fechar e 'N' para retornar ao menu principal.\n");
		}									
	}
	while(1);
}

//Menu principal
int main(void)
{
	int opcao = 0, fecha = 0, laco = 0, check_senha;
	char senha[] = "a";
	
	//Tela de login de admin
	while(laco == 0)
	{
		setlocale(LC_ALL,"Portuguese");
			
		system("cls");
					
		printf("### CARTÓRIO EBAC ###\n\n");
		printf("Por favor, digite sua senha de administrador: \n");
		scanf("%s", senha);
		
		check_senha = strcmp(senha, "admin"); //Comprativo da senha inserida com a armazenada no sistema
		
		//Verificação de aprovação do login
		if(check_senha == 0)
		{
			//Login aprovado
			while(fecha == 0) //Laço para permitir o encerramento do programa
			{
				setlocale(LC_ALL,"Portuguese");
				
				system("cls");
				laco = 1;
				
				printf("### CARTÓRIO EBAC ###\n\n");
				printf("Olá! Digite o número de uma opção para prosseguir:\n\n");
				printf("\t1 - Registrar novo cadastro\n");
				printf("\t2 - Consultar cadastro\n");
				printf("\t3 - Excluir cadastro\n");
				printf("\t4 - Encerrar programa\n\n");
				
				printf("Opção: ");
				scanf("%d", &opcao);
				getchar();
				
				system("cls");
				
				switch(opcao)
				{
					case 1:
						printf("--- REGISTRO DE NOVO CADASTRO ---\n\n\n");
						cadastrar();
					break;
					
					case 2:
						printf("--- CONSULTA DE CADASTRO ---\n\n\n");
						consultar();
					break;
					
					case 3:
						printf("--- EXCLUIR CADASTRO ---\n\n\n");
						deletar();
					break;
					
					case 4:
						fecha = encerrar();	
					break;
					
					default:
						printf("Opção inválida\n");
						system("pause");
					break;
				}
			}
		}
		//Login recusado
		else
		{
			printf("\nSenha incorreta!\n");
			system("pause");
		}
	}
	return 0;
}
