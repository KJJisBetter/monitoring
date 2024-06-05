document.addEventListener("DOMContentLoaded", function() {
    google.charts.load('current', { packages: ['gauge'] });
    google.charts.setOnLoadCallback(drawGauges);

    //mostly done by me with some help from the internet to find out the right charts and gauges for what I wanted.
    // used the ai for most the styling. I hate css and styling stuff.

    let cpuGauge, memoryGauge;
    let diskIoBar;
    let networkChart;
    const maxDataPoints = 60;

    function drawGauges() {
        const cpuData = google.visualization.arrayToDataTable([
            ['Label', 'Value'],
            ['CPU', 0]
        ]);
        const memoryData = google.visualization.arrayToDataTable([
            ['Label', 'Value'],
            ['Memory', 0]
        ]);

        const options = {
            width: 300,
            height: 300,
            redFrom: 90,
            redTo: 100,
            yellowFrom: 75,
            yellowTo: 90,
            minorTicks: 5
        };

        cpuGauge = new google.visualization.Gauge(document.getElementById('CPU'));
        memoryGauge = new google.visualization.Gauge(document.getElementById('Memory'));

        cpuGauge.draw(cpuData, options);
        memoryGauge.draw(memoryData, options);

        diskIoBar = new ProgressBar.Line('#Disk', {
            strokeWidth: 4,
            easing: 'easeInOut',
            duration: 1400,
            color: '#FFEA82',
            trailColor: '#eee',
            trailWidth: 1,
            svgStyle: { width: '100%', height: '100%' },
            text: {
                style: {
                    color: '#999',
                    position: 'absolute',
                    right: '0',
                    top: '30px',
                    padding: 0,
                    margin: 0,
                    transform: null
                },
                autoStyleContainer: false
            },
            from: { color: '#FFEA82' },
            to: { color: '#ED6A5A' },
            step: (state, bar) => {
                bar.setText(`${bar.value().toFixed(3)} seconds`);
            }
        });

        // Create and configure the Chart.js line chart for network traffic
        const ctx = document.getElementById('networkChart').getContext('2d');
        networkChart = new Chart(ctx, {
            type: 'line',
            data: {
                labels: [], 
                datasets: [{
                    label: 'Network In (KiBs)',
                    borderColor: 'rgba(75, 192, 192, 1)',
                    backgroundColor: 'rgba(75, 192, 192, 0.2)',
                    data: [] 
                }, {
                    label: 'Network Out (KiBs)',
                    borderColor: 'rgba(255, 99, 132, 1)',
                    backgroundColor: 'rgba(255, 99, 132, 0.2)',
                    data: [] 
                }]
            },
            options: {
                scales: {
                    x: {
                        type: 'time',
                        time: {
                            unit: 'second',
                            displayFormats: {
                                second: 'h:mm:ss a'
                            }
                        },
                        title: {
                            display: true,
                            text: 'Time'
                        }
                    },
                    y: {
                        beginAtZero: true,
                        title: {
                            display: true,
                            text: 'KiBs'
                        }
                    }
                }
            }
        });

        fetchData(cpuData, memoryData);
    }

    function fetchData(cpuData, memoryData) {
        fetch('/cpu/usage')
            .then(response => response.json())
            .then(data => {
                cpuData.setValue(0, 1, data.cpu_usage);
                cpuGauge.draw(cpuData, null);
            });

        fetch('/memory/usage')
            .then(response => response.json())
            .then(data => {
                memoryData.setValue(0, 1, data.memory_usage);
                memoryGauge.draw(memoryData, null);
            });

        fetch('/disk/usage')
            .then(response => response.json())
            .then(data => {
                const diskIoSecondsString = data.disk_usage; 
                const diskIoSeconds = parseFloat(diskIoSecondsString);
                console.log('diskIoSeconds:', diskIoSeconds, typeof diskIoSeconds);
                if (!isNaN(diskIoSeconds)) {
                    diskIoBar.animate(diskIoSeconds / 1024);
                    diskIoBar.setText(`${diskIoSeconds.toFixed(3)} seconds`); 
                } else {
                    console.error('disk_usage is not a valid number:', diskIoSecondsString);
                }
            });

        fetch('/network/usage/1')
            .then(response => response.json())
            .then(data => {
                const now = new Date();
                const networkIn = data.network_in; 
                const networkOut = data.network_out; 
                addNetworkData(networkChart, now, networkIn, networkOut);
            })
            .catch(error => console.error('Error fetching network data:', error));

        setTimeout(() => fetchData(cpuData, memoryData), 500);
    }

    function addNetworkData(chart, label, networkIn, networkOut) {
        if (chart.data.labels.length >= maxDataPoints) {
            chart.data.labels.shift(); 
            chart.data.datasets[0].data.shift(); 
            chart.data.datasets[1].data.shift(); 
        }
        chart.data.labels.push(label);
        chart.data.datasets[0].data.push(networkIn);
        chart.data.datasets[1].data.push(networkOut);
        chart.update();
    }
});
