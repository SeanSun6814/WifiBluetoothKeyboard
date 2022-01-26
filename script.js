let inputbox, serverIpInputbox, statusBox, avgSpeedBox;
let modifierKeyDown = { "Shift": false, "Alt": false, "Control": false };
let sendQueue = [];
let sleeping = true;
let avgSpeed = 0, requestCount = 0, avgSpeedStartTime = 0;

function init() {
    inputbox = document.getElementById("inputbox");
    serverIpInputbox = document.getElementById("serverIp");
    statusBox = document.getElementById("status");
    avgSpeedBox = document.getElementById("avgSpd");

    inputbox.oninput = inputboxHandler;
    document.body.addEventListener('keyup', (event) => {
        if (event.target !== event.currentTarget) return;
        let char = event.key;
        if (char !== "Shift" && char !== "Alt" && char !== "Control") return;
        if (modifierKeyDown[char] === false) return;
        modifierKeyDown[char] = false;
        char += "Up";
        event.preventDefault();
        console.log("Released key in body: ", char);
        requestSend(char);
    });

    document.body.addEventListener('keydown', (event) => {
        if (event.target !== event.currentTarget) return;
        let char = event.key;
        if (char === "Shift" || char === "Alt" || char === "Control") {
            if (modifierKeyDown[char] === true) return;
            modifierKeyDown[char] = true;
            char += "Down";
            event.preventDefault();
        }
        console.log("Pressed key in body: ", char);
        requestSend(char);
    });
    updateConnectionStatus();
    setInterval(() => {
        updateConnectionStatus();
    }, 5000);
    avgSpeedStartTime = getMillis();
    setInterval(() => { updateRunningAverage() }, 1000);
}

function handleBodyClick() {
    console.log("Clicked body");
    updateConnectionStatus();
}

function updateConnectionStatus() {
    if (sleeping) requestSend("status");
}

function requestSend(code, isArr) {
    if (isArr) {
        sendQueue = sendQueue.concat(code);
    } else {
        sendQueue.push(code);
    }
    if (sleeping) {
        handleStringArrInput();
    }
}

function updateStatusLabel(connected) {
    if(connected){
        statusBox.innerHTML = "Connected";
        statusBox.style.color = "green";     
    } else {
        statusBox.innerHTML = "Disconnected";
        statusBox.style.color = "red";
    }
}

function updateAvgSpeedLabel(reqPerSec) {
    let wpm = reqPerSec * 60 / 50;
    avgSpeedBox.innerHTML = Math.floor(wpm) + " WPM";
}

function updateRunningAverage() {
    let avgWindowSize = 10;
    let newValue = requestCount * 1000 / (getMillis() - avgSpeedStartTime);
    requestCount = 0;
    avgSpeedStartTime = getMillis();
    
    avgSpeed -= avgSpeed / avgWindowSize;
    avgSpeed += newValue / avgWindowSize;
    updateAvgSpeedLabel(avgSpeed);
    console.log(newValue, getMillis() - avgSpeedStartTime, avgSpeed);
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
        // updateRunningAverage();
        // clearInterval(avgSpeedBackgroundWorker);
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
        handleStringArrInput();
    });
}

function makeRequest(url, callback) {
    let xmlHttp = new XMLHttpRequest();
    xmlHttp.timeout = 1000;
    xmlHttp.onreadystatechange = function () {
        if (xmlHttp.readyState == 4) {
            updateStatusLabel(xmlHttp.status === 200);
            if (xmlHttp.status === 200) requestCount++;
            callback({ status: xmlHttp.status, res: xmlHttp.responseText });
        }
    }
    xmlHttp.open("GET", url, true);
    xmlHttp.send(null);
}

function getMillis() {
    return Date.now();
}

function translateRequest(a) {
    if (a === "Enter") { return "ret" }
    else if (a === "\n") { return "ret" }
    else if (a === "Backspace") { return "bk" }
    else if (a === "ArrowUp") { return "up" }
    else if (a === "ArrowDown") { return "dn" }
    else if (a === "ArrowLeft") { return "lf" }
    else if (a === "ArrowRight") { return "rt" }
    else if (a === "Delete") { return "de" }
    else if (a === "Tab") { return "ta" }
    else if (a === "Escape") { return "es" }
    else if (a === "ControlDown") { return "ct" }
    else if (a === "ShiftDown") { return "sf" }
    else if (a === "AltDown") { return "al" }
    else if (a === "ControlUp") { return "ctr" }
    else if (a === "ShiftUp") { return "sfr" }
    else if (a === "AltUp") { return "alr" }
    else if (a === " ") { return "sp" }
    else if (a === "`") { return "bt" }
    else if (a === "~") { return "td" }
    else if (a === "!") { return "em" }
    else if (a === "@") { return "at" }
    else if (a === "#") { return "ht" }
    else if (a === "$") { return "dr" }
    else if (a === "%") { return "pc" }
    else if (a === "^") { return "pr" }
    else if (a === "&") { return "ad" }
    else if (a === "*") { return "st" }
    else if (a === "(") { return "op" }
    else if (a === ")") { return "cp" }
    else if (a === "-") { return "ds" }
    else if (a === "_") { return "us" }
    else if (a === "=") { return "eq" }
    else if (a === "+") { return "pl" }
    else if (a === "[") { return "so" }
    else if (a === "{") { return "co" }
    else if (a === "]") { return "sc" }
    else if (a === "}") { return "cc" }
    else if (a === "\\") { return "bs" }
    else if (a === "|") { return "vb" }
    else if (a === ";") { return "sm" }
    else if (a === ":") { return "cn" }
    else if (a === "'") { return "sq" }
    else if (a === "\"") { return "dq" }
    else if (a === ",") { return "cm" }
    else if (a === "<") { return "lt" }
    else if (a === ".") { return "dt" }
    else if (a === ">") { return "gt" }
    else if (a === "/") { return "fs" }
    else if (a === "?") { return "qm" }
    return a;
}