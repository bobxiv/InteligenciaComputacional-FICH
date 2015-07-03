function [i, v] = Sistema(dt)
	%para salvar error se le suma 21
	dt*=-1;
	dt+=21;
	dt=min(dt,44);
	dt=max(dt,1);
	%Conjuntos de entrada
	%ArmarConjunto(centro, pendiente, ancho, x0, x1, dx)
	static FrioCritico = ArmarConjunto(5, 0.2, 16, 0, 45, 0.1);
	static Frio = ArmarConjunto(14.5, 0.2, 13, 0, 45, 0.1);
	static Ideal = ArmarConjunto(21, 0.2, 10, 0, 45, 0.1);
	static Calido = ArmarConjunto(27.5, 0.2, 13, 0, 45, 0.1);
	static CalidoCritico = ArmarConjunto(37, 0.2, 16, 0, 45, 0.1);
	%dibujar los conjuntos de entrada
	%x=0:0.1:45;
	%plot(x,FrioCritico,"1",x,Frio,"2",x,Ideal,"3",x,Calido,"4",x,CalidoCritico,"5");
	%figure;
	%plot(x, FrioCritico+Frio+Ideal+Calido+CalidoCritico);
	
	%Conjuntos de Salida
	%ArmarConjunto(centro, pendiente, ancho, x0, x1, dx)
	static RefMaximo = ArmarConjunto(400, 0.01, 200, -100, 500, 5);
	static RefNormal = ArmarConjunto(200, 0.01, 400, -100, 500, 5);
	static RefApagada = ArmarConjunto(0, 0.01, 200, -100, 500, 5);
	%dibujar los conjuntos de Salida
	%x=-100:5:500;
	%plot(x,RefMaximo,"1",x,RefNormal,"2",x,RefApagada,"5");
	%figure;
	%plot(x, RefMaximo+RefNormal+RefApagada);
	
	%Conjuntos de Salida
	static CalMaximo = ArmarConjunto(10, 0.5, 4, -4, 14, 0.5);
	static CalNormal = ArmarConjunto(5, 0.5, 10, -4, 14, 0.5);
	static CalApagada = ArmarConjunto(0, 0.5, 4, -4, 14, 0.5);
	%dibujar los conjuntos de Salida
	%x=-4:0.5:14;
	%plot(x,CalMaximo,"1",x,CalNormal,"2",x,CalApagada,"5");
	%figure;
	%plot(x, CalMaximo+CalNormal+CalApagada);

	dt=(dt*10)+1;
	%Reglas
	%CalOut y RefOut serian
	CalOut = IF_THEN(PERTENECE(dt, FrioCritico), CalMaximo);
	CalOut += IF_THEN(PERTENECE(dt, Frio), CalNormal);	
	CalOut+=IF_THEN(PERTENECE(dt,UNION(Ideal,Calido,CalidoCritico)),CalApagada);
	%figure(1); plot(CalOut);
	RefOut = IF_THEN(PERTENECE(dt, CalidoCritico), RefMaximo);
	RefOut += IF_THEN(PERTENECE(dt, Calido), RefNormal);
	RefOut+=IF_THEN(PERTENECE(dt,UNION(Ideal, Frio, FrioCritico)),RefApagada);
	%figure(2); plot(RefOut);

	%Defuzzyfico
	i = Centroide( CalOut, -4, 0.5 );
	v = Centroide( RefOut, -100, 5 );
endfunction