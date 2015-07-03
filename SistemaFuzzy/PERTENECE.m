function [y] = PERTENECE(x, c)
	%da over en [0..0.9999999]
	over=x-floor(x);
	%interpolacion del valor de pertenencia de x, entre  floor(x)<= x <= ceil(x)
	y= (1-over)*c(floor(x)) + over*c(ceil(x));
endfunction
	