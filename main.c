/*Gabriel Lopez Vendramini 22121015-6
  Juan Lira Estevão 22121033-9 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define TAM2 1000

void extrato(){
  FILE *extr;
  extr = fopen("./descricao/descr.txt","r");

  if(extr ==NULL)
    printf("\nErro na abertura\n");
  else
    printf("\n");

  printf("\n\n____________EXTRATO____________\n\n");
  char linha[TAM2];
  while ((fgets(linha, TAM2, extr)) != NULL){
    printf("%s",linha);
  }
  fclose(extr);
  printf("______________________________\n\n\n");
}

void rel_mensal(){
  int mes;
  char velha_linha[TAM2];
  char mensal_desat[TAM2];
  char update[TAM2]; /*Vai guardar a table do html*/
  memset(update,0,strlen(update)); /*limpar*/

  printf("Digite o mês desejado, para que o relatório seja gerado: ");
  scanf("%d",&mes);

  /*A partir de qual linha do html anual vamos copiar os valores*/
  int linha_inicial = (mes-1)*8 + mes + 1; 

  FILE *mensal;
  FILE *novo; /*Atualização do Mensal*/
  FILE *anual;
  
  mensal = fopen("mensal.html","r");
  novo = fopen("novo.html","w");
  anual = fopen("anual.html","r");

  if (anual == NULL || novo == NULL || mensal ==NULL)
    printf("\nErro na abertura.\n");
  else
    printf("\n");

  int counter =0;
  int counter2=0;

  while ((fgets(velha_linha, TAM2, anual)) != NULL){
    counter++;

    if(counter>=linha_inicial && counter<=linha_inicial+7){
      strcat(update,velha_linha);
      }
  }

  while ((fgets(mensal_desat,TAM2, mensal))!= NULL){
    counter2++;

    if(counter2==2){
      fputs(update,novo);
    }
    if(counter2>=2 && counter2<=10){

    }
    else{
      fputs(mensal_desat,novo);
    }
  }
  fclose(anual);
  fclose(mensal);
  fclose(novo);

  remove("mensal.html");
  rename("novo.html","mensal.html");

  printf("Relatório mensal gerado para o mês %d\n Abrir o HTML 'mensal.html' para mais detalhes \n\n",mes);
}



void rel_anual(char valor[20],int linha, int mes){
  char velha_linha[TAM2];
  char tag1[20] = "<td class='a'>";
  char tag2[20] = "</td>\n";

  /*Sacada de mestre pra localizar a linha no html*/
  int line = (mes-1)*8 + mes + linha;

  FILE *anual;
  FILE *novo_ano;
  anual = fopen("anual.html","r");
  novo_ano = fopen("novo.html","w");

  if (anual == NULL || novo_ano == NULL)
    printf("\nErro na abertura.\n");
  else
    printf("\n");

  int counter =0;
  
  while ((fgets(velha_linha, TAM2, anual)) != NULL){
    counter++;

    if (counter == line){
      strcat(tag1,valor);
      strcat(tag1,tag2);
      fputs(tag1,novo_ano);
    }
    else
      fputs(velha_linha, novo_ano);
  }

  fclose(anual);
  fclose(novo_ano);
  remove("anual.html");
  rename("novo.html", "anual.html");

}


/*Guardar datas e descrições no txt*/
void abrir_dd(){
  char data_descr[TAM2];
  FILE *dd;
  dd = fopen("./descricao/descr.txt","a");
  if (dd == NULL)
    printf("\nErro na abertura.\n");    
  else
    printf("\n");
  
  setbuf(stdin, NULL); /*para evitar erros na leitura*/
  
  printf("Digite a data e a descrição:\n");
  fgets(data_descr,TAM2,stdin);

  fputs(data_descr,dd);
  fclose(dd);
}

