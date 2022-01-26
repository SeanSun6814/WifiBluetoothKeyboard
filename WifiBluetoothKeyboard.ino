#include <BleKeyboard.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>

#define LED_BUILTIN 2
#define USING_CORS_FEATURE true

const char* ssid = "WifiKeyboard";
const char* password = "mySecretPassword";

WiFiServer server(80);
BleKeyboard bleKeyboard;

void setup() {
//  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
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
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
        if (currentLine.startsWith("GET") && currentLine.endsWith("/EF")) {
          performRequest(currentLine);
        }
      }
    }
    client.stop();
  }
}

void performRequest(String line) {
  int len = line.length();
  String action = "";
  for (int i = 0; i < len; i++) {
    char c = line.charAt(i);
    if (c == '/') {
      performAction(action);
      
      action = "";
    } else {
      action += c;
    }
  }
}


void performAction(String code) {
  code.trim();
//  Serial.println("Processing: " + code);
  if (code.length() == 0 || code.equalsIgnoreCase("GET")) return;
  delay(250);
  if (code.equals("ret")) {
    bleKeyboard.write(KEY_RETURN);
  } else if (code.equals("bk")) {
    bleKeyboard.write(KEY_BACKSPACE);
  } else if (code.equals("up")) {
    bleKeyboard.write(KEY_UP_ARROW);
  } else if (code.equals("dn")) {
    bleKeyboard.write(KEY_DOWN_ARROW);
  } else if (code.equals("lf")) {
    bleKeyboard.write(KEY_LEFT_ARROW);
  } else if (code.equals("rt")) {
    bleKeyboard.write(KEY_RIGHT_ARROW);
  } else if (code.equals("de")) {
    bleKeyboard.write(KEY_DELETE);
  } else if (code.equals("sp")) {
    bleKeyboard.print(" ");
  } else if (code.equals("a")) {
    bleKeyboard.print("a");
  } else if (code.equals("b")) {
    bleKeyboard.print("b");
  } else if (code.equals("c")) {
    bleKeyboard.print("c");
  } else if (code.equals("d")) {
    bleKeyboard.print("d");
  } else if (code.equals("e")) {
    bleKeyboard.print("e");
  } else if (code.equals("f")) {
    bleKeyboard.print("f");
  } else if (code.equals("g")) {
    bleKeyboard.print("g");
  } else if (code.equals("h")) {
    bleKeyboard.print("h");
  } else if (code.equals("i")) {
    bleKeyboard.print("i");
  } else if (code.equals("j")) {
    bleKeyboard.print("j");
  } else if (code.equals("k")) {
    bleKeyboard.print("k");
  } else if (code.equals("l")) {
    bleKeyboard.print("l");
  } else if (code.equals("m")) {
    bleKeyboard.print("m");
  } else if (code.equals("n")) {
    bleKeyboard.print("n");
  } else if (code.equals("o")) {
    bleKeyboard.print("o");
  } else if (code.equals("p")) {
    bleKeyboard.print("p");
  } else if (code.equals("q")) {
    bleKeyboard.print("q");
  } else if (code.equals("r")) {
    bleKeyboard.print("r");
  } else if (code.equals("s")) {
    bleKeyboard.print("s");
  } else if (code.equals("t")) {
    bleKeyboard.print("t");
  } else if (code.equals("u")) {
    bleKeyboard.print("u");
  } else if (code.equals("v")) {
    bleKeyboard.print("v");
  } else if (code.equals("w")) {
    bleKeyboard.print("w");
  } else if (code.equals("x")) {
    bleKeyboard.print("x");
  } else if (code.equals("y")) {
    bleKeyboard.print("y");
  } else if (code.equals("z")) {
    bleKeyboard.print("z");
  } else if (code.equals("0")) {
    bleKeyboard.print("0");
  } else if (code.equals("1")) {
    bleKeyboard.print("1");
  } else if (code.equals("2")) {
    bleKeyboard.print("2");
  } else if (code.equals("3")) {
    bleKeyboard.print("3");
  } else if (code.equals("4")) {
    bleKeyboard.print("4");
  } else if (code.equals("5")) {
    bleKeyboard.print("5");
  } else if (code.equals("6")) {
    bleKeyboard.print("6");
  } else if (code.equals("7")) {
    bleKeyboard.print("7");
  } else if (code.equals("8")) {
    bleKeyboard.print("8");
  } else if (code.equals("9")) {
    bleKeyboard.print("9");
  } else if (code.equals("A")) {
    bleKeyboard.print("A");
  } else if (code.equals("B")) {
    bleKeyboard.print("B");
  } else if (code.equals("C")) {
    bleKeyboard.print("C");
  } else if (code.equals("D")) {
    bleKeyboard.print("D");
  } else if (code.equals("E")) {
    bleKeyboard.print("E");
  } else if (code.equals("F")) {
    bleKeyboard.print("F");
  } else if (code.equals("G")) {
    bleKeyboard.print("G");
  } else if (code.equals("H")) {
    bleKeyboard.print("H");
  } else if (code.equals("I")) {
    bleKeyboard.print("I");
  } else if (code.equals("J")) {
    bleKeyboard.print("J");
  } else if (code.equals("K")) {
    bleKeyboard.print("K");
  } else if (code.equals("L")) {
    bleKeyboard.print("L");
  } else if (code.equals("M")) {
    bleKeyboard.print("M");
  } else if (code.equals("N")) {
    bleKeyboard.print("N");
  } else if (code.equals("O")) {
    bleKeyboard.print("O");
  } else if (code.equals("P")) {
    bleKeyboard.print("P");
  } else if (code.equals("Q")) {
    bleKeyboard.print("Q");
  } else if (code.equals("R")) {
    bleKeyboard.print("R");
  } else if (code.equals("S")) {
    bleKeyboard.print("S");
  } else if (code.equals("T")) {
    bleKeyboard.print("T");
  } else if (code.equals("U")) {
    bleKeyboard.print("U");
  } else if (code.equals("V")) {
    bleKeyboard.print("V");
  } else if (code.equals("W")) {
    bleKeyboard.print("W");
  } else if (code.equals("X")) {
    bleKeyboard.print("X");
  } else if (code.equals("Y")) {
    bleKeyboard.print("Y");
  } else if (code.equals("Z")) {
    bleKeyboard.print("Z");
  } else if (code.equals("bt")) {
    bleKeyboard.print("`");
  } else if (code.equals("td")) {
    bleKeyboard.print("~");
  } else if (code.equals("em")) {
    bleKeyboard.print("!");
  } else if (code.equals("at")) {
    bleKeyboard.print("@");
  } else if (code.equals("ht")) {
    bleKeyboard.print("#");
  } else if (code.equals("dr")) {
    bleKeyboard.print("$");
  } else if (code.equals("pc")) {
    bleKeyboard.print("%");
  } else if (code.equals("pr")) {
    bleKeyboard.print("^");
  } else if (code.equals("ad")) {
    bleKeyboard.print("&");
  } else if (code.equals("st")) {
    bleKeyboard.print("*");
  } else if (code.equals("op")) {
    bleKeyboard.print("(");
  } else if (code.equals("cp")) {
    bleKeyboard.print(")");
  } else if (code.equals("ds")) {
    bleKeyboard.print("-");
  } else if (code.equals("us")) {
    bleKeyboard.print("_");
  } else if (code.equals("eq")) {
    bleKeyboard.print("=");
  } else if (code.equals("pl")) {
    bleKeyboard.print("+");
  } else if (code.equals("so")) {
    bleKeyboard.print("[");
  } else if (code.equals("co")) {
    bleKeyboard.print("{");
  } else if (code.equals("sc")) {
    bleKeyboard.print("]");
  } else if (code.equals("cc")) {
    bleKeyboard.print("}");
  } else if (code.equals("bs")) {
    bleKeyboard.print("\\");
  } else if (code.equals("vb")) {
    bleKeyboard.print("|");
  } else if (code.equals("sm")) {
    bleKeyboard.print(";");
  } else if (code.equals("cn")) {
    bleKeyboard.print(":");
  } else if (code.equals("sq")) {
    bleKeyboard.print("'");
  } else if (code.equals("dq")) {
    bleKeyboard.print("\"");
  } else if (code.equals("cm")) {
    bleKeyboard.print(",");
  } else if (code.equals("lt")) {
    bleKeyboard.print("<");
  } else if (code.equals("dt")) {
    bleKeyboard.print(".");
  } else if (code.equals("gt")) {
    bleKeyboard.print(">");
  } else if (code.equals("fs")) {
    bleKeyboard.print("/");
  } else if (code.equals("qm")) {
    bleKeyboard.print("?");
  } else if (code.equals("ta")) {
    bleKeyboard.write(KEY_TAB);
  } else if (code.equals("es")) {
    bleKeyboard.write(KEY_ESC);
  } else if (code.equals("ct")) {
    bleKeyboard.press(KEY_LEFT_CTRL);
  } else if (code.equals("sf")) {
    bleKeyboard.press(KEY_LEFT_SHIFT);
  } else if (code.equals("al")) {
    bleKeyboard.press(KEY_LEFT_ALT);
  } else if (code.equals("ctr")) {
    bleKeyboard.release(KEY_LEFT_CTRL);
  } else if (code.equals("sfr")) {
    bleKeyboard.release(KEY_LEFT_SHIFT);
  } else if (code.equals("alr")) {
    bleKeyboard.release(KEY_LEFT_ALT);
  }
}
