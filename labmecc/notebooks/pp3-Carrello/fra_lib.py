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
# altre funzioni
#########################################################################


#in un file di testo sostituisce il punto con la virgola
def swichComaDot(name="nome_file"):
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

    
    
#tagliare dei dati da un data frame
def dfCut(df, udf, a, b):
    df = np.column_stack((df ,udf))
    df = pd.DataFrame(df, columns = ['v', 'uv'])
    df = df.drop(df[df.v < a].index | df[df.v > b].index)
    new_df = df.v.to_numpy()
    new_udf = df.uv.to_numpy()
    return new_df, new_udf

    
    
#fare un istogramma
def isto(array,coeff,xlabel='x[u]', x='x'):
    binsize=np.std(array,ddof=1)*coeff
    interval=np.max(array)-np.min(array)
    nbin=int(interval/binsize)
    nbin
    plt.hist(array,bins=nbin,color='blue', alpha=0.75)
    plt.xlabel(xlabel)
    plt.ylabel('Contggi')
    plt.title(label='Istogramma di {0}: $\mu$={1:.3f} S={2:.3f}'.format(x,np.mean(array),np.std(array,ddof=1)))


def y_estrapolato2(x, m, c, sigma_x, sigma_m, sigma_c, cov_mc):
    y = m*np.asarray(x) + c
    uy = np.sqrt(np.power(x, 2)*np.power(sigma_m, 2) + np.power(sigma_x, 2)*np.power(m, 2)+ np.power(sigma_c, 2) + 2*x*cov_mc ) 
    return y, uy


def velocità(X, m, sigma_m, sigma_DT, DT):
    z=len(X)
    v=np.zeros(int(z/2))
    uv=np.zeros(int(z/2))
    
    for(ix,x) in enumerate(v):
        v[ix:] = (X[2*ix+1]-X[2*ix])/0.05
        #sigma_2=np.power(sigma_DT[2*ix+1],2) + np.power(sigma_DT[2*ix],2)
        uv[ix] = np.sqrt(2*m*m*sigma_DT[2*ix]*sigma_DT[2*ix] + np.power((DT[2*ix+1]-DT[2*ix]),2)*sigma_m*sigma_m)/0.05
        
    return v, uv


def t_plot(x, ylabel='y[u]', legenda='y(t)'):
    t=np.linspace(0,0.05*len(x),len(x))
    plt.xlabel('t[s]')
    plt.ylabel(ylabel)  
    plt.plot(t,x, 'o', label=legenda)
    plt.legend()    
    
    
#########################################################################
# funzioni per fare il fit lineare
#########################################################################

# prende le incertezze e fa un nuovo array con l'inverso di quelle incertezze al quadrato (i pesi)
def my_weight(s):
    return np.power(s.astype(float), -2)

# fa la somma dell'array di sopra
def my_denominatore(s):
    return np.sum(my_weight(s))

# fa la media pesata (x array valori, w array dei pesi)
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

def lin_fit(x, y, sd_y, xlabel="x [ux]", ylabel="y [uy]", xm=0., xM=1., ym=0., yM=1., 
            verbose=True, plot=False, setrange=False):

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
        
    return m, np.sqrt(var_m), c, np.sqrt(var_c), cov_mc, rho_mc

def residui(m, c, x, y, uy, y_label="y", uy_label="uy", x_label="x", ux_label="uy"):
    y_atteso = m*x + c
    d = y - y_atteso

    plt.errorbar(x,d,uy,marker='.',linestyle="")
    plt.ylabel("Residui $d={0}-{0}_{{2}}$ [{1}]".format(y_label, uy_label, "atteso"))
    plt.xlabel("${0}$ [{1}]".format(x_label, ux_label))
    plt.grid()
    
def residui_norm(m, c, x, y, uy, y_label="y", uy_label="uy", x_label="x", ux_label="uy"):
    y_atteso = m*x + c
    d = y - y_atteso
    d_norm = d / uy

    plt.errorbar(x,d_norm,uy/uy,marker='.',linestyle="")
    plt.ylabel("Residui normaizzati $d={0}-{0}_{{2}}$ [{1}]".format(y_label, uy_label, "atteso"))
    plt.xlabel("${0}$ [{1}]".format(x_label, ux_label))
    plt.grid()    