/*Guardar valores nos respectivos meses*/
void abrir_arquivo(char a[20], int linha, int col){

  FILE *mes;
  FILE *novo;

  char velha_linha[TAM2];
  float nova_linha;
  int line, contador;

  line = linha;

  setbuf(stdin, NULL); /*para evitar erros na leitura*/

  printf("Digite o valor em reais: ");
  scanf("%f",&nova_linha);

  /*  o "a" define o arquivo que será aberto*/
  mes = fopen(a,"r");
  novo = fopen("novo.txt", "w"); 


  /*Verificando abertura do arquivo*/
  if (mes == NULL || novo == NULL /*|| ano==NULL*/)
    printf("\nErro na abertura.\n");    
  else
    printf("Arquivo aberto com sucesso!\n\n");

  contador = 0;
  
  float val_ant;
  float soma;
  char conv[20];
  char relatorio[20];

  /*Enquanto tiver linha no arq*/
  while ((fgets(velha_linha, TAM2, mes)) != NULL){        
    contador++; /*somar 1 no contador*/

    /* Se o número de linha é igual a linha desejada */
    if (contador == line){

      /*convertendo o valor da linha pra float*/
      val_ant = atof(velha_linha);

      /*somando com o valor digitado do user*/
      soma = val_ant + nova_linha;

      /*convertendo a soma para uma string de nome conv, com 2 casas dps da virgula*/
      sprintf(conv, "%.2f\n", soma);
      sprintf(relatorio,"%.2f",soma);/*mandar pro html*/

      /*jogando no txt*/
      fputs(conv,novo);
      rel_anual(relatorio,linha,col);
    }
    else
        /*Jogar os textos que se mantiveram*/
        fputs(velha_linha, novo);
    }
    /* Fechando */
    fclose(mes);
    fclose(novo);

    /* Deletando o original, o desatualizado */
    remove(a);

    /* Renomear o novo com o nome do antigo */
    rename("novo.txt", a);

    printf("\nValor anterior da categoria: R$%.2f\nAgora, o valor foi alterado pra R$%.2f ao somar R$%.2f\n\n", val_ant,soma,nova_linha);

    /*Guardando o valor na descrição*/
    FILE *descr;
    descr = fopen("./descricao/descr.txt","a");
    if( descr == NULL){
      printf("\nErro na Abertura");
    }
    else
      printf("\n");
    char value[30];
    sprintf(value, "Valor: R$%.2f\n\n", nova_linha);
    fputs(value,descr);
    fclose(descr);
}

/*___________CADASTRO DE RECEITAS___________*/
void receita(){
  int mes, opcaoreceita;

  printf(" _____________________________________\n");
  printf("|______________|RECEITAS|_____________|\n");
  printf("|_____________________________________|\n");
  printf("|Qual receita deseja adicionar ?      |\n");
  printf("|                                     |\n");
  printf("|1. Salario                           |\n");
  printf("|2. Pix                               |\n");
  printf("|3. Loteria                           |\n");
  printf("|4. Voltar ao menu principal          |\n");
  printf("|_____________________________________|\n\n");

  printf("Digite o número da receita que quer adicionar:");
  scanf("%d",&opcaoreceita);

  /*Caso a opção seja incorreta*/
  if(opcaoreceita<0 || opcaoreceita>4){
    printf("Valor Inválido, digite um número entre 1 e 4!\n \n \n");
    receita();
  }

  else if(opcaoreceita>0 && opcaoreceita<4){ /*se for entre 1 e 3*/
    printf("Digite o mês: ");
    scanf("%d",&mes);

    abrir_dd();
    
    if (mes == 1){
      abrir_arquivo("./valores/jan.txt",opcaoreceita,mes);
    }if (mes == 2){
      abrir_arquivo("./valores/fev.txt",opcaoreceita,mes);
    }if (mes == 3){
      abrir_arquivo("./valores/mar.txt",opcaoreceita,mes);
    }if (mes == 4){
      abrir_arquivo("./valores/abr.txt",opcaoreceita,mes);
    }if (mes == 5){
      abrir_arquivo("./valores/mai.txt",opcaoreceita,mes);
    }if (mes == 6){
      abrir_arquivo("./valores/jun.txt",opcaoreceita,mes);
    }if (mes == 7){
      abrir_arquivo("./valores/jul.txt",opcaoreceita,mes);
    }if (mes == 8){
      abrir_arquivo("./valores/ago.txt",opcaoreceita,mes);
    }if (mes == 9){
      abrir_arquivo("./valores/set.txt",opcaoreceita,mes);
    }if (mes == 10){
      abrir_arquivo("./valores/out.txt",opcaoreceita,mes);
    }if (mes == 11){
      abrir_arquivo("./valores/nov.txt",opcaoreceita,mes);
    }if (mes == 12){
      abrir_arquivo("./valores/dez.txt",opcaoreceita,mes);
      }

  }

  /*Caso deseje voltar*/
  else if(opcaoreceita==4){
    printf("Voltando ao menu principal...\n \n \n");
  }

  } 


