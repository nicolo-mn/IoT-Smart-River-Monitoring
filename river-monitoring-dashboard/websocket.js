'use strict';
const RILEVATION = 'RILEVATION';
const MODE = 'MODE';
const VALVE_OPENING = 'VALVE_OPENING';
const MANUAL_MODE = 'MANUAL';
const AUTOMATIC_MODE = 'AUTOMATIC';
const STATE = 'STATE';
const N_MIN = 0.5;

// const NORMAL_STATE = 'NORMAL';
// const ALARM_TOO_LOW_STATE = 'ALARM_TOO_LOW';
// const PRE_ALARM_TOO_HIGH_STATE = 'PRE_ALARM_TOO_HIGH';
// const ALARM_TOO_HIGH_STATE = 'ALARM_TOO_HIGH';
// const ALARM_TOO_HIGH_CRITIC_STATE = 'ALARM_TOO_HIGH_CRITIC';

const retryDelay = 5000;
let exampleSocket;
let dataList = [];
let myLineChart;

function connectWebSocket() {
    console.log("FUNZIONE WEB SOCKET CONNECT");
    exampleSocket = new WebSocket("ws://localhost:8080");
    dataList = [];
    
    exampleSocket.onmessage = (event) => {
        // Parse JSON data received from the server
        const jsonPacket = JSON.parse(event.data);
        const type = jsonPacket.type;
        if (type === RILEVATION) {
            addRilevationData(jsonPacket);
            console.log(dataList);
        } else if (type === MODE) {
            changeMode(jsonPacket);
        } else if (type === VALVE_OPENING) {
            changeValveOpening(jsonPacket);
        } else if (type === STATE) {
            changeState(jsonPacket);
        }
        // Wait for 3 seconds before sending a response
        // setTimeout(() => {
        //     // Convert the object to JSON and send it to the server
        //     exampleSocket.send("PROVA");
        // }, 3000);
    };
    
    document.getElementById("submitBtn").onclick = () => {
        const valveOpening = document.getElementById("valve-opening-input").value;
        exampleSocket.send(JSON.stringify({
            valveOpening: valveOpening
        }));
    };

    exampleSocket.onclose = (event) => {
        console.error(`WebSocket closed with code ${event.code}. Reconnecting in ${retryDelay / 1000} seconds.`);
        setTimeout(connectWebSocket, retryDelay);
    };
}

function addRilevationData(jsonPacket) {
    const rilevation = jsonPacket.data;
    console.log("FUNZIONE ADD RILEVATION DATA");
    while (dataList.length > 0 && rilevation.time - dataList[0].time > N_MIN * 60 * 1000) {
        // Rimuovi il primo elemento della lista
        dataList.shift();
    }
    // Add JSON object to dataList
    dataList.push(rilevation);
    // Aggiorna il dataset del grafico
    myLineChart.data.labels = dataList.map(item => item.time);
    myLineChart.data.datasets[0].data = dataList.map(item => item.waterLevel);
    // Aggiorna il grafico
    myLineChart.update();
}

function changeMode(jsonPacket) {
    const mode = jsonPacket.mode;
    console.log("FUNZIONE CHANGE MODE");
    if (mode === MANUAL_MODE) {
        document.getElementById("alert").innerHTML = "Manual mode on";
        document.getElementById("submitBtn").disabled = true;
    } else if (mode === AUTOMATIC_MODE) {
        document.getElementById("alert").innerHTML = "";
        document.getElementById("submitBtn").disabled = false;
    }
}

function changeValveOpening(jsonPacket) {
    console.log("FUNZIONE CHANGE VALVE OPENING");
    document.getElementById("valve-opening").innerHTML = jsonPacket.opening + "%";
}

function changeState(jsonPacket) {
    console.log("FUNZIONE CHANGE STATE");
    document.getElementById("state").innerHTML = jsonPacket.state;
    changeValveOpening(jsonPacket);
}

function graphInit() {
    console.log("FUNZIONE GRAPH INIT");
    // Prendi il riferimento al canvas e al contesto 2D
    const canvas = document.getElementById('myLineChart');
    const ctx = canvas.getContext('2d');
    // Inizializza il grafico a linea
    myLineChart = new Chart(ctx, {
        type: 'line',
        data: {
            labels: dataList.map(item => item.time),
            datasets: [{
                label: 'Livello dell\'acqua',
                data: dataList.map(item => item.waterLevel),
                fill: false,
                borderColor: 'rgba(75, 192, 192, 1)',
                borderWidth: 1
            }]
        },
        options: {
            responsive: true // Enables responsiveness
        }
    });
}

graphInit();
connectWebSocket();