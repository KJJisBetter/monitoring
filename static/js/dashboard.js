document.addEventListener("DOMContentLoaded", function() {
    google.charts.load('current', { packages: ['gauge'] });
    google.charts.setOnLoadCallback(drawGauges);

    let cpuGauge, memoryGauge;
    let diskIoBar;

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
        })

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
                    const maxIoSeconds = 10; 
                    const diskIoNormalized = Math.min(diskIoSeconds / maxIoSeconds, 1.0); 
                    diskIoBar.animate(diskIoNormalized); 
                    diskIoBar.setText(`${diskIoSeconds.toFixed(3)} seconds`);
                } else {
                    console.error('disk_usage is not a valid number:', diskIoSecondsString);
                }
            });

        setTimeout(() => fetchData(cpuData, memoryData), 500);
    }
});
