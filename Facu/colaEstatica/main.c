#include <stdio.h>
#include <stdlib.h>
#include "cola.h"



int main()
{
	t_info info;
	t_cola cola;
	crearCola(&cola);
	if(!colaVacia(&cola))
	{
		vaciarCola(&cola);
	}

	while(!colaLlena(&cola))
	{
	    puts("Ingrese nuevo registro: ");
		cargarInfo(&info);
		if(ponerEnCola(&cola, &info))
			puts("Puso OK");
	}

	while(!colaVacia(&cola))
	{
	    if(verPrimero(&cola,&info))
			mostrarInfo(&info);
		if(sacarDeCola(&cola, &info))
			puts("\nSaque de cola");

	}
	return 0;
}


void crearCola(t_cola *p)
{
	p->pri=0;
	p->ult=-1;
}

int colaVacia(const t_cola *p)
{
	return p->ult==-1;
}

int colaLlena(const t_cola *p)
{
	return p->pri==0 && p->ult==TAM-1 || p->ult+1==p->pri && p->ult!=-1;
}

void vaciarCola(t_cola *p)
{
	p->pri=0;
	p->ult=-1;
}

int ponerEnCola(t_cola *p, const t_info *d)
{
	if(p->pri==0 && p->ult==TAM-1 || p->ult+1==p->pri && p->ult!=-1)  //cola llena
		return 0;
	p->ult++;
	if(p->ult==TAM)
		p->ult=0;
	p->cola[p->ult]= *d;
	return 1;
}

int sacarDeCola(t_cola *p, t_info *d)
{
	if(p->ult==-1)   //esta vacia
		return 0;
	*d = p->cola[p->pri];
	if(p->pri==p->ult)   //si tiene un solo elemento pri y ult van a ser igules
	{
		p->pri=0;
		p->ult=-1;
	}else{
		p->pri++;
		if(p->pri==TAM)
			p->pri=0;
	}
	return 1;
}

int verPrimero(const t_cola *p, t_info *d)
{
	if(p->ult==-1)
		return 0;
	*d=p->cola[p->pri];
	return 1;
}

int cargarInfo(t_info *d)
{
	scanf("%d",d);
}

void mostrarInfo(t_info *d)
{
	printf("%d",*d);
}


