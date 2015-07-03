clear all;
clf;
%bandera para realizar o no simulacion
simulacion=1;
%dimension en x de la habitacion
Sx=2;
%dimension en y de la habitacion
Sy=2;
%dimension en z de la habitacion
Sz=3;
%calor especifico
CalEsp=1.007;
%densidad del aire 
AirDens=1.25;
%espesor del Ladrillo 
Esp=0.15;
%tiempo en que la puerta este abierta en segundos 
TAperMax=10;
%apertura de la puerta por hora
AperHora=1;
%conductividad termica del material de la pared
CondTerm=0.84;
%resistencia calefactora
Resistencia=1000;
%constante de proporcionalidad de refrigeracion
Refrigeracion=0.02;
%temeratura externa minima
TempExtMin=15;
%temeratura externa maxima
TempExtMax=35;
%tiempo inicial en segundos
tini=1;
%tiempo final en segundos
tfin=7200;
%intervalo de muestreo del tiempo
dt=3;


if(simulacion)
	%cantidad de muestras del tiempo
	Lt=(tfin-tini)/dt+1;
	%area de las 4 paredes y el techo
	Area=(Sx*Sz*2)+(Sy*Sz*2)+(Sx*Sy);
	%volumen de la habitacion
	Volumen=Sx*Sy*Sz;
	%constantes de proporcionalidad
	K1=CondTerm*(Area/Esp)/1000;
	K2=Resistencia/1000;
	K3=-Refrigeracion;
	K4=50*K1;
	%capacitancia termica de la habitacion
	C=CalEsp*AirDens*Volumen;
	%devuelve a(t) que es 1:abierta en el instante t o 0:cerrada en el instante t,(es aleatorio el valor q puede tomar a(t))
	a=AperturasAleatorias(AperHora, TAperMax, tini, tfin, dt);
	%devuelve Tdeseada(t) que es la temperatura deseada en el instante t
	Tdeseada = TemperaturaDeseada(20,22,600, tini, tfin, dt);
	%devuelve TempExt(t) que es la temperatura externa en el instante t
	TempExt=TemperaturaExterna(TempExtMin,TempExtMax,tini,tfin,dt);
	%declaracion del registro de la temperatura(t) a lo largo del tiempo que esta por calcularse
	Temp = zeros(Lt,1);
	%declaracion del registro de la corriente i(t) en el calefactor a lo largo del tiempo que esta por calcularse
	i=Temp;
	%declaracion del registro de la tension(t) en el refrigerador a lo largo del tiempo que esta por calcularse
	v=Temp;
	%la temperatura inicial es la temperatura externa inicial
	Temp(1) = TempExt(1,1);
	%comienza el calculo para la temeratura(t)
	for t=2:1:Lt
		%calculo de la coriente i(t) y la tension v(t) en el instante t
		[it, vt]=Sistema(Tdeseada(t-1,1)-Temp(t-1,1));
		i(t,1)=it;
		v(t,1)=vt;
		%calculo de la temeratura(t) segun la discretizacion de la ecuacion diferencial que gobierna su comportamiento
		Temp(t,1)=(C*Temp(t-1,1)+dt*(K1+a(t)*K4)*TempExt(t,1)+dt*K2*(i(t)^2)+dt*K3*v(t));
		Temp(t,1) /= (C+dt*K1+dt*a(t)*K4);
	endfor
	%finalizó el calculo para la temeratura(t)
	%ploteos de los resultados de la corriente i(t),tension(t),temperatura(t),temperaturadeseada(t),temeraturaexterna(t)
	t=tini:dt:tfin;
	t=t';
	subplot(2,2,1);
	plot(t, i);
	title("Corriente en la resistencia");
	subplot(2,2,2);
	plot(t, v);
	title("Voltaje en la refrigeracion");
	subplot(2,1,2);
	plot(t, Temp, "1", t, Tdeseada, "2", t, TempExt, "3");
	legend("Temperatura real", "Temperatura deseada", "Temperatura externa", "location", "southeast");
	legend("boxon");
	title("Temperatura en el cuarto");
else
	i=zeros(1,41);
	v=i;
	%comienza el calculo para la corriente i(t),tension(t)
	for x=1:41
		[it, vt]=Sistema(x-21);
		i(1,x)=it;
		v(1,x)=vt;
	endfor
	%finalizó el calculo para la corriente i(t),tension(t)
	%ploteos de los resultados de la corriente i(t),tension(t)
	subplot(2,1,1)
	plot((-20:20), i);
	title("Corriente en la resistencia");
	
	subplot(2,1,2)
	plot((-20:20), v);
	title("voltaje de refrigeracion");
endif




