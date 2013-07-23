function is = essential(A, b, j);
% function [result] = essential(A, b, j);
%      decides wether the j'th inequality is essential for Ax <= b

cty = repmat("U", 1, size(b)(1));
cty(j) = "F";
[x, m] = glpk(A(j,:), A, b,[],[], cty, repmat("C", 1, size(A)(2)), -1);

is = m > b(j);
end