/*___________CADASTRO DE GASTOS____________*/
void gastos(){
  int mes, opcaogasto; 


  printf(" _____________________________________\n");
  printf("|_______________|Gastos|______________|\n");
  printf("|_____________________________________|\n");
  printf("|Qual Gasto deseja adicionar ?        |\n");
  printf("|                                     |\n");
  printf("|1. Moradia                           |\n");
  printf("|2. Estudos                           |\n");
  printf("|3. Transporte                        |\n");
  printf("|4. Alimentação                       |\n");
  printf("|5. Lazer                             |\n");
  printf("|6. Voltar ao menu principal          |\n");
  printf("|_____________________________________|\n\n\n");

  printf("Digite o número da categoria de gastos:");
  scanf("%d",&opcaogasto);

    if(opcaogasto>0 && opcaogasto<6){ /*se for entre 1 e 5*/
      printf("Digite o mês: ");
      scanf("%d",&mes);

      abrir_dd();

      if (mes == 1){
        abrir_arquivo("./valores/jan.txt",opcaogasto+3,mes);
      }if (mes == 2){
        abrir_arquivo("./valores/fev.txt",opcaogasto+3,mes);
      }if (mes == 3){
        abrir_arquivo("./valores/mar.txt",opcaogasto+3,mes);
      }if (mes == 4){
        abrir_arquivo("./valores/abr.txt",opcaogasto+3,mes);
      }if (mes == 5){
        abrir_arquivo("./valores/mai.txt",opcaogasto+3,mes);
      }if (mes == 6){
        abrir_arquivo("./valores/jun.txt",opcaogasto+3,mes);
      }if (mes == 7){
        abrir_arquivo("./valores/jul.txt",opcaogasto+3,mes);
      }if (mes == 8){
        abrir_arquivo("./valores/ago.txt",opcaogasto+3,mes);
      }if (mes == 9){
        abrir_arquivo("./valores/set.txt",opcaogasto+3,mes);
      }if (mes == 10){
        abrir_arquivo("./valores/out.txt",opcaogasto+3,mes);
      }if (mes == 11){
        abrir_arquivo("./valores/nov.txt",opcaogasto+3,mes);
      }if (mes == 12){
        abrir_arquivo("./valores/dez.txt",opcaogasto+3,mes);
        }

  }
  else if(opcaogasto==6){

    printf("Voltando ao menu principal...");
  }
  else{
    printf("Valor Inválido, digite um número entre 1 e 6!");
    receita();
  }
} 

