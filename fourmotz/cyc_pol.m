function [A,b] = cyc_pol(dim, rows);

A = vander(1:rows)(:,rows-dim:rows-1);

A = rows*A-ones(rows,1)*sum(A);

b = ones(rows,1);

end
