function [c] = ArmarConjunto( centro, pendiente, ancho, x0, x1, dx)
	%intervalo
	x=x0:dx:x1;
	%te arma un triangulo en el eje y negativo
	c=-abs(x.-centro)*pendiente;
	%te lo sube al triangulo hacia +y  segun el ancho deseado
	c=c.+ancho*pendiente/2;
	%te recorta la base haciendo que el min sea 0 y te recorta la altura haciendo que el max sea 1
    %puede entonces convertirse en un trapecio de altura 1	
	c=max(c,0);
	c=min(c,1);
endfunction