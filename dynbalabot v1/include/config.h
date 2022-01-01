//hlavni konfiguracni soubor pro dynbalabota

/////zde se aktivují příslušné funkční celky
#define OTAupload  //musí být aktivována wifi při použití OTA
#define WIFI
//#define DIAG
#define WS
//#define CAN
//#define CANDEBUG //slouzi k aktivaci PINGu


#define IOPIN1 27
#define IOPIN2 14
#define IOPIN3 12 //pouzito pro adresovane led diody
#define VCCPIN 39
#define INTPIN 23
//PID:
#define runTime  0.01   //puvodne 5ms = 200hz PIDloop; IMU bezi na 120Hz
#define maxHodnota 500 // +- hodnota, která se saturuje po výstupu z PID -- upravit podle hoverboard vstupu
#define DEADBAND 2   // určuje jak tenká je hranice (bod kdy systém zůstane v klidu) =====  Hystereze
#define deadMot 90  //kompenzace deadbandu motorů do jake miry motor nic nedela od stredu na obe strany - muze se menit v zavislovsti na napajecim napeti