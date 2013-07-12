function A = matexempl(n, s);
% function [result] = matexempl(n, s);
% BenTal-Nemirovski matrix, size n, s = sin \theta
%    calculate c = cos \theta
%    columns: 2(n+1), in pairs, indexed 1,2,...,n,0
%             (prepared for good use of fourmotz as is)
%    rows correspond to inequalities:
%              Xn + (1-c)/s Yn <= 1
%              Xn - s/c     Yn <= 0
%                   -Yn        <= 0
%        for 1 <= j <= n
%              Tj = 2^(j-1) \theta
%
%              X_{j-1} cos T_{n-j} + Yj sin T_{n-j} - Xj   <= 0
%           | -X_{j-1} sin T_{n-j} + Yj cos T_{n-j} - Yj | <= 0


c = sqrt(1 - s*s);

## Top matrix
T = [1, (1-c)/s; 1, -s/c; 0, -1];
## Fill with zeros to the left
A = [ zeros(3, 2*n), T ];


## Remaining rows
for j = 0:(n-1)
  T = [ c , s, -1, 0; -s, c, 0, -1; s, -c, 0, -1];
  A = [ A; zeros(3, 2*(n-j-1)), T, zeros(3, 2*j) ];
  ## Double the angle for the next step
  t = 2 * s * c;
  c = 2 * c * c - 1;
  s = t;
endfor

A = shift(A, -2,2);
end
