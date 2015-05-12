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