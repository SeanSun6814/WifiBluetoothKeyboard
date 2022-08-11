#define LED_BUILTIN 2   
#define USING_CORS_FEATURE true

#include <BleKeyboard.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>

const char* ssid = "WifiKeyboard";
const char* password = "mySecretPassword";

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
            Serial.write(c);
            if (currentLine.indexOf("GET /ret/ HTTP") != -1) {
              bleKeyboard.write(KEY_RETURN);
            } else if (currentLine.indexOf("GET /bk/ HTTP") != -1) {
              bleKeyboard.write(KEY_BACKSPACE);
            } else if (currentLine.indexOf("GET /up/ HTTP") != -1) {
              bleKeyboard.write(KEY_UP_ARROW);
            } else if (currentLine.indexOf("GET /dn/ HTTP") != -1) {
              bleKeyboard.write(KEY_DOWN_ARROW);
            } else if (currentLine.indexOf("GET /lf/ HTTP") != -1) {
              bleKeyboard.write(KEY_LEFT_ARROW);
            } else if (currentLine.indexOf("GET /rt/ HTTP") != -1) {
              bleKeyboard.write(KEY_RIGHT_ARROW);
            } else if (currentLine.indexOf("GET /de/ HTTP") != -1) {
              bleKeyboard.write(KEY_DELETE);
            } else if (currentLine.indexOf("GET /sp/ HTTP") != -1) {
              bleKeyboard.print(" ");
            } else if (currentLine.indexOf("GET /a/ HTTP") != -1) {
              bleKeyboard.print("a");
            } else if (currentLine.indexOf("GET /b/ HTTP") != -1) {
              bleKeyboard.print("b");
            } else if (currentLine.indexOf("GET /c/ HTTP") != -1) {
              bleKeyboard.print("c");
            } else if (currentLine.indexOf("GET /d/ HTTP") != -1) {
              bleKeyboard.print("d");
            } else if (currentLine.indexOf("GET /e/ HTTP") != -1) {
              bleKeyboard.print("e");
            } else if (currentLine.indexOf("GET /f/ HTTP") != -1) {
              bleKeyboard.print("f");
            } else if (currentLine.indexOf("GET /g/ HTTP") != -1) {
              bleKeyboard.print("g");
            } else if (currentLine.indexOf("GET /h/ HTTP") != -1) {
              bleKeyboard.print("h");
            } else if (currentLine.indexOf("GET /i/ HTTP") != -1) {
              bleKeyboard.print("i");
            } else if (currentLine.indexOf("GET /j/ HTTP") != -1) {
              bleKeyboard.print("j");
            } else if (currentLine.indexOf("GET /k/ HTTP") != -1) {
              bleKeyboard.print("k");
            } else if (currentLine.indexOf("GET /l/ HTTP") != -1) {
              bleKeyboard.print("l");
            } else if (currentLine.indexOf("GET /m/ HTTP") != -1) {
              bleKeyboard.print("m");
            } else if (currentLine.indexOf("GET /n/ HTTP") != -1) {
              bleKeyboard.print("n");
            } else if (currentLine.indexOf("GET /o/ HTTP") != -1) {
              bleKeyboard.print("o");
            } else if (currentLine.indexOf("GET /p/ HTTP") != -1) {
              bleKeyboard.print("p");
            } else if (currentLine.indexOf("GET /q/ HTTP") != -1) {
              bleKeyboard.print("q");
            } else if (currentLine.indexOf("GET /r/ HTTP") != -1) {
              bleKeyboard.print("r");
            } else if (currentLine.indexOf("GET /s/ HTTP") != -1) {
              bleKeyboard.print("s");
            } else if (currentLine.indexOf("GET /t/ HTTP") != -1) {
              bleKeyboard.print("t");
            } else if (currentLine.indexOf("GET /u/ HTTP") != -1) {
              bleKeyboard.print("u");
            } else if (currentLine.indexOf("GET /v/ HTTP") != -1) {
              bleKeyboard.print("v");
            } else if (currentLine.indexOf("GET /w/ HTTP") != -1) {
              bleKeyboard.print("w");
            } else if (currentLine.indexOf("GET /x/ HTTP") != -1) {
              bleKeyboard.print("x");
            } else if (currentLine.indexOf("GET /y/ HTTP") != -1) {
              bleKeyboard.print("y");
            } else if (currentLine.indexOf("GET /z/ HTTP") != -1) {
              bleKeyboard.print("z");
            } else if (currentLine.indexOf("GET /0/ HTTP") != -1) {
              bleKeyboard.print("0");
            } else if (currentLine.indexOf("GET /1/ HTTP") != -1) {
              bleKeyboard.print("1");
            } else if (currentLine.indexOf("GET /2/ HTTP") != -1) {
              bleKeyboard.print("2");
            } else if (currentLine.indexOf("GET /3/ HTTP") != -1) {
              bleKeyboard.print("3");
            } else if (currentLine.indexOf("GET /4/ HTTP") != -1) {
              bleKeyboard.print("4");
            } else if (currentLine.indexOf("GET /5/ HTTP") != -1) {
              bleKeyboard.print("5");
            } else if (currentLine.indexOf("GET /6/ HTTP") != -1) {
              bleKeyboard.print("6");
            } else if (currentLine.indexOf("GET /7/ HTTP") != -1) {
              bleKeyboard.print("7");
            } else if (currentLine.indexOf("GET /8/ HTTP") != -1) {
              bleKeyboard.print("8");
            } else if (currentLine.indexOf("GET /9/ HTTP") != -1) {
              bleKeyboard.print("9");
            } else if (currentLine.indexOf("GET /A/ HTTP") != -1) {
              bleKeyboard.print("A");
            } else if (currentLine.indexOf("GET /B/ HTTP") != -1) {
              bleKeyboard.print("B");
            } else if (currentLine.indexOf("GET /C/ HTTP") != -1) {
              bleKeyboard.print("C");
            } else if (currentLine.indexOf("GET /D/ HTTP") != -1) {
              bleKeyboard.print("D");
            } else if (currentLine.indexOf("GET /E/ HTTP") != -1) {
              bleKeyboard.print("E");
            } else if (currentLine.indexOf("GET /F/ HTTP") != -1) {
              bleKeyboard.print("F");
            } else if (currentLine.indexOf("GET /G/ HTTP") != -1) {
              bleKeyboard.print("G");
            } else if (currentLine.indexOf("GET /H/ HTTP") != -1) {
              bleKeyboard.print("H");
            } else if (currentLine.indexOf("GET /I/ HTTP") != -1) {
              bleKeyboard.print("I");
            } else if (currentLine.indexOf("GET /J/ HTTP") != -1) {
              bleKeyboard.print("J");
            } else if (currentLine.indexOf("GET /K/ HTTP") != -1) {
              bleKeyboard.print("K");
            } else if (currentLine.indexOf("GET /L/ HTTP") != -1) {
              bleKeyboard.print("L");
            } else if (currentLine.indexOf("GET /M/ HTTP") != -1) {
              bleKeyboard.print("M");
            } else if (currentLine.indexOf("GET /N/ HTTP") != -1) {
              bleKeyboard.print("N");
            } else if (currentLine.indexOf("GET /O/ HTTP") != -1) {
              bleKeyboard.print("O");
            } else if (currentLine.indexOf("GET /P/ HTTP") != -1) {
              bleKeyboard.print("P");
            } else if (currentLine.indexOf("GET /Q/ HTTP") != -1) {
              bleKeyboard.print("Q");
            } else if (currentLine.indexOf("GET /R/ HTTP") != -1) {
              bleKeyboard.print("R");
            } else if (currentLine.indexOf("GET /S/ HTTP") != -1) {
              bleKeyboard.print("S");
            } else if (currentLine.indexOf("GET /T/ HTTP") != -1) {
              bleKeyboard.print("T");
            } else if (currentLine.indexOf("GET /U/ HTTP") != -1) {
              bleKeyboard.print("U");
            } else if (currentLine.indexOf("GET /V/ HTTP") != -1) {
              bleKeyboard.print("V");
            } else if (currentLine.indexOf("GET /W/ HTTP") != -1) {
              bleKeyboard.print("W");
            } else if (currentLine.indexOf("GET /X/ HTTP") != -1) {
              bleKeyboard.print("X");
            } else if (currentLine.indexOf("GET /Y/ HTTP") != -1) {
              bleKeyboard.print("Y");
            } else if (currentLine.indexOf("GET /Z/ HTTP") != -1) {
              bleKeyboard.print("Z");
            } else if (currentLine.indexOf("GET /bt/ HTTP") != -1) {
              bleKeyboard.print("`");
            } else if (currentLine.indexOf("GET /td/ HTTP") != -1) {
              bleKeyboard.print("~");
            } else if (currentLine.indexOf("GET /em/ HTTP") != -1) {
              bleKeyboard.print("!");
            } else if (currentLine.indexOf("GET /at/ HTTP") != -1) {
              bleKeyboard.print("@");
            } else if (currentLine.indexOf("GET /ht/ HTTP") != -1) {
              bleKeyboard.print("#");
            } else if (currentLine.indexOf("GET /dr/ HTTP") != -1) {
              bleKeyboard.print("$");
            } else if (currentLine.indexOf("GET /pc/ HTTP") != -1) {
              bleKeyboard.print("%");
            } else if (currentLine.indexOf("GET /pr/ HTTP") != -1) {
              bleKeyboard.print("^");
            } else if (currentLine.indexOf("GET /ad/ HTTP") != -1) {
              bleKeyboard.print("&");
            } else if (currentLine.indexOf("GET /st/ HTTP") != -1) {
              bleKeyboard.print("*");
            } else if (currentLine.indexOf("GET /op/ HTTP") != -1) {
              bleKeyboard.print("(");
            } else if (currentLine.indexOf("GET /cp/ HTTP") != -1) {
              bleKeyboard.print(")");
            } else if (currentLine.indexOf("GET /ds/ HTTP") != -1) {
              bleKeyboard.print("-");
            } else if (currentLine.indexOf("GET /us/ HTTP") != -1) {
              bleKeyboard.print("_");
            } else if (currentLine.indexOf("GET /eq/ HTTP") != -1) {
              bleKeyboard.print("=");
            } else if (currentLine.indexOf("GET /pl/ HTTP") != -1) {
              bleKeyboard.print("+");
            } else if (currentLine.indexOf("GET /so/ HTTP") != -1) {
              bleKeyboard.print("[");
            } else if (currentLine.indexOf("GET /co/ HTTP") != -1) {
              bleKeyboard.print("{");
            } else if (currentLine.indexOf("GET /sc/ HTTP") != -1) {
              bleKeyboard.print("]");
            } else if (currentLine.indexOf("GET /cc/ HTTP") != -1) {
              bleKeyboard.print("}");
            } else if (currentLine.indexOf("GET /bs/ HTTP") != -1) {
              bleKeyboard.print("\\");
            } else if (currentLine.indexOf("GET /vb/ HTTP") != -1) {
              bleKeyboard.print("|");
            } else if (currentLine.indexOf("GET /sm/ HTTP") != -1) {
              bleKeyboard.print(";");
            } else if (currentLine.indexOf("GET /cn/ HTTP") != -1) {
              bleKeyboard.print(":");
            } else if (currentLine.indexOf("GET /sq/ HTTP") != -1) {
              bleKeyboard.print("'");
            } else if (currentLine.indexOf("GET /dq/ HTTP") != -1) {
              bleKeyboard.print("\"");
            } else if (currentLine.indexOf("GET /cm/ HTTP") != -1) {
              bleKeyboard.print(",");
            } else if (currentLine.indexOf("GET /lt/ HTTP") != -1) {
              bleKeyboard.print("<");
            } else if (currentLine.indexOf("GET /dt/ HTTP") != -1) {
              bleKeyboard.print(".");
            } else if (currentLine.indexOf("GET /gt/ HTTP") != -1) {
              bleKeyboard.print(">");
            } else if (currentLine.indexOf("GET /fs/ HTTP") != -1) {
              bleKeyboard.print("/");
            } else if (currentLine.indexOf("GET /qm/ HTTP") != -1) {
              bleKeyboard.print("?");
            } else if (currentLine.indexOf("GET /ta/ HTTP") != -1) {
              bleKeyboard.write(KEY_TAB);
            } else if (currentLine.indexOf("GET /es/ HTTP") != -1) {
              bleKeyboard.write(KEY_ESC);
            } else if (currentLine.indexOf("GET /ct/ HTTP") != -1) {
              bleKeyboard.press(KEY_LEFT_CTRL);
            } else if (currentLine.indexOf("GET /sf/ HTTP") != -1) {
              bleKeyboard.press(KEY_LEFT_SHIFT);
            } else if (currentLine.indexOf("GET /al/ HTTP") != -1) {
              bleKeyboard.press(KEY_LEFT_ALT);
            } else if (currentLine.indexOf("GET /ctr/ HTTP") != -1) {
              bleKeyboard.release(KEY_LEFT_CTRL);
            } else if (currentLine.indexOf("GET /sfr/ HTTP") != -1) {
              bleKeyboard.release(KEY_LEFT_SHIFT);
            } else if (currentLine.indexOf("GET /alr/ HTTP") != -1) {
              bleKeyboard.release(KEY_LEFT_ALT);
            }
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }
    // close the connection:
    client.stop();
    //    Serial.println("Client Disconnected.");
  }
}
