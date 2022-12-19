import matplotlib.pyplot as plt
import numpy as np
from math import log10, floor, sqrt
import scipy as sp
from scipy import stats
import pandas as pd

#########################################################################
# funzione per scrivere e arrotondare con un certo numero di cifre significative
#########################################################################
def PrintResult(name,mean,sigma,digits,unit):
    mean = round(mean,digits)
    sigma = round(sigma,digits)
    nu = sigma / mean
    result = (name+" = ({0}\pm{1} ) ".format(mean,sigma)+unit+" [{0:.2f}%]".format(nu*100))
    print (result)
    #return ""
    

#########################################################################
# funzioni specifiche per PP5-Volano
#########################################################################
def import_dataV(file_name=""):
    df = pd.read_csv(file_name,header=1,sep='\t')
    switchComaDot(file_name)
    df = df.rename(columns={"Time (s)":"t","Angle (rad)":"a", "Position (m)": "h"})
    np_h = df.h.to_numpy()
    np_t = df.t.to_numpy()
    np_a = df.a.to_numpy()
    #media   
    #mean = np_a.mean()
    #sdt deviation capionaria
    #np_std=np.std(np_a, ddof=1)
    
    return np_t, np_h, np_a, df




def velocita1(X,sigmaX):
    z=len(X)
    v=np.zeros(int(z/2))
    uv=np.zeros(int(z/2))
    
    for(ix,x) in enumerate(v):
        v[ix:] = (X[2*ix+1]-X[2*ix])/0.05
        uv[ix:] = (2**(0.5))*sigmaX/0.05
        
    return v, uv


#########################################################################
# altre funzioni
#########################################################################
#in un file di testo sostituisce il punto con la virgola
def switchComaDot(name="nome_file"):
    reading_file = open(name, "r")

    new_file_content = ""
    for line in reading_file:
        stripped_line = line.strip()
        new_line = stripped_line.replace(",", ".")
        new_file_content += new_line +"\n"
    reading_file.close()

    writing_file = open(name, "w")
    writing_file.write(new_file_content)
    writing_file.close()


#test del Chi^2 sul fit lineare
def Chi2(x, y, u_y, m, c, alpha=0.05):
  residui_norm=(y-m*x-c)/u_y
  theta=np.sum(residui_norm**2)
  ndf=len(x)-2
  print("Chi^2mis = {0}\nndf = {1}".format(theta.round(4), ndf))

  p_value = 1 - stats.chi2.cdf(theta, ndf)  
  if p_value>alpha:
    print ("p_value={0} > {1}".format(p_value.round(2),alpha))
    print ("Test del Chi2 al livello di significatività alpha={0} superato".format(alpha))
  else:
    print ("p_value={0} < {1}".format(p_value.round(2),alpha))
    print ("Test del Chi2 al livello di significatività alpha={0} non superato".format(alpha))
  return p_value

  #Chi2 + media pesata
def Chi2_mean(y, uy, alpha=0.05):
    mean = w_mean(y, uy)
    u_mean = u_pesata(uy)
    d = (y - mean)/uy
    theta = np.sum(d**2)
    ndf = len(y)-1
    print("Chi^2mis = {0}\nndf = {1}".format(theta.round(4), ndf))

    p_value = 1 - stats.chi2.cdf(theta, ndf)  
    if p_value>alpha:
        print ("p_value={0} > {1}".format(p_value.round(2),alpha))
        print ("Test del Chi2 al livello di significatività alpha={0} superato".format(alpha))
    else:
        print ("p_value={0} < {1}".format(p_value.round(2),alpha))
        print ("Test del Chi2 al livello di significatività alpha={0} non superato".format(alpha))
    return mean, u_mean, p_value

#tagliare dei dati da un data frame
def dfCut(v, uv, t, a, b, out=True):
    df = np.column_stack((v ,uv, t))
    df = pd.DataFrame(df, columns = ['v', 'uv', 't'])
    
    if(out):
     df = df.drop(df[df.t < a].index | df[df.t > b].index)
    
    else:
        df_aux=df
        df = df.drop(df[df.t > a].index)  
        df_aux = df_aux.drop(df_aux[df_aux.t < b].index)
        df = pd.merge(df, df_aux, how='outer')
    
    new_v = df.v.to_numpy()
    new_uv = df.uv.to_numpy()
    new_t = df.t.to_numpy()
    
    return new_v, new_uv, new_t
    
#altri tagli
def cut(x, y, xmin, xmax):
  df = np.column_stack((x ,y))
  df = pd.DataFrame(df, columns = ['x', 'y'])    
  df = df.drop(df[df.x < xmin].index | df[df.x > xmax].index)
  new_x = df.x.to_numpy()
  new_y = df.y.to_numpy()
  return new_x, new_y       

