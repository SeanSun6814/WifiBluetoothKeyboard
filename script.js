let inputbox, serverIpInputbox, statusBox, avgSpeedBox, speedometerProgressbar, queueSizeBox, displayTextBox;
let modifierKeyDown = { "Shift": false, "Alt": false, "Control": false };
let sendQueue = [];
let sleeping = true, listening = false, connected = false, requestListening = true;
let avgSpeed = 0, requestCount = 0, avgSpeedStartTime = 0, avgSpeedBackgroundWorker = false;
let displayTextArr = ["", "", "", "", ""];

function init() {
    inputbox = document.getElementById("inputbox");
    serverIpInputbox = document.getElementById("serverIp");
    statusBox = document.getElementById("status");
    avgSpeedBox = document.getElementById("avgSpd");
    speedometerProgressbar = document.getElementById("speedbox-score");
    queueSizeBox = document.getElementById("queueSize");
    displayTextBox = document.getElementById("displayText");

    inputbox.oninput = inputboxHandler;

    document.body.addEventListener("click", (event) => {
        if (event.target !== event.currentTarget) return;
        handleBodyClick();
    });

    document.body.addEventListener('keyup', (event) => {
        if (event.target !== event.currentTarget) return;
        if (!listening) return;
        event.preventDefault();
        document.body.focus();
        let char = event.key;
        if (char !== "Shift" && char !== "Alt" && char !== "Control") return;
        if (modifierKeyDown[char] === false) return;
        modifierKeyDown[char] = false;
        char += "Up";
        console.log("Released key in body: ", char);
        requestSend([char]);
    });

    document.body.addEventListener('keydown', (event) => {
        if (event.target !== event.currentTarget) return;
        if (!listening) return;
        event.preventDefault();
        document.body.focus();
        let char = event.key;
        if (char === "Shift" || char === "Alt" || char === "Control") {
            if (modifierKeyDown[char] === true) return;
            modifierKeyDown[char] = true;
            char += "Down";
        }
        console.log("Pressed key in body: ", char);
        requestSend([char]);
    });
    updateConnectionStatusLabel(false);
    requestUpdateConnectionStatus();
    setInterval(() => {
        requestUpdateConnectionStatus();
    }, 5000);
    updateAvgSpeedLabel(0);
    createAvgSpeedBackgroundWorker();
}

function onConnect() {
    requestSend(["ShiftUp", "AltUp", "ControlUp"]);
    if (requestListening) {
        listening = true;
        requestListening = false;
    }
}
function handleClearQueueButton() {
    clearSendQueue();
    document.body.focus();
}

function onDisconnect() {
    clearSendQueue();
    listening = false;
}

function clearSendQueue() {
    sendQueue = [];
}

function createAvgSpeedBackgroundWorker() {
    avgSpeedStartTime = getMillis();
    avgSpeedBackgroundWorker = setInterval(() => { updateRunningAverage() }, 200);
}

function removeAvgSpeedBackgroundWorker() {
    clearInterval(avgSpeedBackgroundWorker);
    avgSpeedBackgroundWorker = false;
}

function handleConnectButton() {
    console.log("Clicked connection button");
    requestUpdateConnectionStatus();
    if (!connected)
        requestListening = true;
    else
        listening = true;
    console.log(connected, listening, requestListening);
    updateConnectionStatusLabel(connected);
}

function handleBodyClick() {
    console.log("Clicked body");
    requestUpdateConnectionStatus();
    listening = false;
    requestListening = false;
    updateConnectionStatusLabel(connected);
}

function requestUpdateConnectionStatus() {
    if (sleeping) requestSend(["status"]);
}

function requestSend(code) {
    sendQueue = sendQueue.concat(code);
    addToDisplayText(code);
    if (sleeping) {
        handleStringArrInput();
    }
    updateQueueSizeLabel();
}

function addToDisplayText(arr) {
    let numLines = displayTextArr.length;
    let line = displayTextArr.pop();
    while (arr.length > 0) {
        while (line.length < 40 && arr.length > 0) {
            let c = arr.shift();
            line += translateDisplayText(c);
            if (c === "Backspace") {
                if (line.length > 0) {
                    line = line.substring(0, line.length - 1);
                }
                //  else {
                //     displayTextArr.unshift("");
                //     line = displayTextArr.pop();
                // }
            }
        }
        displayTextArr.push(line);
        line = "";
    }
    let reanimate = displayTextArr.length !== numLines;
    while (displayTextArr.length > 5) {
        displayTextArr.shift();
    }
    updateDisplayText(reanimate);
}

function updateDisplayText(reanimate) {
    let str = displayTextArr.join("<br>");
    displayTextBox.innerHTML = str +"<span class='blinking-cursor'>|</span>";
    if (!reanimate) return;
    displayTextBox.style.animation = 'none';
    displayTextBox.offsetHeight; /* trigger reflow */
    displayTextBox.style.animation = null;
}

function updateQueueSizeLabel() {
    queueSizeBox.innerHTML = "Clear Queue [" + sendQueue.length + "]";
}
    
function updateConnectionStatusLabel(newStatus) {
    connected = newStatus;
    if (connected) {
        if (listening) {
            statusBox.innerHTML = "Connected";
            statusBox.style.color = "green";
        } else {
            statusBox.innerHTML = "Ready";
            statusBox.style.color = "green";
        }
    } else {
        if (requestListening) {
            statusBox.innerHTML = "Connecting";
            statusBox.style.color = "red";
        } else {
            statusBox.innerHTML = "Disconnected";
            statusBox.style.color = "red";
        }
    }
}

