#include <stdio.h>
#include <stdlib.h>
#define COLA_VACIA 0
#define SACO_OK 1

typedef sturct
{
	int dato;
}t_info;

typedef struct
{
	t_info info;
	struct s_nodo *sig;
}t_nodo;

typedef struct
{
	t_nodo *pri;
	t_nodo *ult;
}t_cola;

int main
{
	t_cola cola;
	return 0;
}

int colaLlena (const t_cola *p)
{
	void* aux=malloc(sizeof(t_nodo));
	free(aux);
	return aux==null;
}

int colaVacia(const t_cola *p)
{
	return p->pri==null;
}

void crearCola(t_cola *p)
{
	p->pri=0;
	p->ult=-1;
}

int ponerCola(t_cola *p, const t_info *d)
{
	t_nodo *nue;
	nue=(t_nodo *)malloc(sizeof(t_nodo));
	if(!nue)
		return 0;
	nue->info=*d;
	nue->sig=null;
	if(p->pri==null)
		p->pri=nue;
	else
		p->ult->sig=nue;
	p->ult=nue;
	return 1;
}

int sacarCola(t_cola *p, t_info *d)
{
	t_nodo *aux;
	if(p->pri==null)
		return COLA_VACIA;
	aux=p->pri;
	*d=aux->info;
	p->pri=aux->sig;
	free(aux);
	if(p->pri==null)
		p->ult=null;
	return SACO_OK;
}

int vaciarCola(t_cola *p)
{
	t_nodo *aux;
	while(p->pri)
	{
		aux=p->pri;
		p->pri=aux->sig;
		free(aux);
	}
	p->ult=null;
}

int verPri(const t_cola *p, t_info *d)
{
	if(p->pri==null)
		return 0;
	*d=p->pri->d;
	return 1;
}

