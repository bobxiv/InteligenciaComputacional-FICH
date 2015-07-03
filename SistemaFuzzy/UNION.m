function [r] = UNION(varargin)
	r=varargin{1};
	for i=2:nargin
		t=varargin{i};
		r = max(r, t);
	endfor
endfunction