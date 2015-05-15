//t_empleado
typedef struct{
	int dni;
	char apyn[51];
	char sexo;
	t_fecha f_nac;
	float sueldo;
}t_empleado;

void grabar_linea_variable(FILE* arch_bin, char * linea)
{
	t_empleado emp;
	
	//sueldo
	char * pact = strchr(linea,'\n');
	*pact = '\o';  //desreferenciamos el puntero    asi reemplazamos el caracter
	pact = strchr(linea, '|');  //lee de atras para adelante    busca ' | '
	scanf(pact+1, "%f", &emp.sueldo);    //
	
	
	//fecha
	*pact = '\o';
	pact = strchr(linea , '|');
	sscanf(pact+1,"%d/%d/%d",&emp.f_nac.dia,$emp.f_nac.mes,&emp.f_nac.anio);
	
	//sexo
	*pact ='\o';
	pact=strchr(linea,'|');
	emp.sexo=*(pact+1);
	
	//apyn
	*pact = '|';
	pact = strchr(linea, '|');
	*(pact+51)='\o';
	strcpy(emp.apyn, pact+1);
	
	*pact ='\o';
	sscanf(linea, "%d", &emp.dni);
	fwrite(&emp, sizeof(t_empleado),1,"arch.bin");
}

void grabar_empleado_var(FILE *arch_txt, const t_empleado *emp)
{
	fprintf(arch_txt, "$d|%s|%c|%d/%d/%d|%.2f\n",emp->dni,emp->apyn,emp->sexo,emp->f_nac.dia,emp->f_nac.mes,emp->f_nac.anio, emp->sueldo);
}

int texto_a_binario(const char *path_txt, const char* path_bin)
{
	char linea[500];  //donde guardamos la linea que leemos del archivo
	t_empleado emp;  //struct empleado
	FILE * arch_txt = fopen(path_txt, "rt");
	FILE * arch_bin = fopen(path_bin,"wb");
	if(!arch_bin || !arch_txt)
	{
		if(arch_bin)
			fclose(arch_bin);
		if(arch_txt)
			fclose(arch_txt);
		return error_archivo;
	}
	fgets(arch_txt,linea,500);   //obtiene una linea desde el texto

	while(!feof(arch_txt))    //si fuera binario usamos fread, como es texto leemos lineas
	{
		grabar_linea(arch_bin,linea);
		fgets(arch_txt,linea,500);
	}
	fclose(arch_txt);
	fclose(arch_bin);
	return TODO_OK;
}

void grabar_empleado_fijo(FILE *arch_txt,const t_empleado * emp)
{
	fprintf(arch_txt,"%08d%-50s       %c %02d %02d %4d %09.2f \h",emp->dni,emp->apyn,emp->sexo,emp->f_nac.dia,emp->f_nac.mes,emp->f_nac.anio,emp->sueldo);
}


int binario_a_texto(const char * path_bin,const char * path_txt)
{
	t_empleado emp;
	FILE * arch_bin = fopen(path_bin,"rb");
	FILE * arch_txt = fopen(path_txt,"wb");
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
		grabar_empleado(arch_txt,&emp);
		fread(&emp,sizeof(t_empleado),1,arch_bin);
	}
	fclose(arch_txt);
	fclose(arch_bin);
	return TODO_OK;
}