function updateAvgSpeedLabel(reqPerSec) {
    let wpm = reqPerSec * 60 / 7;
    avgSpeedBox.innerHTML = Math.floor(wpm) + " WPM";

    let percent = wpm * 100 / 70;
    if (Math.floor(wpm) === 0) {
        removeAvgSpeedBackgroundWorker();
        percent = 0;
    }
    updatedSpeed = Math.round(percent * 180 / 100) - 45;
    speedometerProgressbar.style.transform = "rotate(" + updatedSpeed + "deg)";
}

function updateRunningAverage() {
    let avgWindowSize = 4;
    let newValue = requestCount * 1000 / (getMillis() - avgSpeedStartTime);
    avgSpeed -= avgSpeed / avgWindowSize;
    avgSpeed += newValue / avgWindowSize;
    updateAvgSpeedLabel(avgSpeed);

    requestCount = 0;
    avgSpeedStartTime = getMillis();
}

function inputboxHandler() {
    console.log("Inputbox string value: " + inputbox.value);
    requestSend(inputbox.value.split(''), true);
    inputbox.value = "";
}

function handleStringArrInput() {
    sleeping = false;
    requestStartTime = getMillis();
    if (sendQueue.length === 0) {
        return sleeping = true;
    }
    let char = sendQueue.shift();
    // console.log("SendSeq: char[" + char + "], str[" + str + "]");
    let requestUrl = "http://" + serverIpInputbox.value + "/" + translateRequest(char) + "/EF/";
    console.log("requestUrl: " + requestUrl);
    makeRequest(requestUrl, (data) => {
        if (data.status !== 200)
            console.log("REQUEST_FAILED [" + data.status + "]: " + requestUrl + "" + data.res);
        // console.log(data);
        if (data.status === 200 && char !== "status") {
            requestCount++;
            if (avgSpeedBackgroundWorker === false) createAvgSpeedBackgroundWorker();
        }
        updateQueueSizeLabel();
        handleStringArrInput();
    });
}

function makeRequest(url, callback) {
    let xmlHttp = new XMLHttpRequest();
    xmlHttp.timeout = 1000;
    xmlHttp.onreadystatechange = function () {
        if (xmlHttp.readyState == 4) {
            let newStatus = xmlHttp.status === 200;
            if (!connected && newStatus) onConnect();
            if (connected && !newStatus) onDisconnect();
            updateConnectionStatusLabel(newStatus);
            callback({ status: xmlHttp.status, res: xmlHttp.responseText });
        }
    }
    xmlHttp.open("GET", url, true);
    xmlHttp.send(null);
}

function getMillis() {
    return Date.now();
}

function translateDisplayText(a) {
    if (a === "Enter") { return ""; }
    else if (a === "Backspace") { return ""; }
    else if (a === "ArrowUp") { return ""; }
    else if (a === "ArrowDown") { return ""; }
    else if (a === "ArrowLeft") { return ""; }
    else if (a === "ArrowRight") { return ""; }
    else if (a === "Delete") { return ""; }
    else if (a === "Tab") { return ""; }
    else if (a === "Escape") { return ""; }
    else if (a === "ControlDown") { return ""; }
    else if (a === "ShiftDown") { return ""; }
    else if (a === "AltDown") { return ""; }
    else if (a === "ControlUp") { return ""; }
    else if (a === "ShiftUp") { return ""; }
    else if (a === "AltUp") { return ""; }
    else if (a === "status") { return ""; }
    return a;
}

function translateRequest(a) {
    if (a === "Enter") { return "ret"; }
    else if (a === "\n") { return "ret"; }
    else if (a === "Backspace") { return "bk"; }
    else if (a === "ArrowUp") { return "up"; }
    else if (a === "ArrowDown") { return "dn"; }
    else if (a === "ArrowLeft") { return "lf"; }
    else if (a === "ArrowRight") { return "rt"; }
    else if (a === "Delete") { return "de"; }
    else if (a === "Tab") { return "ta"; }
    else if (a === "Escape") { return "es"; }
    else if (a === "ControlDown") { return "ct"; }
    else if (a === "ShiftDown") { return "sf"; }
    else if (a === "AltDown") { return "al"; }
    else if (a === "ControlUp") { return "ctr"; }
    else if (a === "ShiftUp") { return "sfr"; }
    else if (a === "AltUp") { return "alr"; }
    else if (a === " ") { return "sp"; }
    else if (a === "`") { return "bt"; }
    else if (a === "~") { return "td"; }
    else if (a === "!") { return "em"; }
    else if (a === "@") { return "at"; }
    else if (a === "#") { return "ht"; }
    else if (a === "$") { return "dr"; }
    else if (a === "%") { return "pc"; }
    else if (a === "^") { return "pr"; }
    else if (a === "&") { return "ad"; }
    else if (a === "*") { return "st"; }
    else if (a === "(") { return "op"; }
    else if (a === ")") { return "cp"; }
    else if (a === "-") { return "ds"; }
    else if (a === "_") { return "us"; }
    else if (a === "=") { return "eq"; }
    else if (a === "+") { return "pl"; }
    else if (a === "[") { return "so"; }
    else if (a === "{") { return "co"; }
    else if (a === "]") { return "sc"; }
    else if (a === "}") { return "cc"; }
    else if (a === "\\") { return "bs"; }
    else if (a === "|") { return "vb"; }
    else if (a === ";") { return "sm"; }
    else if (a === ":") { return "cn"; }
    else if (a === "'") { return "sq"; }
    else if (a === "\"") { return "dq"; }
    else if (a === ",") { return "cm"; }
    else if (a === "<") { return "lt"; }
    else if (a === ".") { return "dt"; }
    else if (a === ">") { return "gt"; }
    else if (a === "/") { return "fs"; }
    else if (a === "?") { return "qm"; }
    return a;
}