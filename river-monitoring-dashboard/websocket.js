'use strict';

let webSocket;
let dataList = [];
let myLineChart = graphInit(dataList);

// Connects to the Java application through a WebSocket
function connectWebSocket() {
    webSocket = new WebSocket("ws://localhost:8080");
    dataList = [];
    
    webSocket.onmessage = (event) => {
        const jsonPacket = JSON.parse(event.data);
        const type = jsonPacket.type;
        if (type === 'RILEVATION') {
            addRilevationData(jsonPacket, dataList, myLineChart);
        } else if (type === 'CHANGE_MODE') {
            changeMode(jsonPacket);
        } else if (type === 'SET_VALVE_MANUAL') {
            changeValveOpening(jsonPacket);
        } else if (type === 'CHANGE_STATE') {
            changeState(jsonPacket);
        }
    };
    
    document.getElementById("submitBtn").onclick = () => {
        const valveOpening = document.getElementById("valve-opening-input").value;
        document.getElementById("valve-opening").innerHTML = valveOpening + "%";
        webSocket.send(JSON.stringify({
            type: 'SET_VALVE_AUTOMATIC',
            valveOpening: valveOpening
        }));
    };

    webSocket.onclose = (event) => {
        connectWebSocket();
    };

    webSocket.onopen = () => {
        document.getElementById("valve-opening").innerHTML = "0%";
        document.getElementById("state").innerHTML = 'NORMAL';
        document.getElementById("alert").innerHTML = "";
        document.getElementById("submitBtn").disabled = false;
    }
}


// Displays the value of the range input
document.getElementById("valve-opening-input").addEventListener("input", function() {
    document.getElementById("input-value").innerHTML = this.value;
});

connectWebSocket();
