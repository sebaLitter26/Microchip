#ifndef COLA_H_INCLUDED
#define COLA_H_INCLUDED

#define TAM 5

typedef struct{
	int dato;
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
int sacarDeCola (t_cola * , t_info *);
int verPrimero(const t_cola *, t_info *);
void mostrarInfo(t_info *);


#endif // COLA_H_INCLUDED
