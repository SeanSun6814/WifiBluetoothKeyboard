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
    bool sendPage = false;
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
            if (sendPage) {
              Serial.write("SENDING_PAGE...\n");
              client.println("<!DOCTYPE html><html lang=\"en\"><head> <meta charset=\"UTF-8\"> <title>Keyboard</title> <script src=\"script.js\"></script> <link rel=\"stylesheet\" href=\"style.css\"></head><body onload=\"init()\"> <input id=\"serverIp\" value=\"192.168.4.1\" placeholder=\"Server IP Address\" /> <button onclick=\"handleClearQueueButton();\" id=\"queueSize\">Clear Queue</button> <br> <textarea id=\"inputbox\" placeholder=\"Paste text here...\"></textarea> <div class=\"speedbox\"> <div class=\"speedbox__score\" id=\"speedbox-score\" onclick=\"handleConnectButton();\"></div> <div class=\"speedbox__odo\"> <p id=\"status\" style=\"color: red;\">Disconnected</p> <div class=\"speedbox__down\"><i class=\"fa fa-arrow-circle-up\"></i> <span id=\"avgSpd\"></span> </div> </div> <div class=\"speedbox__base\"></div> </div> <div class=\"scroller\"> <span class=\"displayText\" id=\"displayText\">This is the first line <br>second line <br>third line<br>forth line.</span> </div></body><script> let inputbox, serverIpInputbox, statusBox, avgSpeedBox, speedometerProgressbar, queueSizeBox, displayTextBox; let modifierKeyDown = { Shift: false, Alt: false, Control: false }; let sendQueue = []; let sleeping = true, listening = false, connected = false, requestListening = true; let avgSpeed = 0, requestCount = 0, avgSpeedStartTime = 0, avgSpeedBackgroundWorker = false; let displayTextArr = [\"\", \"\", \"\", \"\", \"\"]; function init() { inputbox = document.getElementById(\"inputbox\"); serverIpInputbox = document.getElementById(\"serverIp\"); statusBox = document.getElementById(\"status\"); avgSpeedBox = document.getElementById(\"avgSpd\"); speedometerProgressbar = document.getElementById(\"speedbox-score\"); queueSizeBox = document.getElementById(\"queueSize\"); displayTextBox = document.getElementById(\"displayText\"); inputbox.oninput = inputboxHandler; document.body.addEventListener(\"click\", (event) => { if (event.target !== event.currentTarget) return; handleBodyClick(); }); document.body.addEventListener(\"keyup\", (event) => { if (event.target !== event.currentTarget) return; if (!listening) return; event.preventDefault(); document.body.focus(); let char = event.key; if (char !== \"Shift\" && char !== \"Alt\" && char !== \"Control\") return; if (modifierKeyDown[char] === false) return; modifierKeyDown[char] = false; char += \"Up\"; console.log(\"Released key in body: \", char); requestSend([char]); }); document.body.addEventListener(\"keydown\", (event) => { if (event.target !== event.currentTarget) return; if (!listening) return; event.preventDefault(); document.body.focus(); let char = event.key; if (char === \"Shift\" || char === \"Alt\" || char === \"Control\") { if (modifierKeyDown[char] === true) return; modifierKeyDown[char] = true; char += \"Down\"; } console.log(\"Pressed key in body: \", char); requestSend([char]); }); updateConnectionStatusLabel(false); requestUpdateConnectionStatus(); setInterval(() => { requestUpdateConnectionStatus(); }, 5000); updateAvgSpeedLabel(0); createAvgSpeedBackgroundWorker(); } function onConnect() { requestSend([\"ShiftUp\", \"AltUp\", \"ControlUp\"]); if (requestListening) { listening = true; requestListening = false; } } function handleClearQueueButton() { clearSendQueue(); document.body.focus(); } function onDisconnect() { clearSendQueue(); listening = false; } function clearSendQueue() { sendQueue = []; } function createAvgSpeedBackgroundWorker() { avgSpeedStartTime = getMillis(); avgSpeedBackgroundWorker = setInterval(() => { updateRunningAverage(); }, 200); } function removeAvgSpeedBackgroundWorker() { clearInterval(avgSpeedBackgroundWorker); avgSpeedBackgroundWorker = false; } function handleConnectButton() { console.log(\"Clicked connection button\"); requestUpdateConnectionStatus(); if (!connected) requestListening = true; else listening = true; console.log(connected, listening, requestListening); updateConnectionStatusLabel(connected); } function handleBodyClick() { console.log(\"Clicked body\"); requestUpdateConnectionStatus(); listening = false; requestListening = false; updateConnectionStatusLabel(connected); } function requestUpdateConnectionStatus() { if (sleeping) requestSend([\"status\"]); } function requestSend(code) { sendQueue = sendQueue.concat(code); addToDisplayText(code); if (sleeping) { handleStringArrInput(); } updateQueueSizeLabel(); } function addToDisplayText(arr) { let numLines = displayTextArr.length; let line = displayTextArr.pop(); while (arr.length > 0) { while (line.length < 40 && arr.length > 0) { let c = arr.shift(); line += translateDisplayText(c); if (c === \"Backspace\") { if (line.length > 0) { line = line.substring(0, line.length - 1); } } } displayTextArr.push(line); line = \"\"; } let reanimate = displayTextArr.length !== numLines; while (displayTextArr.length > 5) { displayTextArr.shift(); } updateDisplayText(reanimate); } function updateDisplayText(reanimate) { let str = displayTextArr.join(\"<br>\"); displayTextBox.innerHTML = str + \"<span class=\'blinking-cursor\'>|</span>\"; if (!reanimate) return; displayTextBox.style.animation = \"none\"; displayTextBox.offsetHeight; /* trigger reflow */ displayTextBox.style.animation = null; } function updateQueueSizeLabel() { queueSizeBox.innerHTML = \"Clear Queue [\" + sendQueue.length + \"]\"; } function updateConnectionStatusLabel(newStatus) { connected = newStatus; if (connected) { if (listening) { statusBox.innerHTML = \"Connected\"; statusBox.style.color = \"green\"; } else { statusBox.innerHTML = \"Ready\"; statusBox.style.color = \"green\"; } } else { if (requestListening) { statusBox.innerHTML = \"Connecting\"; statusBox.style.color = \"red\"; } else { statusBox.innerHTML = \"Disconnected\"; statusBox.style.color = \"red\"; } } } function updateAvgSpeedLabel(reqPerSec) { let wpm = (reqPerSec * 60) / 7; avgSpeedBox.innerHTML = Math.floor(wpm) + \" WPM\"; let percent = (wpm * 100) / 210; if (Math.floor(wpm) === 0) { removeAvgSpeedBackgroundWorker(); percent = 0; } updatedSpeed = Math.round((percent * 180) / 100) - 45; speedometerProgressbar.style.transform = \"rotate(\" + updatedSpeed + \"deg)\"; } function updateRunningAverage() { let avgWindowSize = 4; let newValue = (requestCount * 1000) / (getMillis() - avgSpeedStartTime); avgSpeed -= avgSpeed / avgWindowSize; avgSpeed += newValue / avgWindowSize; updateAvgSpeedLabel(avgSpeed); requestCount = 0; avgSpeedStartTime = getMillis(); } function inputboxHandler() { console.log(\"Inputbox string value: \" + inputbox.value); requestSend(inputbox.value.split(\"\"), true); inputbox.value = \"\"; } function handleStringArrInput() { sleeping = false; requestStartTime = getMillis(); if (sendQueue.length === 0) { return (sleeping = true); } let char = sendQueue.shift(); let requestUrl = \"http://\" + serverIpInputbox.value + \"/\" + translateRequest(char) + \"/\"; console.log(\"requestUrl: \" + requestUrl); makeRequest(requestUrl, (data) => { if (data.status !== 200) console.log(\"REQUEST_FAILED [\" + data.status + \"]: \" + requestUrl + \"\" + data.res); if (data.status === 200 && char !== \"status\") { requestCount++; if (avgSpeedBackgroundWorker === false) createAvgSpeedBackgroundWorker(); } updateQueueSizeLabel(); handleStringArrInput(); }); } function makeRequest(url, callback) { let xmlHttp = new XMLHttpRequest(); xmlHttp.timeout = 1000; xmlHttp.onreadystatechange = function () { if (xmlHttp.readyState == 4) { let newStatus = xmlHttp.status === 200; if (!connected && newStatus) onConnect(); if (connected && !newStatus) onDisconnect(); updateConnectionStatusLabel(newStatus); callback({ status: xmlHttp.status, res: xmlHttp.responseText }); } }; xmlHttp.open(\"GET\", url, true); xmlHttp.send(null); } function getMillis() { return Date.now(); } function translateDisplayText(a) { if (a === \"Enter\") { return \"\"; } else if (a === \"Backspace\") { return \"\"; } else if (a === \"ArrowUp\") { return \"\"; } else if (a === \"ArrowDown\") { return \"\"; } else if (a === \"ArrowLeft\") { return \"\"; } else if (a === \"ArrowRight\") { return \"\"; } else if (a === \"Delete\") { return \"\"; } else if (a === \"Tab\") { return \"\"; } else if (a === \"Escape\") { return \"\"; } else if (a === \"ControlDown\") { return \"\"; } else if (a === \"ShiftDown\") { return \"\"; } else if (a === \"AltDown\") { return \"\"; } else if (a === \"ControlUp\") { return \"\"; } else if (a === \"ShiftUp\") { return \"\"; } else if (a === \"AltUp\") { return \"\"; } else if (a === \"status\") { return \"\"; } return a; } function translateRequest(a) { if (a === \"Enter\") { return \"ret\"; } else if (a === \"\\n\") { return \"ret\"; } else if (a === \"Backspace\") { return \"bk\"; } else if (a === \"ArrowUp\") { return \"up\"; } else if (a === \"ArrowDown\") { return \"dn\"; } else if (a === \"ArrowLeft\") { return \"lf\"; } else if (a === \"ArrowRight\") { return \"rt\"; } else if (a === \"Delete\") { return \"de\"; } else if (a === \"Tab\") { return \"ta\"; } else if (a === \"Escape\") { return \"es\"; } else if (a === \"ControlDown\") { return \"ct\"; } else if (a === \"ShiftDown\") { return \"sf\"; } else if (a === \"AltDown\") { return \"al\"; } else if (a === \"ControlUp\") { return \"ctr\"; } else if (a === \"ShiftUp\") { return \"sfr\"; } else if (a === \"AltUp\") { return \"alr\"; } else if (a === \" \") { return \"sp\"; } else if (a === \"`\") { return \"bt\"; } else if (a === \"~\") { return \"td\"; } else if (a === \"!\") { return \"em\"; } else if (a === \"@\") { return \"at\"; } else if (a === \"#\") { return \"ht\"; } else if (a === \"$\") { return \"dr\"; } else if (a === \"%\") { return \"pc\"; } else if (a === \"^\") { return \"pr\"; } else if (a === \"&\") { return \"ad\"; } else if (a === \"*\") { return \"st\"; } else if (a === \"(\") { return \"op\"; } else if (a === \")\") { return \"cp\"; } else if (a === \"-\") { return \"ds\"; } else if (a === \"_\") { return \"us\"; } else if (a === \"=\") { return \"eq\"; } else if (a === \"+\") { return \"pl\"; } else if (a === \"[\") { return \"so\"; } else if (a === \"{\") { return \"co\"; } else if (a === \"]\") { return \"sc\"; } else if (a === \"}\") { return \"cc\"; } else if (a === \"\\\\\") { return \"bs\"; } else if (a === \"|\") { return \"vb\"; } else if (a === \";\") { return \"sm\"; } else if (a === \":\") { return \"cn\"; } else if (a === \"\'\") { return \"sq\"; } else if (a === \'\"\') { return \"dq\"; } else if (a === \",\") { return \"cm\"; } else if (a === \"<\") { return \"lt\"; } else if (a === \".\") { return \"dt\"; } else if (a === \">\") { return \"gt\"; } else if (a === \"/\") { return \"fs\"; } else if (a === \"?\") { return \"qm\"; } return a; }</script><style> html, body { margin: 0; height: 100%; } p, span { font-family: sans-serif; } .speedbox>* { margin: 0; padding: 0; } .legend { color: #333; } .speedbox { margin: 5em; height: 400px; width: 400px; display: flex; display: -webkit-flex; flex-direction: column; -webkit-flex-direction: column; align-items: center; -webkit-align-items: center; position: relative; } .speedbox__groove { height: 200px; width: 400px; background: transparent; border-top-left-radius: 200px; border-top-right-radius: 200px; border: 40px solid #eee; border-bottom: 0; box-sizing: border-box; position: absolute; left: 0; top: 0; } .speedbox__score { position: absolute; left: 0; top: 0; transform: rotate(-45deg); -webkit-transform: rotate(0deg); height: 400px; width: 400px; background: transparent; border-radius: 50%; border: 20px solid #5c6f7b; border-color: transparent transparent #5c6f7b #5c6f7b; box-sizing: border-box; cursor: pointer; z-index: 1; transition: transform 0.3s ease; } .speedbox__base { width: 480px; height: 200px; background: white; position: relative; top: 200px; z-index: 20; } .speedbox__base:before { content: \"\"; width: 480px; position: absolute; top: 0; border-bottom: 1px solid #eee; box-shadow: 1px 3px 15px rgba(0, 0, 0, 0.5); } .speedbox__odo { text-align: center; position: absolute; color: #5c6f7b; bottom: 200px; left: 50%; transform: translateX(-50%); } .speedbox__odo i { font-size: 13px; opacity: 0.6; } .speedbox__odo>div { margin-bottom: 0; } .speedbox__odo span { font-size: 0.7em; } .speedbox__down { font-size: 25px; text-shadow: 0 0 3px rgba(0, 0, 0, 0.2); line-height: 1.2em; } .scroller { height: 12em; line-height: 3.2em; position: relative; left: 10%; overflow: hidden; } .scroller>span { position: absolute; top: 0; animation: slide 0.5s forwards ease-out; font-weight: bold; margin: auto; } @keyframes slide { 0% { top: -0.4em; } 100% { top: -1.4em; } } .displayText { font-size: 300%; } .blinking-cursor { font-weight: 100; color: #2e3d48; animation: 1s blink step-start infinite; } @keyframes blink { from, to { color: transparent; } 50% { color: grey; } }</style></html>");
            }
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
            } else if (currentLine.indexOf("GET / HTTP") != -1) {
              sendPage = true;
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
