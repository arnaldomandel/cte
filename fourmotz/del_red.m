function [A, b] = del_red(A, b);
% function [result] = del_red(A, b);
%    Eliminate redundant inequalities from Ax <= b

  i = 1;

  while i <= size(b)(1)
    if essential(A, b, i)
      i = i + 1;
      fprintf(stderr,"%d\r",i);
    else
      A(i,:) = [];
      b(i,:) = [];
    endif
end
