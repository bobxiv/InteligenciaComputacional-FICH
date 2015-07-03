
function [a] = AperturasAleatorias(prob, Tape, tini, tfin, dt)
	%prob:%apertura de la puerta por hora
	%Tape: tiempo en segundo en que la puerta esta abierta
	
	%cantidad de muestras del tiempo
	Lt=(tfin-tini)/dt+1;
	%declaracion de las aperturas aleatorias a(t) donde t es un instasnte de tiempo en segundos 
	%3600*ceil( Lt/3600 ) da el inervalo de observacion en segundos
	a = zeros( 3600*ceil( Lt/3600 ), 1);
	prob = ceil(prob);
	%calcular el salto segun el intervalo de muestreo
	jump=3600/dt;
	%calcular el tiempo de apertura de la puerta segun el intervalo de muestreo
	Tape=ceil(Tape/dt);
	for ini=1:jump:Lt
		for cont=1:prob
			pos = floor(rand() * (jump-Tape)) + ini;
			posl= pos+Tape;
			%rellena de 1 implicando que en esos instantes de tiempo esta abierta la puerta
			a(pos:posl) = ones(Tape+1,1);
		endfor
	endfor
	a = a(1:Lt);
endfunction