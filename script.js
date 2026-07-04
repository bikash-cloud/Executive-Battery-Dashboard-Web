function updateClock() {
  const now = new Date();
  document.getElementById("liveTime").textContent = now.toLocaleTimeString();
}

setInterval(updateClock, 1000);
updateClock();

function getRiskClass(risk) {
  if (risk === "LOW") return "low";
  if (risk === "MEDIUM") return "medium";
  if (risk === "HIGH") return "high";
  if (risk === "CRITICAL") return "critical";
  return "low";
}

function formatValue(value, decimals = 2) {
  if (value === undefined || value === null) return "--";
  return Number(value).toFixed(decimals);
}

function updateDashboard(data) {
  document.getElementById("connectionStatus").textContent = "LIVE";
  document.getElementById("connectionStatus").className = "status online";

  document.getElementById("healthScore").textContent =
    formatValue(data.healthScore, 1) + "%";

  document.getElementById("packVoltage").textContent =
    formatValue(data.packVoltage, 2) + " V";

  document.getElementById("averageVoltage").textContent =
    formatValue(data.averageVoltage, 2) + " V";

  const risk = document.getElementById("riskLevel");
  risk.textContent = data.riskLevel || "--";
  risk.className = getRiskClass(data.riskLevel);

  document.getElementById("cell1").textContent = formatValue(data.cell1, 2) + " V";
  document.getElementById("cell2").textContent = formatValue(data.cell2, 2) + " V";
  document.getElementById("cell3").textContent = formatValue(data.cell3, 2) + " V";
  document.getElementById("cell4").textContent = formatValue(data.cell4, 2) + " V";

  document.getElementById("runtimeState").textContent = data.runtimeState || "--";
  document.getElementById("relayStatus").textContent = data.relayStatus || "--";
  document.getElementById("buzzerStatus").textContent = data.buzzerStatus || "--";
  document.getElementById("imbalance").textContent =
    formatValue(data.imbalance, 2) + "%";

  document.getElementById("lastFault").textContent = data.lastFault || "--";
  document.getElementById("recommendation").textContent =
    data.recommendation || "Waiting for battery telemetry...";

  if (data.runtimeState === "FAULT") {
    addFaultHistory(data.lastFault, data.riskLevel);
  }

  updateVoltageChart(data);
}

let lastLoggedFault = "";

function addFaultHistory(fault, severity) {
  if (!fault || fault === "No Fault") return;

  const currentFault = fault + severity;
  if (currentFault === lastLoggedFault) return;
  lastLoggedFault = currentFault;

  const table = document.getElementById("faultTable");

  if (
    table.children.length === 1 &&
    table.children[0].children[1].textContent === "No fault recorded"
  ) {
    table.innerHTML = "";
  }

  const row = document.createElement("tr");

  row.innerHTML = `
    <td>${new Date().toLocaleTimeString()}</td>
    <td>${fault}</td>
    <td class="${getRiskClass(severity)}">${severity}</td>
    <td>Active</td>
  `;

  table.prepend(row);

  if (table.children.length > 8) {
    table.removeChild(table.lastChild);
  }
}
