/*
  State change detection (edge detection)  -  http://www.arduino.cc/en/Tutorial/ButtonStateChange
  monostabil reglabil - adaptare scrisa de Nicu FLORICA (niq_ro) pt Tudor Gheorghe
  http://www.arduinotehniq.com
  ver.1 - logica negativa sau normala pt releu
*/

#define buton 2          // pinul unde este conectat contactul
#define releu 13         // pinul de comanda pentru releu
#define potentiometru A0 // pinul unde se conecteaza cursorul potentiometrului    

// logica inversa
#define aprins LOW
#define stins HIGH

/*
// logica bormala
#define aprins HIGH
#define stins LOW
*/

int actual = 0;          // starea curenta a butonulu
int anterior = 0;        // starea anterioara a butonului
int vcursor = 0;         // citire valoare tensiune pe cursor (0..1023);
unsigned long tpapasare = 0;   // memorare timp apasare buton (
unsigned long tpmentinere = 0; // timp mentinere releu atras (in secunde)
int tpmin = 1000;        // timp minim actionare releu (in milisecunde)
int tpmax = 10000;       // timp maxim actionare releu (in milisecunde)
byte actionat = 0;         // variabila pentru a sti ca s-a actionat releul

void setup() 
{
  Serial.begin(9600);     // initializare comunicare seriala
  pinMode(buton, INPUT);          // pinul este definit ca intrare
  pinMode(releu, OUTPUT);         // pinul este definit ca iesire
  pinMode(potentiometru, INPUT);  // pinul este de finit ca intrare
  digitalWrite(buton, HIGH);      // pinul este in stare HIGH (pull-up intern)
  digitalWrite(releu, stins);       // pinul este in stare LOW
  tpapasare = 0;
}


void loop() {
  actual = digitalRead(buton);  // se citeste starea butonului

  vcursor = analogRead(potentiometru); // citire tensiune pe cursor
  tpmentinere = map(vcursor, 0, 1023, tpmin, tpmax); // corelare timpi minim spre maxim

  if (actual != anterior) // daca starea actuala butonului  este diferita de cea anterioara
  {
    if (actual == LOW) // butonul este apasat
    {
      actionat = 0;   // releul nu a fost actionat
      Serial.println("butonul este apasat");
      tpapasare = millis();  // se memoreaza timpul cand s-a apasat butonul
    } 
    else 
    {
      Serial.println("butonul este neapasat");
    }
    delay(50);  // mica pauza
  }
  anterior = actual;   // se memoreaza starea 

  if ((millis() - tpapasare <= tpmentinere) and (actionat == 0))  // daca nu s-a actionat releul si trebuie sa fie actionat
  {
    digitalWrite(releu, aprins);
    Serial.println("releu comandat");
    actionat = 1;
    
  }  
  if ((millis() - tpapasare > tpmentinere) and (actionat == 1))
  {
    digitalWrite(releu, stins);
    Serial.println("releu liber");
    actionat = 0;    // releu liber
  }

}  // sfarsit de program
