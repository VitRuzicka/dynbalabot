#!/usr/bin/python3

from ws4py.client.threadedclient import WebSocketClient
import time, requests, json, os
#from pyplot import live_plotter
import numpy as np
from pyqtgraph.Qt import QtGui, QtCore
import numpy as np
import pyqtgraph as pg
from pyqtgraph.ptime import time
import sys
#vytvořit popup menu se jmény hostů a jejich adresami ->  výběr hosta, připojení k jeho adrese
host = "dynbalabot.local" #example
websocketHost = "ws://" + host + "/ws"
#host = "ws://192.168.0.160/ws"
app = QtGui.QApplication([])
pripojeno = False
dt = 10  # obnovovaci frekvence grafu (aktualńe 200hz)
size = 700
buffersize = 2*size
hodnotaPrijem1 = 0
hodnotaPrijem2 = 0
hodnotaPrijem3 = 0



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
        if len(zprava) == 3: #prijem tri hodnot
            print("error {0}, uhel {1}°, vstup {2}".format( zprava['err'], zprava['uhel'], zprava['out']))
            global hodnotaPrijem1
            global hodnotaPrijem2
            global hodnotaPrijem3
            hodnotaPrijem1 = zprava['err']
            hodnotaPrijem2 = zprava['uhel']
            hodnotaPrijem3 = zprava['out']
            #vykresli_graf(zprava['err'], zprava['uhel'])

def tlacitko():
    print("tlacitko spusteno")
def close_app():
    sys.exit()

if not QtGui.QApplication.instance():
    app = QtGui.QApplication([])
else:
    app = QtGui.QApplication.instance()

win = QtGui.QWidget()
win.setWindowTitle("Graf")
win.resize(1000, 600)
layout = QtGui.QGridLayout()
win.setLayout(layout)

b1 = QtGui.QPushButton("Pripojit")
b1.clicked.connect(tlacitko)

#b2 = QtGui.QPushButton("Close port")
#b2.clicked.connect(close_port)

#t1 = QtGui.QLineEdit("Enter Device Serial")

p1 = pg.PlotWidget()
p1.setRange(yRange=[-15000, 15000]) #max hodnoty vstupnich hodnot
p1.addLegend()
p1.showGrid(x=True, y=True, alpha=0.8)
p1.setLabel('left', 'vychylka')

curve1 = p1.plot(pen='y', name="Error") #nutno pojmenovat
curve2 = p1.plot(pen='g', name="Uhel") #nutno pojmenovat
curve3 = p1.plot(pen='r', name="Vystup") #pozdeji pouzit pro vystupni hodnotu

layout.addWidget(p1, 0, 0, 1, 3)
layout.addWidget(b1, 1, 0)
#layout.addWidget(t1, 1, 1)
#layout.addWidget(b2, 1, 2)


buffer1 = np.zeros(buffersize+1, int)
buffer2 = np.zeros(buffersize+1, int)
buffer3 = np.zeros(buffersize+1, int)

x = 0


def update():
    global curve1, curve2, x, ser, size, buffersize
    i = buffer1[buffersize]
    buffer1[i] = buffer1[i+size] = int(float(hodnotaPrijem1)*100)
    buffer1[buffersize] = i = (i+1) % size

    j = buffer2[buffersize]
    buffer2[j] = buffer2[j+size] = int(float(hodnotaPrijem2)*100)
    buffer2[buffersize] = j = (j+1) % size

    k = buffer3[buffersize]
    buffer3[k] = buffer3[k+size] = int(hodnotaPrijem3) *10
    buffer3[buffersize] = k = (k+1) % size

    curve1.setData(buffer1[i:i+size])
    curve1.setPos(x, 0)
    curve2.setData(buffer2[j:j+size])
    curve2.setPos(x, 0)
    curve3.setData(buffer3[k:k+size])
    curve3.setPos(x, 0)
    app.processEvents()

timer = QtCore.QTimer()
timer.timeout.connect(update)
timer.start(dt)
timer.setInterval(dt)

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
                close_app()
        except KeyboardInterrupt:
            ws.close()
            close_app()




# if(ser != None):
#  timer.stop()
prijem_dat()
win.show()
app.exec_()