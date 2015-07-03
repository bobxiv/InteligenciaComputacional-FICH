function [T] = TemperaturaExterna(minima, maxima, tini, tfin, dx)
	t=tini:dx:tfin;
	t=(t.-43200);  %12*3600=43200
	T = (-(t.*t))';
	T /= -(T( 1 )/(maxima-minima));
	T = T .+ (maxima);
endfunction