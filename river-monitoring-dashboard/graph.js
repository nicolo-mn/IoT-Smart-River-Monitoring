'use strict';

// Graph initialization
function graphInit(dataList) {
    const canvas = document.getElementById('myLineChart');
    const ctx = canvas.getContext('2d');
    let myLineChart = new Chart(ctx, {
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
    return myLineChart;
}
