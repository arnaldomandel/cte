function [A, b] = genpoly(n, s);
% function [result] = genpoly(n, s);
% Apply Fourier-Motzkin to system based on
%    BenTal-Nemirovski matrix, size n, s = sin \theta

if s == 0
  s = .5 ** n;
endif

A = matexempl(n, s);
b = [ 1; zeros(3*n+2,1) ];

[A, b] = fourmotz(A, b, 2);

A = A ./ [b,b];

end
