#include <stdio.h>
#include <stdlib.h>
#include "cola.h"

int main()
{
    int opcion,*op;
    op=&opcion;
    FILE *pf;
    t_empleado info;
    t_cola cola;
    t_cola cola2;
    crearCola(&cola);
    crearCola(&cola2);
    do
    {
        menu(op);
        ejecutar(opcion,&cola,&cola2,&info);
    }while(opcion!=9);
    return 0;
}

void menu(int *op)
{
    do{
        system("cls");
        printf("Menu: \n");
        printf("1 - Cargar Cola\n2 - Mostrar elementos\n3 - Vaciar Cola\n4 - Ver Tope\n5 - Guardar en cola.bin\n6 - Cargar de cola.bin\n7 - Pasar de binario a texto\n8 - Grabar de texto a binario\n9 - Salir\n\n");
        printf("Ingrese Comado: ");
        scanf("%d",op);
    }while(*op<1 || *op>9);
}

ejecutar(int opcion, t_cola *cola,t_cola *cola2,t_empleado *info)
{
    switch(opcion)
    {
        case 1:
            puts("Cargar registro");
            if(!colaLlena(cola))
            {
                while(!colaLlena(cola))
                {
                    puts("Ingrese un registro: \n");
                    cargarInfo(info);
                    //mostrarInfo(&info);
                    if(ponerEnCola(cola,info))
                        puts("Se agrego un registro\n");
                }
            }else
                puts("La cola ya esta cargada.\n\n");
            break;
        case 2:
            if(!colaVacia(cola))
            {
                while(!colaVacia(cola))
                {
                    //if(verPrimero(cola,info))
                      //  mostrarInfo(info);
                    if(sacarDeCola(cola,info))
                    {
                        mostrarInfo(info);
                        puts("\nSaque de Cola\n");
                    }
                }
            }else
                puts("La cola esta vacia.\n\n");
            break;
        case 3:
            if(!colaVacia(cola))
            {
                vaciarCola(cola);
                puts("cola vacia.\n\n");
            }
            break;
        case 4:
            if(!colaVacia(cola))
            {
                if(verPrimero(cola,info));
                {
                    puts("Primero: \n");
                    mostrarInfo(info);
                }
            }else
                puts("La cola esta vacia.\n\n");
            break;
        case 5:
            if(!colaVacia(cola)){
               guardar_arch_bin("cola.bin",cola, cola2,info);
            }else
                puts("La cola esta vacia.\n\n");
            break;
        case 6:
            cargar_arch_bin("cola.bin",cola,info);
            break;
        case 7:
            if(TODO_OK==binario_a_texto("cola.bin","cola.txt"))
                puts("Se convirtio de binario a texto\n");
            else
                puts("Error al querer convertir de binario a texto\n");
            break;
        case 8:
            if(TODO_OK==texto_a_binario("cola.txt", "cola.bin"))
                puts("Se convirtio de texto a binario\n");
            else
                puts("Error al querer convertir de texto a binario\n");
            break;

    }
    system("pause");
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

int ponerEnCola(t_cola *p,const t_empleado *d)
{
    if(colaLlena(p))   //puede haber problemas
        return 0;
    p->ult++;
    if(p->ult==TAM)
        p->ult=0;
    p->cola[p->ult]=*d;
    return 1;
}

int sacarDeCola(t_cola *p, t_empleado *d)
{
    if(p->ult==-1)
        return 0;
    *d = p->cola[p->pri];
    if(p->pri==p->ult)
    {
        p->pri=0;
        p->ult=-1;
    }
    else
        p->pri=(p->pri+1)%TAM;
    return 1;
}

int verPrimero(const t_cola *p,t_empleado *d)
{
    if(p->ult==-1)
        return 0;
    *d=p->cola[p->pri];
    return 1;
}

int cargarInfo(t_empleado *d)
{
    puts("Ingrese DNI: ");
    scanf("%d",&d->dni);

    puts("Ingrese Nombre: ");
    scanf("%s",&d->apyn);
    puts("Ingrese Sexo (M/F): ");
    scanf(" %c",&d->sexo);
    puts("Ingrese Fecha de Nacimiento (dia/mes/anio): ");
    scanf("%d/%d/%d",&d->f_nac.dia,&d->f_nac.mes,&d->f_nac.anio);
    puts("Ingrese Sueldo: ");
    scanf("%f",&d->sueldo);
}

void mostrarInfo(t_empleado *d)
{
    printf("DNI: %d\n",d->dni);
    printf("Nombre: %s\n",d->apyn);
    printf("Sexo: %c\n",d->sexo);
    printf("Fecha de Nacimiento: %d/%d/%d\n",d->f_nac.dia,d->f_nac.mes,d->f_nac.anio);
    printf("Sueldo: %f\n",d->sueldo);

}



void grabar_linea_variable(FILE* arch_bin, char * linea)
{
	t_empleado emp;

	//sueldo
	char * pact = strchr(linea,'\n');
	*pact = '\0';  //desreferenciamos el puntero    asi reemplazamos el caracter
	pact = strchr(linea, '|');  //lee de atras para adelante    busca ' | '
    printf("%s\n",pact+1);
    sscanf(pact, "%d|%s|%c|%d/%d/%d|%f\n",emp.dni,emp.apyn,emp.sexo,emp.f_nac.dia,emp.f_nac.mes,emp.f_nac.anio, emp.sueldo);
	/*
	sscanf(pact+1, "%f", &emp.sueldo);    //

	//fecha
	*pact = '\0';
	pact = strchr(linea , '|');
	printf("%s",pact+1);
	scanf(pact+1,"%d/%d/%d",&emp.f_nac.dia,&emp.f_nac.mes,&emp.f_nac.anio);

	//sexo
	*pact ='\0';
	pact=strchr(linea,'|');
	emp.sexo=*(pact+1);

	//apyn
	*pact = '|';
	pact = strchr(linea, '|');
	*(pact+51)='\0';
	strcpy(emp.apyn, pact+1);

	*pact ='\0';
	sscanf(linea, "%d", &emp.dni);
*/
    mostrarInfo(&emp);
    fwrite(&emp, sizeof(t_empleado),1,arch_bin);
}

int texto_a_binario(const char *path_txt, const char* path_bin)
{
	char linea[500];  //donde guardamos la linea que leemos del archivo
	t_empleado emp;  //struct empleado
	FILE * arch_txt = fopen(path_txt, "r+");
	FILE * arch_bin = fopen(path_bin,"wb");
	if(!arch_bin || !arch_txt)
	{
		if(arch_bin)
			fclose(arch_bin);
		if(arch_txt)
			fclose(arch_txt);
		return ERROR_ARCHIVO;
	}
	fgets(linea,500,arch_txt);   //obtiene una linea desde el texto

	while(!feof(arch_txt))    //si fuera binario usamos fread, como es texto leemos lineas
	{
		grabar_linea_variable(arch_bin,linea);
		fgets(linea,500,arch_txt);
	}
	fclose(arch_txt);
	fclose(arch_bin);
	return TODO_OK;

}

void grabar_empleado_fijo(FILE *arch_txt,const t_empleado * emp)
{
	fprintf(arch_txt,"%08d%-50s       %c %02d %02d %4d %09.2f \h",emp->dni,emp->apyn,emp->sexo,emp->f_nac.dia,emp->f_nac.mes,emp->f_nac.anio,emp->sueldo);
}

void grabar_empleado_var(FILE *arch_txt, const t_empleado *emp)
{
	fprintf(arch_txt, "%d|%s|%c|%d/%d/%d|%.2f\n",emp->dni,emp->apyn,emp->sexo,emp->f_nac.dia,emp->f_nac.mes,emp->f_nac.anio, emp->sueldo);

}


int binario_a_texto(const char * path_bin,const char * path_txt)
{
	t_empleado emp;
	FILE * arch_bin = fopen(path_bin,"rb");
	FILE * arch_txt = fopen(path_txt,"r+");
	if(!arch_bin || !arch_txt)
	{
		if(arch_bin)
			fclose(arch_bin);
		if(arch_txt)
			fclose(arch_txt);
		return ERROR_ARCHIVO;
	}
	fread(&emp,sizeof(t_empleado),1,arch_bin);
	while(!feof(arch_bin))
	{
		grabar_empleado_var(arch_txt,&emp);
		fread(&emp,sizeof(t_empleado),1,arch_bin);
	}
	fclose(arch_txt);
	fclose(arch_bin);
	return TODO_OK;
}

void cargar_arch_bin(const char * path_bin, t_cola *cola,t_empleado *emp)
{
    FILE * pf;
    if(!(pf=fopen(path_bin,"rb")))
        printf("No se pudo abrir el archivo %s\n",path_bin);
    else{
        printf("%s se cargo correctamente\n",path_bin);
        fread(cola,sizeof(t_cola),1,pf);
        while(!feof(pf))
        {
            mostrarInfo(emp);
            fread(cola,sizeof(t_cola),1,pf);
        }
        fclose(pf);
    }
}

void guardar_arch_bin(const char * path_bin,t_cola *cola,t_cola * cola2,t_empleado * emp)
{
    FILE * pf;

    if(!(pf=fopen(path_bin,"wb")))
        printf("No se pudo abrir el archivo %s\n",path_bin);
    else{
        while(!colaVacia(cola))
        {
            sacarDeCola(cola,emp);
            if(!colaLlena(cola2))
                ponerEnCola(cola2,emp);
        }
        while(!colaVacia(cola2))
        {
            sacarDeCola(cola2,emp);
            ponerEnCola(cola,emp);
            fwrite(cola,sizeof(t_cola),1,pf);
        }
        printf("%s se grabo correctamente\n",path_bin);
        fclose(pf);
    }
}


