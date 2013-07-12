function n = vertices(A);
% function [result] = vertices(A);
%   number of extreme points of  Ax <= 1

n = size(convhull(A(:,1), A(:,2)))(1);

end
