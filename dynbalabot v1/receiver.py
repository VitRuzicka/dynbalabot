#!/usr/bin/python3

import threading
from ws4py.client.threadedclient import WebSocketClient
import time, requests, json, os
#from pyplot import live_plotter
import numpy as np
from pyqtgraph.Qt import QtGui, QtCore
import numpy as np
import pyqtgraph as pg
from pyqtgraph.ptime import time
#vytvořit popup menu se jmény hostů a jejich adresami ->  výběr hosta, připojení k jeho adrese
host = "dynbalabot.local" #example
websocketHost = "ws://" + host + "/ws"
#host = "ws://192.168.0.160/ws"
app = QtGui.QApplication([])
hodnotaPrijem1 = 0
hodnotaPrijem2 = 0
pripojeno = False


p = pg.plot()
p.setWindowTitle('dynbalabot')
curve = p.plot()
curve2 = p.plot()
data =[0] 
data2 = [0]


class Klient(WebSocketClient):
    def opened(self):
        print("Spojeni navazano")
        global pripojeno
        pripojeno = True
    def closed(self, code, reason=None):
        print("Spojeni ukonceno ", code, reason)
    def received_message(self, obsah):
        #print(obsah)
        zprava = json.loads(obsah.data.decode("utf-8"))
        if len(zprava) == 2:
            print(zprava['err'] + " " + zprava['uhel'])
            global hodnotaPrijem1
            global hodnotaPrijem2
            hodnotaPrijem1 = zprava['err']
            hodnotaPrijem2 = zprava['uhel']
            #vykresli_graf(zprava['err'], zprava['uhel'])
            
def update():
    global curve, curve2, data
    data.append(float(hodnotaPrijem1))
    xdata = np.array(data, dtype='float64')
    curve = plotItem.plot(xdata)               #co je plot item?
    data = np.roll(xdata, 1)  # scroll data
    curve.setData(xdata)

    data2.append(float(hodnotaPrijem2))
    xdata = np.array(data2, dtype='float64')
    curve2.setData(xdata)
    app.processEvents()

timer = QtCore.QTimer()
timer.timeout.connect(update)
timer.start(0)
       

def prijem_dat():
    global pripojeno
    if not pripojeno:
        try:
            response = os.system("ping -c 1 -w 150 " + host) #pokud se lze pingnout na zarizeni pokraacuj dal 
            if response == 0:
                #lze se pripojit
                ws = Klient(websocketHost) #pripoj se pres websockety
                ws.connect()
                print("Pripojeno k WS, cekam na data")

            else:
                print("dynbalabot neni pripojen k internetu")
                exit()
        except KeyboardInterrupt:
            ws.close()
            exit()




if __name__ == '__main__':       
    try:
        prijem_dat()
        #t1 = threading.Thread(target=prijem_dat, args=())
        #t2 = threading.Thread(target=vykresli_graf, args=(hodnotaPrijem1, hodnotaPrijem2, ))
        # creating two threads here t1 & t2
        #t1.start()
        #t2.start()
        import sys
        if (sys.flags.interactive != 1) or not hasattr(QtCore, 'PYQT_VERSION'):
            QtGui.QApplication.instance().exec_()
    except:
        print ("nelze nastartovat obe vlakna")
    while 1:
        #vykresli_graf(hodnotaPrijem1, hodnotaPrijem2)
        pass

