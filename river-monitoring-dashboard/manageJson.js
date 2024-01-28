'use strict';

const N_MIN = 5;

// Adds a new rilevation to the graph
function addRilevationData(jsonPacket, dataList, myLineChart) {
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
