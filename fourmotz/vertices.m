function n = vertices(A);
% function [result] = vertices(A);
%   number of extreme points of  Ax <= 1
%   A has just two columns

n = size(convhull(A(:,1), A(:,2)))(1);

end
