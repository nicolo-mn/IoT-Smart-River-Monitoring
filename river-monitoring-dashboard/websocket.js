'use strict';
const N_MIN = 5;
const retryDelay = 5000;

let webSocket;
let dataList = [];
let myLineChart;

function connectWebSocket() {
    console.log("FUNZIONE WEB SOCKET CONNECT");
    webSocket = new WebSocket("ws://localhost:8080");
    dataList = [];
    
    webSocket.onmessage = (event) => {
        // Parse JSON data received from the server
        const jsonPacket = JSON.parse(event.data);
        const type = jsonPacket.type;
        if (type === 'RILEVATION') {
            addRilevationData(jsonPacket);
            console.log(dataList);
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
        console.error(`WebSocket closed with code ${event.code}. Reconnecting in ${retryDelay / 1000} seconds.`);
        setTimeout(connectWebSocket, retryDelay);
    };

    webSocket.onopen = () => {
        document.getElementById("valve-opening").innerHTML = "0%";
        document.getElementById("state").innerHTML = 'NORMAL';
        document.getElementById("alert").innerHTML = "";
        document.getElementById("submitBtn").disabled = false;
    }
}

function addRilevationData(jsonPacket) {
    const rilevation = jsonPacket.data;
    rilevation.time = Math.round(rilevation.time / 1000);
    console.log("FUNZIONE ADD RILEVATION DATA");
    while (dataList.length > 0 && rilevation.time - dataList[0].time > N_MIN * 60) {
        dataList.shift();
    }
    // In case ESP restarts
    if (dataList.length > 0 && rilevation.time < dataList[dataList.length - 1].time) {
        dataList = [];
    }
    dataList.push(rilevation);
    myLineChart.data.labels = dataList.map(item => item.time);
    myLineChart.data.datasets[0].data = dataList.map(item => item.waterLevel);
    myLineChart.update();
}

function changeMode(jsonPacket) {
    const mode = jsonPacket.mode;
    console.log("FUNZIONE CHANGE MODE");
    if (mode === 'MANUAL') {
        document.getElementById("alert").innerHTML = "Manual mode on";
        document.getElementById("submitBtn").disabled = true;
    } else if (mode === 'AUTOMATIC') {
        document.getElementById("alert").innerHTML = "";
        document.getElementById("submitBtn").disabled = false;
    }
}

function changeValveOpening(jsonPacket) {
    console.log("FUNZIONE CHANGE VALVE OPENING");
    if ('valveOpening' in jsonPacket) {
        document.getElementById("valve-opening").innerHTML = jsonPacket.valveOpening + "%";
    }
}

function changeState(jsonPacket) {
    console.log("FUNZIONE CHANGE STATE");
    document.getElementById("state").innerHTML = jsonPacket.state;
    changeValveOpening(jsonPacket);
}

function graphInit() {
    console.log("FUNZIONE GRAPH INIT");
    const canvas = document.getElementById('myLineChart');
    const ctx = canvas.getContext('2d');
    myLineChart = new Chart(ctx, {
        type: 'line',
        data: {
            labels: dataList.map(item => item.time),
            datasets: [{
                label: 'Water level change',
                data: dataList.map(item => item.waterLevel),
                fill: false,
                borderColor: 'rgba(75, 192, 192, 1)',
                borderWidth: 1
            }]
        },
        options: {
            responsive: true, 
            scales: {
                x: {
                    display: true,
                    title: {
                        display: true,
                        text: 'Seconds from the program\'s start'
                    }
                },
                y: {
                    display: true,
                    title: {
                        display: true,
                        text: 'Water level'
                    }
                }
            }
        }
    });
}


document.getElementById("valve-opening-input").addEventListener("input", function() {
    document.getElementById("input-value").innerHTML = this.value;
});
graphInit();
connectWebSocket();
