set term wxt 0
set title "Analisi logaritmica dell'errore"
set xlabel "{/Symbol D}r"
set ylabel "{/Symbol D}P"
f(x)=a*x+b
fit f(x) 'err_analysis.dat' u (log10($1)):(log10($2)) via a,b
plot [-5.5:-1.5][-11:-2] 'err_analysis.dat' u (log10($1)):(log10($2)) t 'log{/Symbol D}P({/Symbol D}r)' pt 7, f(x) t 'y={/Symbol g}+nlog({/Symbol D}r)'