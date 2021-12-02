from ws4py.client.threadedclient import WebSocketClient
import time, requests, json, os
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
        print(obsah)
        zprava = json.loads(obsah)
        try:
            print(zprava['akce'])
        except IndexError: #nutno doladit podle toho co to hodi
            print("hodnota nenalezena v poli")
            
            
if __name__ == '__main__':
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
