function X = findsols(n, s, pr = 1);
% function [result] = genpoly(n, s);
% Apply Fourier-Motzkin to system based on
%    BenTal-Nemirovski matrix, size n, s = sin \theta

if s == 0
				#s = .5 ** n;
  s=2*2**(n+1)/(4**(n+1)+1);
endif

A = bentalnem(n, s);
b = [ 1; zeros(3*n+2,1) ];

tantheta = s/sqrt(1 - s*s);
#tantheta = (s+sqrt(4+s*s))/(2*s);
t = 0;
vars = repmat("C", 1, size(A)(2));
cty = ["UUU",repmat("SUU",1,n)];
c = [1,t,zeros(1,2*n)];
X=[];
for i = 1:2**n
  t = (t + tantheta)/(1-t*tantheta);
  c(2) = t;
  [x, m] = glpk(c, A, b, [], [], cty, vars, -1);
  X = [X;x(1),x(2)];
  if pr
    fprintf(stderr, "%d [%g,%g] %g (%g)\n", i,x(1),x(2), m, x(1)**2+x(2)**2);
  endif;
#  fprintf(stderr,"%g ",x);
#  fprintf(stderr,"\n");
endfor 

if all(diff(X(:,2))>0)
  fprintf(stderr, "\n\n%d distinct vertices.\n", 2**n);
else
  fprintf(stderr, "\n\nLess than %d vertices.\n", 2**n);
endif

end
