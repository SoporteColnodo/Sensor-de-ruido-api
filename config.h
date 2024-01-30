const char* ssid = "usuariodered";     // Nombre de la red WiFi
const char* password = "contrasena";    // Contraseña de la red WiFi
const char* hostname = "nombre_de_dispositivo"; // nombre del dispositivo

//extern String createcsv[] = {"general.csv", "newpromediooctubre.csv"};//archivos que se crean
extern String createcsv[] = {"generaldbcolnodo004.csv"};//archivos que se crean
extern String User = "nombre del dispositivo en AA";//nombre del dispositivo en AA
extern String Password = "contraseña del dispositivo en AA.";//contraseña del dispositivo en AA

//Si los dispositivos no se sincronizan o no responden este tiempo puede cambiar es decir el delay no es constante debido
// a que funciona a partir de la sincronización de dispositivos externos
extern unsigned long previousMillisAPI = 0; //tiempo inicial de envio del API
extern unsigned long previousMillisSD = 0; //tiempo inicial de envio a SD
extern const long intervalAPI = 600000; // tiempo de ciclo envio al API en milisegundos
extern const long intervalSD = 120000;  // tiempo de ciclo envio a SD en milisegundos

IPAddress ip(192, 168, 43, 12); //IP del dispositivo
IPAddress gateway(192, 168, 43, 1); //IP-gateway del dispositivo
IPAddress subnet(255, 255, 255, 0); //IP-subnet del dispositivo