#fa un plot
def generic_plot(x, y, xlabel='x[ux]', ylabel='y[u]', legenda='y(t)', title="Grafico di y(x)", shape="o"):
    plt.xlabel(xlabel)
    plt.ylabel(ylabel)  
    plt.plot(x, y, shape, label=legenda)
    plt.title(title)
    plt.legend()

#zoom su un grafico
def plt_zoom(x, y, xmin, xmax, xlabel='x[ux]', ylabel='y[u]', legenda='y(t)', title="Grafico di y(x)", shape="o"):
  df = np.column_stack((x ,y))
  df = pd.DataFrame(df, columns = ['x', 'y'])    
  df = df.drop(df[df.x < xmin].index | df[df.x > xmax].index)
  new_x = df.x.to_numpy()
  new_y = df.y.to_numpy()  

  plt.xlabel(xlabel)
  plt.ylabel(ylabel) 
  plt.plot(new_x, new_y, shape,label=legenda)
  plt.title(title)
  plt.legend()
    
#fare un istogramma
def isto(array,coeff=0.5,xlabel='x[u]', x='x', multinomiale=False):
    binsize=np.std(array,ddof=1)*coeff
    interval=np.max(array)-np.min(array)
    nbin=int(interval/binsize)
    nbin
    v_att, lim, ciao = plt.hist(array,bins=nbin,color='blue', alpha=0.75)
    dev_std = np.sqrt(v_att)
    plt.xlabel(xlabel)
    plt.ylabel('Contggi')
    plt.title(label='Istogramma di {0}: $\mu$={1:.3f} S={2:.3f}'.format(x,np.mean(array),np.std(array,ddof=1)))
    if(multinomiale):
        a=np.zeros(len(lim)-1)
        for (ix,x) in enumerate(lim):
           if(ix!=len(lim)-1):
               a[ix] = (lim[ix+1]+lim[ix])/2
        plt.errorbar(a, v_att, dev_std, marker='.',linestyle="", color="black")
    return a, v_att, dev_std

#fare una tabella (inserire prima l'array più corto)
def tab(n_col, a=np.zeros(10), b=np.zeros(10), c=np.zeros(10), d=np.zeros(10), e=np.zeros(10)): 
    if(n_col==2):   
        for ix,x in enumerate(a):
            print("{0} & {1} \\\ \n".format(a[ix], b[ix]))
    elif(n_col==3):
        for ix,x in enumerate(a):
            print("{0} & {1} & {2} \\\ \n".format(a[ix], b[ix], c[ix]))
    elif(n_col==4):   
        for ix,x in enumerate(a):
            print("{0} & {1} & {2} & {3} \\\ \n".format(a[ix], b[ix], c[ix], d[ix]))
    elif(n_col==5):
        for ix,x in enumerate(a):
            print("{0} & {1} & {2} & {3} & {4}\\\ \n".format(a[ix], b[ix], c[ix], d[ix], e[ix])) 

#somma in quadratura tra le incertezze  HO MESSO N=10
def s_q(array, sigma_b):
    print("somma in quadratura delle incertezze: {0}".format(np.sqrt((np.std(array, ddof=1)/np.sqrt(5))**2+sigma_b**2)))
    return np.sqrt((np.std(array, ddof=1)/np.sqrt(10))**2+sigma_b**2)
            
#confronto tra due misure (Z)
def confronto(x, y, ux, uy):
    return (x-y)/np.sqrt(ux**2+uy**2)

#incertezza sulla media pesata, ux è l'array incertezze
def u_pesata(ux):
    print("incertezza sulla media pesata: {0}".format(1/np.sqrt(my_denominatore(ux))))
    return 1/np.sqrt(my_denominatore(ux))

#media pesata con l'inverso delle incertezze al quadrato
def w_mean(x, ux):
    #print("media pesata: {0}".format(my_mean(x, my_weight(ux))))
    return my_mean(x, my_weight(ux))     

#########################################################################
# funzioni specifiche per PP3-Carrello, lab. mecc.
#########################################################################
def y_estrapolato2(x, m, c, sigma_x, sigma_m, sigma_c, cov_mc):
    y = m*np.asarray(x) + c
    uy = np.sqrt(np.power(x, 2)*np.power(sigma_m, 2) + np.power(sigma_x, 2)*np.power(m, 2)+ np.power(sigma_c, 2) + 2*x*cov_mc ) 
    return y, uy


