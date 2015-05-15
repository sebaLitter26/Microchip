#include <stdio.h>
#include <stdlib.h>
#define TAM 5

typedef struct{
	ind dato;
}t_info;

typedef struct{
	t_info cola[TAM];
	int pri;
	int ult;
}t_cola;

void crearCola(t_cola *);
int colaVacia(const t_cola *);
int colaLlena(const t_cola *);
void vaciarCola(t_cola *);
int cargarInfo(t_info *);
int ponerEnCola(t_cola *, const t_info *);
int sacarDeCola (t_cola * , const t_info *);
int verPrimero(const t_cola *, t_info *);
void mostrarInfo(t_info *);


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
	if(p->pri==0 && p->ult==TAM-1 || p->ult+1 && p->ult!=-1)
		return 0;
	p->ult++;
	if(p->ult==TAM)
		p->ult=0;
	p->cola[p->ult];
	return 1;
}

int sacarDeCola(t_cola *p, const t_info *d)
{
	if(p->ult==-1)
		return 0;
	*d=p->cola[p->pri];
	if(p->pri==p->ult)
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
	printf("%d",d);
}


int main()
{
	t_info info;
	t_cola cola;
	crearCola(&cola);
	if(!colaVacia(&cola))
	{
		vaciarCola(&cola);
	}
	if(!colaLlena(&cola))
	{
		cargarInfo(&info);
		if(ponerEnCola(&cola, &info))
			puts("Puso OK");
	}
	if(!colaVacia(&cola))
	{
		if(sacarDeCola(&cola, &info))
			puts("Saque de cola");
		if(verPrimero(&cola,&info))
			mostrarInfo(&info);	
	}
	return 0;
}