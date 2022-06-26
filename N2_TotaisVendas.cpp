//	Adryellen das Graças Santana
//	Lucas Oliveira Loura

#include <stdio.h>
#include <stdlib.h>

struct TProdutos
{
	int codProd;
	char modCtrl;
	double qtdeEstq, prcUnit, margLucr;
};

struct TVendas
{
	int ano, mes, dia, codProd;
	double qtde, preco, total;
};

struct TTotVendasDia
{
	int dia, mes, ano;
	double total;
	int qtde;
	double mediaDia;
};

struct TTotVendasProd
{
	int codProd;
	double valrTot;
	int qtde;
	double prcMed, prcUnit, lucr, contrib;
};

int dataJaExiste(int dia, int mes, int ano, TTotVendasDia *pV, int tamV);

int prodJaExiste(int cod, TTotVendasProd *pV, int tamV);

double buscarPrcUnit(int cod, TProdutos *pV, int tamV);

int main (void)
{
	TProdutos *prods, prodsAux;
	TVendas *vendas, vendasAux;
	TTotVendasDia *totVendasDia;
	TTotVendasProd *totVendasProd, totVendasProdAux;
	int tamProds = 0, tamVendas = 0, tamTotVendasDia = 0, tamTotVendasProd = 0;
	int i, pos;
	FILE *arq;
	double totGeralVendas = 0, medVendasDia, medVendasProd, lucrMed = 0; 
	int qtdeProdVend = 0;
	bool trocou;

	arq = fopen("produtos.txt", "r");
	prods = NULL;

	while (fscanf(arq, "%d;%c;%lf;%lf;%lf;", & prodsAux.codProd, & prodsAux.modCtrl, & prodsAux.qtdeEstq, & prodsAux.prcUnit, & prodsAux.margLucr) != EOF)
	{
		tamProds++;
		prods = (TProdutos *)realloc(prods, sizeof(TProdutos) * tamProds);
		prods[tamProds - 1] = prodsAux;
	}

	fclose(arq);

	//arq = fopen("vendasTemp.txt","r");
	arq = fopen("vendas.txt","r");
	vendas = NULL;

	while (fscanf(arq, "%d;%d;%d;%d;%lf;%lf;", & vendasAux.ano, & vendasAux.mes, & vendasAux.dia, & vendasAux.codProd, & vendasAux.qtde, & vendasAux.preco) != EOF)
	{
		vendasAux.total = vendasAux.qtde * vendasAux.preco;
		tamVendas++;
		vendas = (TVendas *)realloc(vendas, sizeof(TVendas) * tamVendas);
		vendas[tamVendas - 1] = vendasAux;
	}

	fclose(arq);

	arq = fopen("totvendas.txt", "w");


	//	calcular, exibir e gravar total de vendas por dia


	totVendasDia = NULL;

	for (i = 0; i < tamVendas; i++)
	{
		pos = dataJaExiste(vendas[i].dia, vendas[i].mes, vendas[i].ano, totVendasDia, tamTotVendasDia);

		if (pos >= 0)
		{
			totVendasDia[pos].total += vendas[i].total;
			totVendasDia[pos].qtde++;
			totVendasDia[pos].mediaDia = totVendasDia[pos].total / totVendasDia[pos].qtde;
		}
		else
		{
			tamTotVendasDia++;
			totVendasDia = (TTotVendasDia *)realloc(totVendasDia, sizeof(TTotVendasDia) * tamTotVendasDia);
			totVendasDia[tamTotVendasDia - 1].dia = vendas[i].dia;
			totVendasDia[tamTotVendasDia - 1].mes = vendas[i].mes;
			totVendasDia[tamTotVendasDia - 1].ano = vendas[i].ano;
			totVendasDia[tamTotVendasDia - 1].total = vendas[i].total;
			totVendasDia[tamTotVendasDia - 1].qtde = 1;
			totVendasDia[tamTotVendasDia - 1].mediaDia = totVendasDia[tamTotVendasDia - 1].total / totVendasDia[tamTotVendasDia - 1].qtde;
		}
	}

	printf("TOTAIS DE VENDA POR DIA\n");
	printf("Dia                Total    Media/Dia\n");
	for (i = 0; i < tamTotVendasDia; i++)
		printf("%02d/%02d/%d      %8.2lf     %8.2lf\n", totVendasDia[i].dia, totVendasDia[i].mes, totVendasDia[i].ano, totVendasDia[i].total, totVendasDia[i].mediaDia);

	fprintf(arq, "TOTAIS DE VENDA POR DIA\n");
	fprintf(arq, "Dia                Total    Média/Dia\n");
	for (i = 0; i < tamTotVendasDia; i++)
		fprintf(arq, "%02d/%02d/%d      %8.2lf     %8.2lf\n", totVendasDia[i].dia, totVendasDia[i].mes, totVendasDia[i].ano, totVendasDia[i].total, totVendasDia[i].mediaDia);


	//	calcular, reorganizar, exibir e gravar totais de venda por produto


	totVendasProd = NULL;

	for (i = 0; i < tamVendas; i++)
	{
		pos = prodJaExiste(vendas[i].codProd, totVendasProd, tamTotVendasProd);

		if (pos >= 0)
		{
			totVendasProd[pos].valrTot += vendas[i].total;
			totVendasProd[pos].qtde += vendas[i].qtde;
			totVendasProd[pos].prcMed = totVendasProd[pos].valrTot / totVendasProd[pos].qtde;
			totVendasProd[pos].lucr = (totVendasProd[pos].prcMed / totVendasProd[pos].prcUnit - 1) * 100;
		}
		else
		{
			tamTotVendasProd++;
			totVendasProd = (TTotVendasProd *)realloc(totVendasProd, sizeof(TTotVendasProd) * tamTotVendasProd);
			totVendasProd[tamTotVendasProd - 1].codProd = vendas[i].codProd;
			totVendasProd[tamTotVendasProd - 1].valrTot = vendas[i].total;
			totVendasProd[tamTotVendasProd - 1].qtde = vendas[i].qtde;
			totVendasProd[tamTotVendasProd - 1].prcMed = totVendasProd[tamTotVendasProd - 1].valrTot / totVendasProd[tamTotVendasProd - 1].qtde;
			totVendasProd[tamTotVendasProd - 1].prcUnit = buscarPrcUnit(totVendasProd[tamTotVendasProd - 1].codProd, prods, tamProds);
			totVendasProd[tamTotVendasProd - 1].lucr = (totVendasProd[tamTotVendasProd - 1].prcMed / totVendasProd[tamTotVendasProd - 1].prcUnit - 1) * 100;
		}
	}

	do
	{
		trocou = false;

		for (i = 0; i < tamTotVendasProd - 1; i++)
		{
			if (totVendasProd[i].codProd > totVendasProd[i + 1].codProd)
			{
				totVendasProdAux = totVendasProd[i];
				totVendasProd[i] = totVendasProd[i + 1];
				totVendasProd[i + 1] = totVendasProdAux;
				trocou = true;
			}
		}

	}	while (trocou);

	printf("\nTOTAIS DE VENDA POR PRODUTO\n");
	printf("Prod.         VlrTot       Qtde     Pc Medio     Lucrat.\n");
	for (i = 0; i < tamTotVendasProd; i++)
		printf("%5d     %10.2lf     %6d        %5.2lf       %4.1lf%%\n", totVendasProd[i].codProd, totVendasProd[i].valrTot, totVendasProd[i].qtde, totVendasProd[i].prcMed, totVendasProd[i].lucr);

	fprintf(arq, "\nTOTAIS DE VENDA POR PRODUTO\n");
	fprintf(arq, "Prod.         VlrTot       Qtde     Pç Médio     Lucrat.\n");
	for (i = 0; i < tamTotVendasProd; i++)
		fprintf(arq, "%5d     %10.2lf     %6d        %5.lf       %4.1lf%%\n", totVendasProd[i].codProd, totVendasProd[i].valrTot, totVendasProd[i].qtde, totVendasProd[i].prcMed, totVendasProd[i].lucr);


	//	calcular, exibir e gravar total e estatísticas do período


	for (i = 0; i < tamVendas; i++)
	{
		totGeralVendas += vendas[i].total;
		qtdeProdVend += vendas[i].qtde;
	}

	medVendasDia = totGeralVendas / tamTotVendasDia;
	medVendasProd = totGeralVendas / qtdeProdVend;

	for (i = 0; i < tamTotVendasProd; i++)
		lucrMed += (totVendasProd[i].lucr * totVendasProd[i].qtde) / qtdeProdVend;

	printf("\nTOTAL E ESTATISTICAS DO PERIODO\n");
	printf("Total Geral de Vendas (R$)         %10.2lf\n", totGeralVendas);
	printf("Quantidade de produtos vendidos    %10d\n", qtdeProdVend);
	printf("Media de Vendas por dia util (R$)  %10.2lf\n", medVendasDia);
	printf("Media de Vendas por produto (R$)   %10.2lf\n", medVendasProd);
	printf("Lucratividade Media               %10.1lf%%\n", lucrMed);

	fprintf(arq,"\nTOTAL E ESTATISTICAS DO PERÍODO\n");
	fprintf(arq,"Total Geral de Vendas (R$)         %10.2lf\n", totGeralVendas);
	fprintf(arq,"Quantidade de produtos vendidos    %10d\n", qtdeProdVend);
	fprintf(arq,"Média de Vendas por dia util (R$)  %10.2lf\n", medVendasDia);
	fprintf(arq,"Média de Vendas por produto (R$)   %10.2lf\n", medVendasProd);
	fprintf(arq,"Lucratividade Média               %10.1lf%%\n", lucrMed);


	//	reorganizar, exibir e gravar produtos mais lucrativos em ordem decrescente


	do
	{
		trocou = false;

		for (i = 0; i < tamTotVendasProd - 1; i++)
		{
			if (totVendasProd[i].lucr < totVendasProd[i + 1].lucr)
			{
				totVendasProdAux = totVendasProd[i];
				totVendasProd[i] = totVendasProd[i + 1];
				totVendasProd[i + 1] = totVendasProdAux;
				trocou = true;
			}
		}

	}	while (trocou);

	printf("\nPRODUTOS MAIS LUCRATIVOS\n");
	printf("Lucratividade Media = %.1lf%%\n\n", lucrMed);
	printf("Prod.     Lucrat.\n");

	for (i = 0; i < tamTotVendasProd; i++)
	{
		if (totVendasProd[i].lucr > lucrMed)
			printf("%5d       %4.1lf%%\n", totVendasProd[i].codProd, totVendasProd[i].lucr);
		else
			break;
	}

	fprintf(arq, "\nPRODUTOS MAIS LUCRATIVOS\n");
	fprintf(arq, "Lucratividade Média = %.1lf%%\n\n", lucrMed);
	fprintf(arq, "Prod.     Lucrat.\n");

	for (i = 0; i < tamTotVendasProd; i++)
	{
		if (totVendasProd[i].lucr > lucrMed)
			fprintf(arq, "%5d       %4.1lf%%\n", totVendasProd[i].codProd, totVendasProd[i].lucr);
		else
			break;
	}


	//	calcular, reorganizar, exibir e gravar a contribuição de cada produto


	for (i = 0; i < tamTotVendasProd; i++)
		totVendasProd[i].contrib = (totVendasProd[i].valrTot / totGeralVendas) * 100;

	do
	{
		trocou = false;

		for (i = 0; i < tamTotVendasProd - 1; i++)
		{
			if (totVendasProd[i].contrib < totVendasProd[i + 1].contrib)
			{
				totVendasProdAux = totVendasProd[i];
				totVendasProd[i] = totVendasProd[i + 1];
				totVendasProd[i + 1] = totVendasProdAux;
				trocou = true;
			}
		}

	}	while (trocou);

	printf("\nCONTRIBUICAO DE CADA PRODUTO\n");
	printf("Prod.          VlrTot    Contrib.\n");
	for (i = 0; i < tamTotVendasProd; i++)
		printf("%5d      %10.2lf       %4.1lf%%\n", totVendasProd[i].codProd, totVendasProd[i].valrTot, totVendasProd[i].contrib);

	fprintf(arq, "\nCONTRIBUICAO DE CADA PRODUTO\n");
	fprintf(arq, "Prod.          VlrTot    Contrib.\n");
	for (i = 0; i < tamTotVendasProd; i++)
		fprintf(arq, "%5d      %10.2lf       %4.1lf%%\n", totVendasProd[i].codProd, totVendasProd[i].valrTot, totVendasProd[i].contrib);

	fclose(arq);

	return (0);
}

int dataJaExiste(int dia, int mes, int ano, TTotVendasDia *pV, int tamV)
{
	int i;

	if (pV == NULL)
		return -1;
	else
	{
		for (i = 0; i < tamV; i++)
		{
			if (dia == pV[i].dia && mes == pV[i].mes && ano == pV[i].ano)
				return i;
		}
		return -1;
	}
}

int prodJaExiste(int cod, TTotVendasProd *pV, int tamV)
{
	int i;

	if (pV == NULL)
		return -1;
	else
	{
		for (i = 0; i < tamV; i++)
		{
			if (cod == pV[i].codProd)
				return i;
		}
		return -1;
	}
}

double buscarPrcUnit(int cod, TProdutos *pV, int tamV)
{
	int i;

	if (pV == NULL)
		return -1;
	else
	{
		for (i = 0; i < tamV; i++)
		{
			if (cod == pV[i].codProd)
				return pV[i].prcUnit;
		}
		return -1;
	}
}
