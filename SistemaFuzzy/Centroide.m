function [r] = Centroide(c, x0, dx)
	r=0;
	t=0;
	n=max(size(c));
	for i=1:(n-1)
		iact=(i-1)*dx+x0;
		inext=iact+dx;
		r += (iact+inext) * (c(i)+c(i+1)) / 2;
		t += (c(i)+c(i+1));
	endfor
	r/=t;
endfunction