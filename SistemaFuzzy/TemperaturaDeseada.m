function [t] = TemperaturaDeseada(tmin, tmax, int, tini, tfin, dt)
	%int : intervalo
	Lt=(tfin-tini)/dt+1;
	int=ceil(int/dt);
	t = tmin*ones(Lt+int,1);
	for pos=int:(int*2):Lt
		t(pos:(pos+int)) = tmax*ones(int+1,1);
	end
	%ejemplo de t=[tmin tmin tmin tmax tmax tmax tmin tmin tmin...] aca el int= 3
	t = t(1:Lt);
endfunction