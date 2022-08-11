#define LED_BUILTIN 2   
#define USING_CORS_FEATURE true

#include <BleKeyboard.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>

const char *ssid = "WifiKeyboard";
const char *password = "mySecretPassword";

WiFiServer server(80);
BleKeyboard bleKeyboard;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  //  Serial.begin(115200);
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  server.begin();
  bleKeyboard.begin();
}

void loop() {
  WiFiClient client = server.available();   
  if (client) {                             
    digitalWrite(LED_BUILTIN, HIGH);
    String currentLine = "";                
    while (client.connected()) {            
      if (client.available()) {             
        char c = client.read();             
        Serial.write(c);                    
        if (c == '\n') {                    
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Access-Control-Allow-Origin: *");
            client.println("Connection: close");
            client.println();
            digitalWrite(LED_BUILTIN, LOW);
            break;
          } else {    
            currentLine = "";
          }
        } else if (c != '\r') {  
          currentLine += c;      
        }
        
        if (currentLine.endsWith("GET /ret/EF")) {
          bleKeyboard.write(KEY_RETURN);
        } else if (currentLine.endsWith("GET /bk/EF")) {
          bleKeyboard.write(KEY_BACKSPACE);
        }  else if (currentLine.endsWith("GET /up/EF")) {
          bleKeyboard.write(KEY_UP_ARROW);
        }  else if (currentLine.endsWith("GET /dn/EF")) {
          bleKeyboard.write(KEY_DOWN_ARROW);
        }  else if (currentLine.endsWith("GET /lf/EF")) {
          bleKeyboard.write(KEY_LEFT_ARROW);
        }  else if (currentLine.endsWith("GET /rt/EF")) {
          bleKeyboard.write(KEY_RIGHT_ARROW);
        }  else if (currentLine.endsWith("GET /de/EF")) {
          bleKeyboard.write(KEY_DELETE);
        } else if (currentLine.endsWith("GET /sp/EF")) {
          bleKeyboard.print(" ");
        } else if (currentLine.endsWith("GET /a/EF")) {
          bleKeyboard.print("a");
        } else if (currentLine.endsWith("GET /b/EF")) {
          bleKeyboard.print("b");
        } else if (currentLine.endsWith("GET /c/EF")) {
          bleKeyboard.print("c");
        } else if (currentLine.endsWith("GET /d/EF")) {
          bleKeyboard.print("d");
        } else if (currentLine.endsWith("GET /e/EF")) {
          bleKeyboard.print("e");
        } else if (currentLine.endsWith("GET /f/EF")) {
          bleKeyboard.print("f");
        } else if (currentLine.endsWith("GET /g/EF")) {
          bleKeyboard.print("g");
        } else if (currentLine.endsWith("GET /h/EF")) {
          bleKeyboard.print("h");
        } else if (currentLine.endsWith("GET /i/EF")) {
          bleKeyboard.print("i");
        } else if (currentLine.endsWith("GET /j/EF")) {
          bleKeyboard.print("j");
        } else if (currentLine.endsWith("GET /k/EF")) {
          bleKeyboard.print("k");
        } else if (currentLine.endsWith("GET /l/EF")) {
          bleKeyboard.print("l");
        } else if (currentLine.endsWith("GET /m/EF")) {
          bleKeyboard.print("m");
        } else if (currentLine.endsWith("GET /n/EF")) {
          bleKeyboard.print("n");
        } else if (currentLine.endsWith("GET /o/EF")) {
          bleKeyboard.print("o");
        } else if (currentLine.endsWith("GET /p/EF")) {
          bleKeyboard.print("p");
        } else if (currentLine.endsWith("GET /q/EF")) {
          bleKeyboard.print("q");
        } else if (currentLine.endsWith("GET /r/EF")) {
          bleKeyboard.print("r");
        } else if (currentLine.endsWith("GET /s/EF")) {
          bleKeyboard.print("s");
        } else if (currentLine.endsWith("GET /t/EF")) {
          bleKeyboard.print("t");
        } else if (currentLine.endsWith("GET /u/EF")) {
          bleKeyboard.print("u");
        } else if (currentLine.endsWith("GET /v/EF")) {
          bleKeyboard.print("v");
        } else if (currentLine.endsWith("GET /w/EF")) {
          bleKeyboard.print("w");
        } else if (currentLine.endsWith("GET /x/EF")) {
          bleKeyboard.print("x");
        } else if (currentLine.endsWith("GET /y/EF")) {
          bleKeyboard.print("y");
        } else if (currentLine.endsWith("GET /z/EF")) {
          bleKeyboard.print("z");
        } else if (currentLine.endsWith("GET /0/EF")) {
          bleKeyboard.print("0");
        } else if (currentLine.endsWith("GET /1/EF")) {
          bleKeyboard.print("1");
        } else if (currentLine.endsWith("GET /2/EF")) {
          bleKeyboard.print("2");
        } else if (currentLine.endsWith("GET /3/EF")) {
          bleKeyboard.print("3");
        } else if (currentLine.endsWith("GET /4/EF")) {
          bleKeyboard.print("4");
        } else if (currentLine.endsWith("GET /5/EF")) {
          bleKeyboard.print("5");
        } else if (currentLine.endsWith("GET /6/EF")) {
          bleKeyboard.print("6");
        } else if (currentLine.endsWith("GET /7/EF")) {
          bleKeyboard.print("7");
        } else if (currentLine.endsWith("GET /8/EF")) {
          bleKeyboard.print("8");
        } else if (currentLine.endsWith("GET /9/EF")) {
          bleKeyboard.print("9");
        } else if (currentLine.endsWith("GET /A/EF")) {
          bleKeyboard.print("A");
        } else if (currentLine.endsWith("GET /B/EF")) {
          bleKeyboard.print("B");
        } else if (currentLine.endsWith("GET /C/EF")) {
          bleKeyboard.print("C");
        } else if (currentLine.endsWith("GET /D/EF")) {
          bleKeyboard.print("D");
        } else if (currentLine.endsWith("GET /E/EF")) {
          bleKeyboard.print("E");
        } else if (currentLine.endsWith("GET /F/EF")) {
          bleKeyboard.print("F");
        } else if (currentLine.endsWith("GET /G/EF")) {
          bleKeyboard.print("G");
        } else if (currentLine.endsWith("GET /H/EF")) {
          bleKeyboard.print("H");
        } else if (currentLine.endsWith("GET /I/EF")) {
          bleKeyboard.print("I");
        } else if (currentLine.endsWith("GET /J/EF")) {
          bleKeyboard.print("J");
        } else if (currentLine.endsWith("GET /K/EF")) {
          bleKeyboard.print("K");
        } else if (currentLine.endsWith("GET /L/EF")) {
          bleKeyboard.print("L");
        } else if (currentLine.endsWith("GET /M/EF")) {
          bleKeyboard.print("M");
        } else if (currentLine.endsWith("GET /N/EF")) {
          bleKeyboard.print("N");
        } else if (currentLine.endsWith("GET /O/EF")) {
          bleKeyboard.print("O");
        } else if (currentLine.endsWith("GET /P/EF")) {
          bleKeyboard.print("P");
        } else if (currentLine.endsWith("GET /Q/EF")) {
          bleKeyboard.print("Q");
        } else if (currentLine.endsWith("GET /R/EF")) {
          bleKeyboard.print("R");
        } else if (currentLine.endsWith("GET /S/EF")) {
          bleKeyboard.print("S");
        } else if (currentLine.endsWith("GET /T/EF")) {
          bleKeyboard.print("T");
        } else if (currentLine.endsWith("GET /U/EF")) {
          bleKeyboard.print("U");
        } else if (currentLine.endsWith("GET /V/EF")) {
          bleKeyboard.print("V");
        } else if (currentLine.endsWith("GET /W/EF")) {
          bleKeyboard.print("W");
        } else if (currentLine.endsWith("GET /X/EF")) {
          bleKeyboard.print("X");
        } else if (currentLine.endsWith("GET /Y/EF")) {
          bleKeyboard.print("Y");
        } else if (currentLine.endsWith("GET /Z/EF")) {
          bleKeyboard.print("Z");
        } else if (currentLine.endsWith("GET /bt/EF")) {
          bleKeyboard.print("`");
        } else if (currentLine.endsWith("GET /td/EF")) {
          bleKeyboard.print("~");
        } else if (currentLine.endsWith("GET /em/EF")) {
          bleKeyboard.print("!");
        } else if (currentLine.endsWith("GET /at/EF")) {
          bleKeyboard.print("@");
        } else if (currentLine.endsWith("GET /ht/EF")) {
          bleKeyboard.print("#");
        } else if (currentLine.endsWith("GET /dr/EF")) {
          bleKeyboard.print("$");
        } else if (currentLine.endsWith("GET /pc/EF")) {
          bleKeyboard.print("%");
        } else if (currentLine.endsWith("GET /pr/EF")) {
          bleKeyboard.print("^");
        } else if (currentLine.endsWith("GET /ad/EF")) {
          bleKeyboard.print("&");
        } else if (currentLine.endsWith("GET /st/EF")) {
          bleKeyboard.print("*");
        } else if (currentLine.endsWith("GET /op/EF")) {
          bleKeyboard.print("(");
        } else if (currentLine.endsWith("GET /cp/EF")) {
          bleKeyboard.print(")");
        } else if (currentLine.endsWith("GET /ds/EF")) {
          bleKeyboard.print("-");
        } else if (currentLine.endsWith("GET /us/EF")) {
          bleKeyboard.print("_");
        } else if (currentLine.endsWith("GET /eq/EF")) {
          bleKeyboard.print("=");
        } else if (currentLine.endsWith("GET /pl/EF")) {
          bleKeyboard.print("+");
        } else if (currentLine.endsWith("GET /so/EF")) {
          bleKeyboard.print("[");
        } else if (currentLine.endsWith("GET /co/EF")) {
          bleKeyboard.print("{");
        } else if (currentLine.endsWith("GET /sc/EF")) {
          bleKeyboard.print("]");
        } else if (currentLine.endsWith("GET /cc/EF")) {
          bleKeyboard.print("}");
        } else if (currentLine.endsWith("GET /bs/EF")) {
          bleKeyboard.print("\\");
        } else if (currentLine.endsWith("GET /vb/EF")) {
          bleKeyboard.print("|");
        } else if (currentLine.endsWith("GET /sm/EF")) {
          bleKeyboard.print(";");
        } else if (currentLine.endsWith("GET /cn/EF")) {
          bleKeyboard.print(":");
        } else if (currentLine.endsWith("GET /sq/EF")) {
          bleKeyboard.print("'");
        } else if (currentLine.endsWith("GET /dq/EF")) {
          bleKeyboard.print("\"");
        } else if (currentLine.endsWith("GET /cm/EF")) {
          bleKeyboard.print(",");
        } else if (currentLine.endsWith("GET /lt/EF")) {
          bleKeyboard.print("<");
        } else if (currentLine.endsWith("GET /dt/EF")) {
          bleKeyboard.print(".");
        } else if (currentLine.endsWith("GET /gt/EF")) {
          bleKeyboard.print(">");
        } else if (currentLine.endsWith("GET /fs/EF")) {
          bleKeyboard.print("/");
        } else if (currentLine.endsWith("GET /qm/EF")) {
          bleKeyboard.print("?");
        } else if (currentLine.endsWith("GET /ta/EF")) {
          bleKeyboard.write(KEY_TAB);
        } else if (currentLine.endsWith("GET /es/EF")) {
          bleKeyboard.write(KEY_ESC);
        } else if (currentLine.endsWith("GET /ct/EF")) {
          bleKeyboard.press(KEY_LEFT_CTRL);
        } else if (currentLine.endsWith("GET /sf/EF")) {
          bleKeyboard.press(KEY_LEFT_SHIFT);
        } else if (currentLine.endsWith("GET /al/EF")) {
          bleKeyboard.press(KEY_LEFT_ALT);
        } else if (currentLine.endsWith("GET /ctr/EF")) {
          bleKeyboard.release(KEY_LEFT_CTRL);
        } else if (currentLine.endsWith("GET /sfr/EF")) {
          bleKeyboard.release(KEY_LEFT_SHIFT);
        } else if (currentLine.endsWith("GET /alr/EF")) {
          bleKeyboard.release(KEY_LEFT_ALT);
        }
      }
    }
    // close the connection:
    client.stop();
    //    Serial.println("Client Disconnected.");
  }
}
