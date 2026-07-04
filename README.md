# 🔋 Executive Battery Dashboard

An enterprise-grade Battery Management System (BMS) dashboard built using **ESP32**, **Firebase Realtime Database**, **Chart.js**, and a modern web interface.

The dashboard monitors four battery cells in real time, performs battery health analysis, detects abnormal operating conditions, uploads telemetry to the cloud, and visualizes live battery analytics.

---

## 🚀 Live Demo

GitHub Pages:
https://bikash-cloud.github.io/Executive-Battery-Dashboard-Web/

Wokwi Simulation:
https://wokwi.com/projects/468549438125530113

---

# Features

- Real-time 4-cell voltage monitoring
- Battery Health Score calculation
- Pack Voltage calculation
- Average Cell Voltage
- Cell Imbalance Detection
- Runtime State Detection
- Risk Level Analysis
- Relay Control
- Buzzer Alarm
- Firebase Realtime Database Integration
- Historical Voltage Trend Graph
- Executive Analytics Dashboard
- Responsive UI
- Live Cloud Telemetry

---

# Hardware

- ESP32 DevKit V1
- 16x2 I2C LCD
- Relay Module
- Buzzer
- 4 Analog Voltage Inputs
- WiFi Connectivity

---

# Software Stack

- Arduino IDE
- ESP32
- Firebase Realtime Database
- HTML
- CSS
- JavaScript
- Chart.js
- GitHub Pages

---

# Project Structure

```
Executive-Battery-Dashboard-Web
│
├── index.html
├── style.css
├── script.js
├── firebase.js
├── charts.js
├── data.js
└── README.md
```

---

# Dashboard Preview

The Executive Dashboard displays:

- Battery Health
- Pack Voltage
- Average Voltage
- Risk Level
- Cell Voltages
- Diagnostics
- Fault History
- Historical Voltage Trend
- Operator Recommendation

---

# Firebase Data

Example JSON

```json
{
  "cell1": 2.91,
  "cell2": 0.79,
  "cell3": 2.46,
  "cell4": 1.88,
  "packVoltage": 8.04,
  "averageVoltage": 2.01,
  "healthScore": 0,
  "imbalance": 105.25,
  "runtimeState": "FAULT",
  "riskLevel": "HIGH"
}
```

---

# How to Run

1. Clone the repository

```
git clone https://github.com/bikash-cloud/Executive-Battery-Dashboard-Web.git
```

2. Open the project in VS Code.

3. Start Live Server.

4. Ensure Firebase Realtime Database is configured.

5. Power on the ESP32.

6. View the live dashboard.

---

# Future Improvements

- Battery Remaining Time Prediction
- AI Fault Prediction
- Cloud Data Analytics
- Email Alerts
- Mobile App
- Battery Temperature Monitoring
- MQTT Support
- OTA Firmware Updates

---

# Author

**Bikash Kumar Swain**

Electronics & Communication Engineering

Embedded Systems | IoT | AI | ESP32 | STM32 | Firebase

GitHub:
https://github.com/bikash-cloud

---

## License

This project is released under the MIT License.
