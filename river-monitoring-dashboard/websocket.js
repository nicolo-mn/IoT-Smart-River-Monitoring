'use strict';
const RILEVATION = 'RILEVATION';
const MODE_CHANGED = 'MODE';
const MANUAL_MODE = 'MANUAL';
const HIGH_WATER_LEVEL_MODE = 'HIGH_WATER_LEVEL';
const FREE_MODE = 'FREE';
const N_MIN = 0.5;
const retryDelay = 5000;
let exampleSocket;
let dataList;
let isManualMode = false;
let myLineChart;

function connectWebSocket() {
    console.log("FUNZIONE WEB SOCKET CONNECT");
    exampleSocket = new WebSocket("ws://localhost:8080");
    dataList = [];
    graphInit();
    
    exampleSocket.onmessage = (event) => {
        // Parse JSON data received from the server
        const jsonPacket = JSON.parse(event.data);
        const type = jsonPacket.type;
        const jsonData = jsonPacket.data;
        if (type === RILEVATION) {
            addRilevationData(jsonData);
            console.log(dataList);
        } else if (type === 'MODE') {
            changeMode();
        }
        // Wait for 3 seconds before sending a response
        setTimeout(() => {
            // Convert the object to JSON and send it to the server
            exampleSocket.send("PROVA");
        }, 3000);
    };
    
    exampleSocket.onclose = (event) => {
        console.error(`WebSocket closed with code ${event.code}. Reconnecting in ${retryDelay / 1000} seconds.`);
        setTimeout(connectWebSocket, retryDelay);
    };
}

function addRilevationData(jsonData) {
    console.log("FUNZIONE ADD RILEVATION DATA");
    if (jsonData.time - dataList[0].time > N_MIN * 60 * 1000) {
        // Rimuovi il primo elemento della lista
        dataList.shift();
    }
    // Add JSON object to dataList
    dataList.push(jsonData);
    // Aggiorna il dataset del grafico
    myLineChart.data.labels = dataList.map(item => item.time);
    myLineChart.data.datasets[0].data = dataList.map(item => item.waterLevel);
    // Aggiorna il grafico
    myLineChart.update();
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

function changeMode() {
}



connectWebSocket();