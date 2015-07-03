function [r] = umbral(v)
	r = 2 / (1+exp(-0.1*v))-1;
endfunction
