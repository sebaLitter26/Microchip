#include <stdio.h>
#include <stdlib.h>
#define COLA_VACIA 0
#define SACO_OK 1

<<<<<<< HEAD
typedef struct
=======
typedef sturct
>>>>>>> parent of 709d27e... modificacion de roots
{
	int dato;
}t_info;

<<<<<<< HEAD
typedef struct s_nodo
=======
typedef struct
>>>>>>> parent of 709d27e... modificacion de roots
{
	t_info info;
	struct s_nodo *sig;
}t_nodo;

typedef struct
{
	t_nodo *pri;
	t_nodo *ult;
}t_cola;

<<<<<<< HEAD
int main(void)
=======
int main
>>>>>>> parent of 709d27e... modificacion de roots
{
	t_cola cola;
	return 0;
}

int colaLlena (const t_cola *p)
{
	void* aux=malloc(sizeof(t_nodo));
	free(aux);
<<<<<<< HEAD
	return !aux;
=======
	return aux==null;
>>>>>>> parent of 709d27e... modificacion de roots
}

int colaVacia(const t_cola *p)
{
<<<<<<< HEAD
	return !p->pri;
=======
	return p->pri==null;
>>>>>>> parent of 709d27e... modificacion de roots
}

void crearCola(t_cola *p)
{
<<<<<<< HEAD
	p->pri=NULL;
	p->ult=NULL;
=======
	p->pri=0;
	p->ult=-1;
>>>>>>> parent of 709d27e... modificacion de roots
}

int ponerCola(t_cola *p, const t_info *d)
{
	t_nodo *nue;
	nue=(t_nodo *)malloc(sizeof(t_nodo));
	if(!nue)
		return 0;
	nue->info=*d;
<<<<<<< HEAD
	nue->sig= NULL;
	if(p->pri==NULL)
=======
	nue->sig=null;
	if(p->pri==null)
>>>>>>> parent of 709d27e... modificacion de roots
		p->pri=nue;
	else
		p->ult->sig=nue;
	p->ult=nue;
	return 1;
}

int sacarCola(t_cola *p, t_info *d)
{
	t_nodo *aux;
<<<<<<< HEAD
	if(p->pri==NULL)
=======
	if(p->pri==null)
>>>>>>> parent of 709d27e... modificacion de roots
		return COLA_VACIA;
	aux=p->pri;
	*d=aux->info;
	p->pri=aux->sig;
	free(aux);
<<<<<<< HEAD
	if(p->pri==NULL)
		p->ult=NULL;
=======
	if(p->pri==null)
		p->ult=null;
>>>>>>> parent of 709d27e... modificacion de roots
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
<<<<<<< HEAD
	p->ult=NULL;
=======
	p->ult=null;
>>>>>>> parent of 709d27e... modificacion de roots
}

int verPri(const t_cola *p, t_info *d)
{
<<<<<<< HEAD
	if(p->pri==NULL)
		return 0;
	*d=p->pri->info;
=======
	if(p->pri==null)
		return 0;
	*d=p->pri->d;
>>>>>>> parent of 709d27e... modificacion de roots
	return 1;
}

