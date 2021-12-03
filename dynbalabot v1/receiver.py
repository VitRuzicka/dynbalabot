from ws4py.client.threadedclient import WebSocketClient
import time, requests, json, os
from pyplot import live_plotter
import numpy as np
#vytvořit popup menu se jmény hostů a jejich adresami ->  výběr hosta, připojení k jeho adrese
host = "dynbalabot.local" #example
websocketHost = "ws://" + host + "/ws"
#host = "ws://192.168.0.160/ws"

class Klient(WebSocketClient):
    def opened(self):
        print("Spojeni navazano")
    def closed(self, code, reason=None):
        print("Spojeni ukonceno ", code, reason)
    def received_message(self, obsah):
        #print(obsah)
        zprava = json.loads(obsah.data.decode("utf-8"))
        if len(zprava) == 4:
            print(zprava['nap'])        

sirka = 150 #urcuje kolik dat ma byt najednou zobrazeno
x_vec = np.linspace(0,1,sirka+1)[0:-1] #funkce která generuje cisla pro osu X 
y_vec = np.zeros(sirka)#np.random.randn(len(x_vec)) #vyplneni osy Y random hodnotami (pocet odpovida sirce) ,,, numpy.zeros vyplni jen nulami
line1 = []           
print(len(x_vec))
if __name__ == '__main__':

    while True:
        rand_val = 1#np.random.randn(1) #generuj jednu random hodnotu kolem nuly ale jako pole
        y_vec[-1] = rand_val  #zapsani do posledni hodnoty (-1) znamena posledni
        line1 = live_plotter(x_vec,y_vec,line1) #vykresli
        y_vec = np.append(y_vec[1:],0.0) #spojeni existujiciho grafu s novymi hodnotami


    """
    try:
        response = os.system("ping -c 1 -w 150 " + host) #pokud se lze pingnout na zarizeni pokraacuj dal 
        if response == 0:
            #lze se pripojit
            ws = Klient(websocketHost) #pripoj se pres websockety
            ws.connect()
            print("Pripojeno k WS, cekam na data")
            while 1:
                time.sleep(0.01)     
        else:
            print("dynbalabot neni pripojen k internetu")
            exit()
       
    except KeyboardInterrupt:
        ws.close()
        exit()
        """