def velocita(X, m, sigma_m, sigma_DT, DT):
    z=len(X)
    v=np.zeros(int(z/2))
    uv=np.zeros(int(z/2))
    
    for(ix,x) in enumerate(v):
        v[ix:] = (X[2*ix+1]-X[2*ix])/0.05
        uv[ix] = np.sqrt(2*m*m*np.power(sigma_DT[2*ix],2) + np.power((DT[2*ix+1]-DT[2*ix]),2)*sigma_m*sigma_m)/0.05
        
    return v, uv


def t_plot(x, ylabel='y[u]', legenda='y(t)'):
    t=np.linspace(0,0.05*len(x),len(x))
    plt.xlabel('t[s]')
    plt.ylabel(ylabel)  
    plt.plot(t,x, 'o', label=legenda)
    plt.legend()    
    
def t_plot2(x, ylabel='y[u]', legenda='y(t)'):
    t=np.linspace(0,0.1*len(x),len(x))
    plt.xlabel('t[s]')
    plt.ylabel(ylabel)  
    plt.plot(t,x, '.', label=legenda)
    plt.legend()  
    
def import_data(file_name=""):
    df = pd.read_csv(file_name,header=1,sep='\t')
    switchComaDot(file_name)
    df = df.rename(columns={"Tempo ( s )":"t","Tempo trascorso ( s )":"DeltaT"})
    np_a = df.DeltaT.to_numpy()
    #media   
  
    mean = np_a.mean()
    #incertezza
    u_np_a=np.sqrt( (np.std(np_a, ddof=1)/np.sqrt(len(np_a)))**2+((0.000001/np.sqrt(12))**2))
    
    return np_a, mean, u_np_a
    
def import_data2(file_name=""):
    df = pd.read_csv(file_name,header=1,sep='\t')
    switchComaDot(file_name)
    df = df.rename(columns={"Tempo ( s )":"t","Tempo trascorso ( s )":"DeltaT"})
    np_a = df.t.to_numpy()
    np_b = df.DeltaT.to_numpy()
    
    return np_a, np_b

def import_data3(file_name=""):
    df = pd.read_csv(file_name,header=1,sep='\t')
    switchComaDot(file_name)
    df = df.rename(columns={"Tempo ( s )":"t","Tempo trascorso ( s )":"DeltaT"})
    np_a = df.index.to_numpy()
    np_b = df.DeltaT.to_numpy()
    
    return np_a, np_b


def modifica_tempi(t):
    z=len(t)
    z=(int(z/2))
    t_nuovo=np.zeros(z)
    for(ix,x) in enumerate(t_nuovo):
        t_nuovo[ix:]=(t[2*ix+1]+t[2*ix])/2      
    return t_nuovo    
   

#########################################################################
# funzioni per fare il fit lineare
#########################################################################

#fa la tabella
def saint_tab(x, y, uy):
    print("$\overline{{x}}$ & {0:.1f} & UNITÀ \\\ \n\hline \n".format(w_mean(x, uy)))
    print("$\overline{{y}}$ & {0:.1f} & UNITÀ \\\ \n\hline \n".format(w_mean(y, uy)))
    print("$\overline{{x^2}}$ & {0:.1f} & UNITÀ \\\ \n\hline \n".format(w_mean(x**2, uy)))
    print("$\overline{{xy}}$ & {0:.1f} & UNITÀ \\\ \n\hline \n".format(w_mean(x*y, uy)))
    print("$Var[x]$ & {0:.1f} & UNITÀ \\\ \n\hline \n".format(my_var(x, my_weight(uy))))
    print("$Cov[x,y]$ & {0:.1f} & UNITÀ \\\ \n\hline \n".format(my_cov(x, y, my_weight(uy))))
    print("$\sum_i \sigma_{{y_i}}^{{-2}}$ & {0:.1f} & UNITÀ \\\ \n\hline \n".format(my_denominatore(uy)))

# prende le incertezze e fa un nuovo array con l'inverso di quelle incertezze al quadrato (i pesi)
def my_weight(s):
    return np.power(s.astype(float), -2)

# fa la somma dell'array di sopra
def my_denominatore(s):
    #print("somma dell'inverso delle incertezze al quadrato: {0}".format(np.sum(my_weight(s))))
    return np.sum(my_weight(s))

# fa la media pesata (x array valori, w array dei pesi). Meglio usare la funzione -->
 #-->"w_mean" perché prende in ingresso direttamente l'array delle incertezze
def my_mean(x, w):
    return np.sum( x*w ) / np.sum( w )

def my_cov(x, y, w):
    return my_mean(x*y, w) - my_mean(x, w)*my_mean(y, w)

def my_var(x, w):
    return my_cov(x, x, w)

def my_line(x, m=1, c=0):
    return m*x + c

