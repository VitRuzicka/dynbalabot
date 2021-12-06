import matplotlib.pyplot as plt
import numpy as np

# lepsi pozadi  pro "osciloskop"
plt.style.use('ggplot')

def live_plotter(x_vec,y1_data,y2_data,line1, line2, pause_time=0.01):
    if line1==[]: #prvni beh programu
        plt.ion() #toto umozni plynule plottovani dat
        fig = plt.figure(figsize=(13,6))
        ax = fig.add_subplot(111)
        # promenna ktera obsahuje caru, kterou muzeme nasledne vratit na konci fce
        line1, = ax.plot(x_vec,y1_data,color="r",alpha=0.8)
        line2, = ax.plot(x_vec,y2_data,color="g",alpha=0.8)        
        #update plot label/title
        plt.ylabel('Vychylka')
        plt.title('Dynbalabot')
        plt.show()
    
    # after the figure, axis, and line are created, we only need to update the y-data
    line1.set_ydata(y1_data)
    line2.set_ydata(y2_data)
    # nastaveni meritka v pripade ze je hodnota z grafu prekroci
    if np.min(y1_data)<=line1.axes.get_ylim()[0] or np.max(y1_data)>=line1.axes.get_ylim()[1]:
        plt.ylim([np.min(y1_data)-np.std(y1_data),np.max(y1_data)+np.std(y1_data)])
    # zastaveni grafu aby se data mohla zpracovat
    plt.pause(pause_time)
    
    # navrat hodnotu cary aby se ji dalo zase volat
    return line1, line2
"""
# the function below is for updating both x and y values (great for updating dates on the x-axis)
def live_plotter_xy(x_vec,y1_data,line1,identifier='',pause_time=0.01):
    if line1==[]:
        plt.ion()
        fig = plt.figure(figsize=(13,6))
        ax = fig.add_subplot(111)
        line1, = ax.plot(x_vec,y1_data,'r-o',alpha=0.8)
        plt.ylabel('Y Label')
        plt.title('Title: {}'.format(identifier))
        plt.show()
        
    line1.set_data(x_vec,y1_data)
    plt.xlim(np.min(x_vec),np.max(x_vec))
    if np.min(y1_data)<=line1.axes.get_ylim()[0] or np.max(y1_data)>=line1.axes.get_ylim()[1]:
        plt.ylim([np.min(y1_data)-np.std(y1_data),np.max(y1_data)+np.std(y1_data)])

    plt.pause(pause_time)
    
    return line1
"""