/*______________GERADOR DE RELATÓRIO_____________*/
void relatorio(){
  int escolha;

  printf(" _____________________________________\n");
  printf("|_____________|Relatório|_____________|\n");
  printf("|_____________________________________|\n");
  printf("|Qual relatório deseja gerar?         |\n");
  printf("|                                     |\n");
  printf("|1. Mensal                            |\n");
  printf("|2. Anual                             |\n");
  printf("|3. Voltar ao menu principal          |\n");
  printf("|_____________________________________|\n\n\n");

  printf("Digite a escolha desejada: ");
  scanf("%d",&escolha);

  if(escolha==1){
    rel_mensal();
  }
  else if(escolha==2){
      printf("Relatório anual atualizado\nAbrir HTML 'anual.html' para mais detalhes.\n\n");
  }
  else if(escolha==3){
    printf("Voltando ao menu principal....");
  }
  else{
    printf("Valor Inválido! Digite um número entre 1 e 3!");
  }

} 
/*___________POUPANÇA_____________*/
void poupanca(){
  printf("\nSIMULAÇÃO - POUPANÇA\n");
  int mes;
  float Poupança,v_mensal,lucro;
  
  printf("Digite o mês: ");
  scanf("%d",&mes);
  
  printf("Digite o valor que deseja inserir: ");
  scanf("%f",&Poupança);
 
  printf("Digite o valor que será inserido por mês: ");
  scanf("%f",&v_mensal);
  float poupança2 = Poupança;

 
  printf("Valor inicial investido na poupança foi de %.2f reais\n",Poupança);
  
  float taxa[12] = {0.0015,0.0013,0.0020,0.0021,0.0027,0.0031,0.0036,0.0043,0.0044,0.0049,0.0049,0.0049};

  int j;
  for(j=1; j< 13; j++){
    if(j>=mes){
      
      poupança2 = (poupança2 +v_mensal) + ((poupança2+v_mensal) * taxa[j-1]);
    
    }
  }
  lucro = poupança2 - (v_mensal*(13-mes)+Poupança)-0.035*v_mensal;
  printf("Ao final do ano: %.2f reais\n",poupança2);
  printf("Valor do lucro foi de %.2f reais\n",lucro);

}
/*___________TESOURO_DIRETO_____________*/
void tesouro_dir(){
  int mes;
  float Poupança,v_mensal,lucro; 
  printf("\nSIMULAÇÃO - TESOURO DIRETO\n");
  printf("Digite o mês de início do investimento: ");
  scanf("%d",&mes);
  
  printf("Digite o valor que deseja inserir agora: ");
  scanf("%f",&Poupança);
 
  printf("Digite o valor que será inserido por mês: ");
  scanf("%f",&v_mensal);
  float poupança2 = Poupança;

  printf("Valor inicial investido no tesouro direto foi de %.2f reais\n",Poupança);
  
  float taxa= 0.0041;

  int j;
  for(j=1; j< 13; j++){
    if(j>=mes){
      poupança2 = (poupança2 +v_mensal) + ((poupança2+v_mensal) * taxa);
    }
  }
  lucro = poupança2 - (v_mensal*(13-mes)+Poupança)-0.0041*v_mensal;
  printf("Ao final do ano, o valor total seria de %.2f reais\n",poupança2);
  printf("Durante esse tempo, o lucro foi de %.2f reais\n",lucro);
}
/*___________BOLSA_DE_VALORES_____________*/
void bolsa(){
  printf("\nSIMULAÇÃO - BOLSA DE VALORES\n");
  int mes;
  float Poupança,v_mensal,lucro;
  
  printf("Digite o mês: ");
  scanf("%d",&mes);
  
  printf("Digite o valor que deseja inserir: ");
  scanf("%f",&Poupança);
 
  
  float poupança2 = Poupança;

 
  printf("Valor inicial investido na poupança foi de %.2f reais\n",Poupança);
  
  float taxa = -0.129;

  poupança2 = Poupança + (taxa*Poupança);

  lucro = poupança2 - Poupança;
  
  printf("Valor final foi de %.2f reais\n",poupança2);
  printf("Valor do lucro foi de %.2f reais\n",lucro);

}

/*___________INVESTIMENTO CDB_____________*/
void cdb(){
  printf("\nSIMULAÇÃO - CERTIFICADO DE DEPÓSITO BANCÁRIO");
  int mes;
  float Poupança,v_mensal,lucro; 
  int banco;
  float taxa;
  printf("\nBANCOS PARCEIROS\n1. Santander\n2. Banco do Brasil\n3. Itaú\n4. Caixa\n5. Bradesco\n6. Nubank\n7. Outro\n\n");

  printf("Digite o banco parceiro:");
  scanf("%d",&banco);

  if(banco>0 && banco<8){
    if (banco==1)
      taxa = 0.0041;
    if (banco==2)
      taxa = 0.0051;
    if (banco==3)
      taxa = 0.0054;
    if (banco==4)
      taxa = 0.0044;
    if (banco==5)
      taxa = 0.0037;
    if (banco==6)
      taxa = 0.0050;
    if (banco==7)
      taxa = 0.0039;

    printf("Digite o mês de início do investimento: ");
    scanf("%d",&mes);
    
    printf("Digite o valor que deseja inserir agora: ");
    scanf("%f",&Poupança);
  
    printf("Digite o valor que será inserido por mês: ");
    scanf("%f",&v_mensal);
    float poupança2 = Poupança;

    printf("Valor inicial investido: %.2f reais\n",Poupança);

    int j;
    for(j=1; j< 13; j++){
      if(j>=mes){
        poupança2 = (poupança2 +v_mensal) + ((poupança2+v_mensal) * taxa);
      }
    }
    lucro = poupança2 - (v_mensal*(13-mes)+Poupança)-0.0041*v_mensal;
    printf("Ao final do ano, o valor total seria de %.2f reais\n",poupança2);
    printf("Durante esse tempo, o lucro foi de %.2f reais\n",lucro);

  }
  else
    printf("Valor inválido! Digite um número entre 1 e 7");

}