def y_estrapolato(x, m, c, sigma_m, sigma_c, cov_mc):
    y = m*x + c
    uy = np.sqrt(np.power(x, 2)*np.power(sigma_m, 2) +
                   np.power(sigma_c, 2) + 2*x*cov_mc ) 
    return y, uy

def lin_fit(x, y, sd_y, xlabel="x [ux]", ylabel="y [uy]", ylabel1="y", uy_label="uy" , xm=0., xM=1., ym=0., yM=1., alpha=0.05,
            verbose=True, plot=False, setrange=False, residui=False, chi2=False):

    #pesi
    w_y = np.power(sd_y.astype(float), -2) 
    
    #m
    m = my_cov(x, y, w_y) / my_var(x, w_y)
    var_m = 1 / ( my_var(x, w_y) * np.sum(w_y) )
    
    #c
    c = my_mean(y, w_y) - my_mean(x, w_y) * m
    var_c = my_mean(x*x, w_y)  / ( my_var(x, w_y) * np.sum(w_y) )
    
    #cov
    cov_mc = - my_mean(x, w_y) / ( my_var(x, w_y) * np.sum(w_y) ) 
   
    #rho
    rho_mc = cov_mc / ( sqrt(var_m) * sqrt(var_c) )

    if (verbose):
        
        print ('m         = ', m.round(4))
        print ('sigma(m)  = ', np.sqrt(var_m).round(4))
        print ('c         = ', c.round(4))
        print ('sigma(c)  = ', np.sqrt(var_c).round(4))
        print ('cov(m, c) = ', cov_mc.round(4))
        print ('rho(m, c) = ', rho_mc.round(4))
        
    if (plot):
        
        # rappresento i dati
        plt.errorbar(x, y, yerr=sd_y, xerr=0, ls='', marker='.', 
                     color="black",label='dati')

        # costruisco dei punti x su cui valutare la retta del fit              
        xmin = float(np.min(x)) 
        xmax = float(np.max(x))
        xmin_plot = xmin-.2*(xmax-xmin)
        xmax_plot = xmax+.2*(xmax-xmin)
        if (setrange):
            xmin_plot = xm
            xmax_plot = xM  
        x1 = np.linspace(xmin_plot, xmax_plot, 100)
        y1 = my_line(x1, m, c)
        
        # rappresento la retta del fit
        plt.plot(x1, y1, linestyle='-.', color="green", label='fit')
        plt.xlabel(xlabel)
        plt.ylabel(ylabel)
        plt.title('Fit lineare')
        plt.xlim(xmin_plot,xmax_plot)
        if (setrange):
            plt.ylim(ym,yM)
        
        # rappresento le incertezze sulla retta 
        y1_plus_1sigma = y1+y_estrapolato(x1, m, c, np.sqrt(var_m), np.sqrt(var_c), cov_mc)[1]
        y1_minus_1sigma = y1-y_estrapolato(x1, m, c, np.sqrt(var_m), np.sqrt(var_c), cov_mc)[1]         
        plt.plot(x1,y1_plus_1sigma, linestyle='-', color="orange", label=r'fit $\pm 1\sigma$')
        plt.plot(x1,y1_minus_1sigma, linestyle='-', color="orange")
        
        plt.grid()
        
        plt.legend()
        
        plt.show()
        
        if (residui):
            residui_norm(m, c, x, y, sd_y, ylabel1, uy_label, xlabel)

        p_value=0
        if (chi2):
            p_value = Chi2(x, y, sd_y, m, c, alpha)
             
        
    return m, np.sqrt(var_m), c, np.sqrt(var_c), cov_mc, rho_mc, p_value

def residui(m, c, x, y, uy, y_label="y", uy_label="uy", x_label="x[ux]"):
    y_atteso = m*x + c
    d = y - y_atteso

    plt.errorbar(x,d,uy,marker='.',linestyle="")
    plt.ylabel("Residui $d={0}-{0}_{{atteso}}$ [{1}]".format(y_label, uy_label))
    plt.xlabel("${0}$".format(x_label))
    plt.grid()
    plt.show()

def residui_norm(m, c, x, y, uy, y_label="y", uy_label="uy", x_label="x[ux]"):
    y_atteso = m*x + c
    d = y - y_atteso
    d_norm = d / uy

    plt.errorbar(x,d_norm,uy/uy,marker='.',linestyle="")
    plt.ylabel("Residui normaizzati $d={0}-{0}_{{atteso}}$/$\sigma_{0}$ ".format(y_label))
    plt.xlabel("${0}$".format(x_label))
    plt.grid()
    plt.show()    
    
    
def prova():
    print("Prova riuscita!")




#oltre a migliorare alcune funzioni come quella della tabella potrebbe essere utile 
#scriverne una che scritta la formula in "python" la converte in "latex"