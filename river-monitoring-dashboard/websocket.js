'use strict';
const N_MIN = 5;

let webSocket;
let dataList = [];
let myLineChart;

// Connects to the Java application through a WebSocket
function connectWebSocket() {
    webSocket = new WebSocket("ws://localhost:8080");
    dataList = [];
    
    webSocket.onmessage = (event) => {
        const jsonPacket = JSON.parse(event.data);
        const type = jsonPacket.type;
        if (type === 'RILEVATION') {
            addRilevationData(jsonPacket);
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

// Adds a new rilevation to the graph
function addRilevationData(jsonPacket) {
    const rilevation = jsonPacket.data;
    rilevation.time = Math.round(rilevation.time / 1000);

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

// Changes the mode of the valve (manual or automatic)
function changeMode(jsonPacket) {
    const mode = jsonPacket.mode;
    if (mode === 'MANUAL') {
        document.getElementById("alert").innerHTML = "Manual mode on";
        document.getElementById("submitBtn").disabled = true;
    } else if (mode === 'AUTOMATIC') {
        document.getElementById("alert").innerHTML = "";
        document.getElementById("submitBtn").disabled = false;
    }
}

// Changes the valve opening based on received data
function changeValveOpening(jsonPacket) {
    if ('valveOpening' in jsonPacket) {
        document.getElementById("valve-opening").innerHTML = jsonPacket.valveOpening + "%";
    }
}

// Changes the state of the river
function changeState(jsonPacket) {
    document.getElementById("state").innerHTML = jsonPacket.state;
    changeValveOpening(jsonPacket);
}

// Graph initialization
function graphInit() {
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

// Displays the value of the range input
document.getElementById("valve-opening-input").addEventListener("input", function() {
    document.getElementById("input-value").innerHTML = this.value;
});

graphInit();
connectWebSocket();