/*___________DESCRIÇÃO_INVESTIMENTOS_____________*/
void ajuda(){
  printf("\n\n\n_____________________________________\n");
  printf("Onde devo investir?\n\n\n");
  printf("1. TESOURO DIRETO\nO tesouro direto é um programa criado pelo Tesouro Nacional. Na prática, você está emprestando dinheiro para o governo em troca de uma rentabilidade.\n\n  Vantagens:\n  -Baixo Risco\n  -Acessibilidade: podemos começar investindo R$30\n  -Resgate a qualquer hora");

  printf("\n\n\n2. BOLSA DE VALORES\nCom o tempo, o valor da bolsa de valores tende a aumentar, embora haja variação sempre, seja positiva ou negativa. Porém, o investimento em empresas estáveis pode alavancar o seu dinheiro\n\n  Vantagens:\n  -Propriedade: ao comprar ações, temos uma participação acionária na empresa\n  -Possibilidade de altos rendimentos\n  -Inúmeras oportunidades, além de ausência de valor mínimo para investir");

  printf("\n\n\n3. CDB\nO funcionamento do CDB é simples. Você escolhe um banco de investimentos, abre sua conta, pesquisa os melhores títulos e entra com o valor desejado para a aplicação. Depois de fazer o investimento, seu dinheiro fica aplicado. Na data de vencimento do título, você recebe o valor original de volta acrescido do rendimento combinado para o período.\n\n  Vantagens:\n  -Opções de alta rentabilidade\n  -Rende mais que a poupança\n  -Variedade de opções e segurança de renda fixa");
  printf("\n\n\n_____________________________________\n");

}
/*_______MENU_INVESTIMENTOS_______________*/
void investimento(){
int escolha;
  printf(" _____________________________________\n");
  printf("|__________|INVESTIMENTOS|____________|\n");
  printf("|_____________________________________|\n");
  printf("|________________MENU_________________|\n");
  printf("|Onde deseja investir?                |\n");
  printf("|                                     |\n");
  printf("|1. Tesouro Direto                    |\n");
  printf("|2. Ações na Bolsa de Valores Ibovespa|\n");
  printf("|3. Certificado de Depósito Bancário  |\n");
  printf("|4. Me ajude a escolher               |\n");
  printf("|5. Voltar ao menu principal          |\n");
  printf("|_____________________________________|\n");

  printf("Digite a opção desejada: ");
  scanf("%d",&escolha);

  if(escolha>0 && escolha<6){
    if(escolha==1)
      tesouro_dir();
    if(escolha==2)
      bolsa();
    if(escolha==3)
      cdb();
    if(escolha==4)
      ajuda();

    if(escolha==5)
      printf("Voltando ao menu principal...");
  }
  else{
    printf("Valor Inválido! Digite um número entre 1 e 5");
  }
} 

/*____________MENU PRINCIPAL____________________*/
int main() {
  while(1){
    printf(" _____________________________________\n");
    printf("|______|GERENCIADOR FINANCEIRO|_______|\n");
    printf("|_____________________________________|\n");
    printf("|________________MENU_________________|\n");
    printf("|Olá, Usuário                         |\n");
    printf("|Como posso ajudar?                   |\n");
    printf("|1. CADASTRAR RECEITA                 |\n");
    printf("|2. CADASTRAR GASTOS                  |\n");
    printf("|3. GERAR RELATÓRIO                   |\n");
    printf("|4. POUPANÇA                          |\n");
    printf("|5. INVESTIMENTOS                     |\n");
    printf("|6. VER EXTRATO                       |\n");
    printf("|_____________________________________|\n");

    int escolha;
    printf("Digite o número daquilo que deseja fazer: ");
    scanf("%d",&escolha);

    if(escolha==1){
      receita();
    }else if(escolha==2){
      gastos();
    }else if(escolha==3){
      relatorio();
    }else if(escolha==4){
      poupanca();
    }else if(escolha==5){
      investimento();
    }else if(escolha==6){
      extrato();
    }
    else{
      printf("Valor Inválido: Escolha um número de 1 a 6!\n");
    }
    }
}