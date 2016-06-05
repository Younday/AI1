
edge(a,b).
edge(b,f).
edge(f,e).
edge(f,j).
edge(j,k).
edge(k,g).
edge(g,c).
edge(c,d).
edge(d,h).
edge(h,l).
edge(l,p).
edge(i,m).
edge(m,n).
edge(n,o).

path(X, Y) :- edge(X, Y).
path(X, Y) :- path(Z, Y), edge(X, Z).
