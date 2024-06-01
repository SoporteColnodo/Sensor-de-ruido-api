//const char* ssid = "Colnodo Invitados";     // Nombre de la red WiFi
//const char* password = "Cm*Go/cwN";    // Contraseña de la red WiFi
const char* hostname = "ESP32DB_002"; // nombre del dispositivo

extern String User = "esp32ruidocolnodo_002@colnodo.apc.org";//nombre del dispositivo en AA
extern String Password = "V4X2PbA3xG7.";//contraseña del dispositivo en AA
extern String idDispositivo = "dd20844abe8020aec75fc1dcdf26c09a";//id del dispositivo

extern const long minutos = 2;  // tiempo en milisegundos
extern const long intervalSD = minutos * 60000;// 2 minutos tiempo en milisegundos - 120000 tiempo 2 minutos ala SD
extern const long hora = 3600000; // tiempo en milisegundos - 3600000 tiempo hora a la api 

extern unsigned long previousMillisSD = 0; //tiempo inicial de envio a SD
extern int maxAttempts = 15; //intentos antes de finalizar proceso
extern int currentAttempts = 0;
extern uint32_t maxciclos = 0;
//extern int tmg = 100; //valores delay inicio


IPAddress ip(192, 168, 43, 105); //IP del dispositivo
IPAddress gateway(192, 168, 43, 1); //IP-gateway del dispositivo
IPAddress subnet(255, 255, 255, 0); //IP-subnet del dispositivo