#ifndef COLA_H_INCLUDED
#define COLA_H_INCLUDED
#define TAM 2
#define ERROR_ARCHIVO -6
#define TODO_OK 3


typedef struct{
    int dia;
    int mes;
    int anio;
}t_fecha;

//t_empleado
typedef struct{
	int dni;
	char apyn[51];
	char sexo;
	t_fecha f_nac;
	float sueldo;
}t_empleado;


typedef struct
{
    t_empleado cola[TAM];
    int pri;
    int ult;
}t_cola;


void menu(int *);
void ejecutar(int, t_cola* ,t_cola *,t_empleado *);
void crearCola(t_cola *);
int colaVacia(const t_cola *);
int colaLlena(const t_cola *);
void vaciarCola(t_cola *);
void cargarInfo(t_empleado *);
int ponerEnCola(t_cola *, const t_empleado *);
int sacarDeCola(t_cola *,t_empleado *);
int verPrimero (const t_cola * ,t_empleado *);
void mostrarInfo(t_empleado *);
void grabar_empleado_var(FILE *arch_txt, const t_empleado *emp);
void grabar_linea_variable(FILE* arch_bin, char * linea);
int texto_a_binario(const char *path_txt, const char* path_bin);
void grabar_empleado_fijo(FILE *arch_txt,const t_empleado * emp);
int binario_a_texto(const char * path_bin,const char * path_txt);
void cargar_arch_bin(const char * path_bin, t_cola *cola,t_empleado *emp);
void guardar_arch_bin(const char * path_bin,t_cola *cola,t_cola * cola2,t_empleado * emp);


#endif // COLA_H_INCLUDED
