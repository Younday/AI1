% isnumber(X) is true if X is a isnumber

isnumber(0).
isnumber(s(X)) :- isnumber(X).

% plus(X,Y,Z) is true if X + Y = Z

plus(0,X,X) :- isnumber(X).
plus(s(X),Y,s(Z)) :- plus(X,Y,Z).

% minus(X,Y,Z) is true if X - Y =Z

minus(X,0,X) :- isnumber(X).
minus(s(X),s(Y),Z) :- minus(X,Y,Z).

% times(X,Y,Z) is true if X * Y = Z

times(X,0,0) :- isnumber(X).
times(X,s(Y),Z) :- times(X,Y,Z1), plus(X,Z1,Z).

% pow(X,Y,Z) is true if X^Y = Z

pow(X,0,s(0)) :- isnumber(X).
pow(X,s(Y),Z) :- pow(X,Y,Z1), times(X,Z1,Z).

% even(X) is true if X is even
even(0) :- isnumber(0).
even(s(s(X))) :- even(X).

% odd(X) is true if X is odd
odd(s(0)) :- isnumber(s(0)).
odd(s(s(X))) :- odd(X).

% div2(X,Y) is true if X div 2 is Y
div2(0, 0) :- isnumber(0).
div2(1, 0) :- isnumber(0).
div2(X, Y) :- plus(Y, Y, X), even(X).
div2(X, Y) :- plus(s(Y), Y, X), odd(X).

divi2(0, 0) :- isnumber(0).
divi2(1, 0) :- isnumber(0).
divi2(X, Y) :- even(X), times(s(s(0)), Y, X).
divi2(X, Y) :- odd(X), times(s(s(0)), Y, Z1), plus(s(0), Z1, X).

% log(X, B, N) is true if B ^ N = X
log(X, B, 0) :- isnumber(s(0)).
log(X, B, N) :- pow(B, N, X).

% fib(X, Y) is true if fib(X) = Y
fib(0, 0).
fib(s(0), s(0)).
fib(X, Y) :- minus(X, s(0), X1), minus(X, s(s(0)), X2), plus(Y1,Y2,Y), fib(X1, Y1), fib(X2, Y2).

% power(X,N,Y) is true if X^N = Y
power(X, N, Y) :- even(N), div2(N,C), pow(X, s(s(0)), D), pow(D, C, Y).
power(X, N, Y) :- odd(N), minus(N, s(0), Q), pow(X,Q,L), times(X, L, Y).

% len(T, N)
len([],0).
len([H|T],N) :- len(T,N1), N is N1+1.

% member(X, L) is true if X is a member of list L
member(X, [X|_]).
member(X, [_|T]) :- member(X, T).

% concat(L, X, Y) is true if L is the concatenation of the lists X and Y
concat(L, X, Y) :- append(X, Y, L).

% reverse(L, R) is true if R is the reversal of L
reverse([], []).
reverse([H|T], R) :- reverse(T, RT), append(RT, [H], R).

% palindrome(L) returns true if L is a palindrome.
palindrome(L) :- reverse(L, L).

% Example queries:
% Isnumbers are represented as successors of 0. For example, 2 is s(s(0)).
% 2+2=4 is plus(s(s(0)), s(s(0)), s(s(s(s(0)))))
% 3*2=? is times(s(s(s(0))), s(s(0)), X)
