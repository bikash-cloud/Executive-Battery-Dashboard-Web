// ===============================
// Demo Telemetry Data Generator
// Later this can be replaced with live ESP32 JSON data
// ===============================

function randomVoltage(min, max) {
  return (Math.random() * (max - min) + min).toFixed(2);
}

function generateDemoTelemetry() {
  const cell1 = parseFloat(randomVoltage(1.5, 2.5));
  const cell2 = parseFloat(randomVoltage(1.5, 2.5));
  const cell3 = parseFloat(randomVoltage(1.5, 2.5));
  const cell4 = parseFloat(randomVoltage(1.5, 2.5));

  const packVoltage = (cell1 + cell2 + cell3 + cell4).toFixed(2);
  const averageVoltage = (packVoltage / 4).toFixed(2);

  const maxCell = Math.max(cell1, cell2, cell3, cell4);
  const minCell = Math.min(cell1, cell2, cell3, cell4);

  const imbalance = (((maxCell - minCell) / averageVoltage) * 100).toFixed(2);
  const healthScore = Math.max(0, (100 - imbalance)).toFixed(1);

  let riskLevel = "LOW";
  let runtimeState = "NORMAL";
  let relayStatus = "ON";
  let buzzerStatus = "OFF";
  let lastFault = "No Fault";
  let recommendation = "Battery operating normally. No immediate action required.";

  if (imbalance > 20) {
    riskLevel = "HIGH";
    runtimeState = "FAULT";
    relayStatus = "OFF";
    buzzerStatus = "ON";
    lastFault = "High Cell Imbalance";
    recommendation = "Perform cell balancing and inspect weak battery cell.";
  }

  if (minCell < 1.2) {
    riskLevel = "CRITICAL";
    runtimeState = "FAULT";
    relayStatus = "OFF";
    buzzerStatus = "ON";
    lastFault = "Critical Low Cell Voltage";
    recommendation = "Stop system operation and replace weak cell immediately.";
  }

  return {
    cell1,
    cell2,
    cell3,
    cell4,
    packVoltage,
    averageVoltage,
    imbalance,
    healthScore,
    runtimeState,
    riskLevel,
    relayStatus,
    buzzerStatus,
    lastFault,
    recommendation
  };
}
