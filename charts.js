const chartCtx = document.getElementById("voltageChart").getContext("2d");

const voltageChart = new Chart(chartCtx, {
  type: "line",
  data: {
    labels: [],
    datasets: [
      {
        label: "Cell 1",
        data: [],
        borderColor: "#0d6efd",
        backgroundColor: "rgba(13,110,253,0.12)",
        borderWidth: 3,
        pointRadius: 3,
        tension: 0.35
      },
      {
        label: "Cell 2",
        data: [],
        borderColor: "#dc3545",
        backgroundColor: "rgba(220,53,69,0.12)",
        borderWidth: 3,
        pointRadius: 3,
        tension: 0.35
      },
      {
        label: "Cell 3",
        data: [],
        borderColor: "#fd7e14",
        backgroundColor: "rgba(253,126,20,0.12)",
        borderWidth: 3,
        pointRadius: 3,
        tension: 0.35
      },
      {
        label: "Cell 4",
        data: [],
        borderColor: "#198754",
        backgroundColor: "rgba(25,135,84,0.12)",
        borderWidth: 3,
        pointRadius: 3,
        tension: 0.35
      }
    ]
  },
  options: {
    responsive: true,
    maintainAspectRatio: false,
    animation: {
      duration: 400
    },
    interaction: {
      mode: "index",
      intersect: false
    },
    plugins: {
      legend: {
        display: true,
        position: "top"
      },
      tooltip: {
        enabled: true
      }
    },
    scales: {
      y: {
        min: 0,
        max: 3.3,
        ticks: {
          callback: function(value) {
            return value + " V";
          }
        },
        title: {
          display: true,
          text: "Cell Voltage"
        },
        grid: {
          color: "rgba(0,0,0,0.08)"
        }
      },
      x: {
        title: {
          display: true,
          text: "Live Time"
        },
        grid: {
          color: "rgba(0,0,0,0.05)"
        }
      }
    }
  }
});

function updateVoltageChart(data) {
  if (!data) return;

  const time = new Date().toLocaleTimeString();

  voltageChart.data.labels.push(time);

  voltageChart.data.datasets[0].data.push(Number(data.cell1 || 0));
  voltageChart.data.datasets[1].data.push(Number(data.cell2 || 0));
  voltageChart.data.datasets[2].data.push(Number(data.cell3 || 0));
  voltageChart.data.datasets[3].data.push(Number(data.cell4 || 0));

  if (voltageChart.data.labels.length > 30) {
    voltageChart.data.labels.shift();

    voltageChart.data.datasets.forEach(dataset => {
      dataset.data.shift();
    });
  }

  voltageChart.update();
}
