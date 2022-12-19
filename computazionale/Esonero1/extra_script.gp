set title 'Fit lineare di P(100) in funzione di {/Symbol D}r^2'
set xlabel '{/Symbol D}r^2'
set ylabel 'P(100)'
f(x)=a*x+b
fit f(x) 'extra.dat' via a,b
print f(0)

plot  'extra.dat' u 1:2  t 'P_{100}({/Symbol D}r^2)'  pt 7, f(x) t 'y=m*{/Symbol D}r^2 + P_{esatto}'

plot  'extra.dat' u 1:2  t 'P_{100}({/Symbol D}r^2)' w linesp  pt 7