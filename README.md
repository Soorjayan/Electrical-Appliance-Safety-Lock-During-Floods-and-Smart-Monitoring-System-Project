# 🌐 Smart Monitoring System (Water Level + Soil Moisture)

## 📌 Project Overview

This project is a **Smart Monitoring System** designed using **ATmega328P and ESP32** to monitor water levels and soil moisture conditions in real time.

It provides a **web-based dashboard** that displays sensor data, status indicators, and allows remote control of a relay system.

---

## 🚀 Features

### 💧 Water Level Monitoring

* LOW
* MEDIUM
* HIGH / DANGER

### 🌱 Soil Moisture Monitoring

* DRY
* NORMAL
* WET

### ⚡ Power Control

* Automatic ON/OFF based on sensor conditions
* Manual control from web interface

### 📊 IoT Dashboard

* Real-time sensor values
* Live graph visualization
* Mobile-friendly interface

---

## 🧠 System Architecture

ATmega328P:

* Reads sensors (Water + Moisture)
* Controls LEDs and relay
* Sends data via UART

ESP32:

* Receives data from ATmega
* Hosts web server
* Displays data and graph
* Allows remote control

---

## 🔌 Hardware Components

* ATmega328P
* ESP32
* Water Level Sensor
* Soil Moisture Sensor
* Relay Module
* LEDs
* 16 MHz Crystal
* Resistors & Capacitors

---

## 💻 Software & Tools

* AVR-GCC (Embedded C)
* Arduino IDE (ESP32)
* HTML + JavaScript (Chart.js)
* UART Communication

---

## 📡 Data Format

ATmega sends data to ESP32:

```
W:650,M:480
```

---

## 🌐 Web Interface

Displays:

* Water Level + Status
* Moisture Level + Status
* Power Status
* Live Graph

---

## 📊 Live Features

* Real-time updates every 2 seconds
* Graph plotting (Chart.js)
* Color indication:

  * 🟢 SAFE
  * 🔴 DANGER

---

## 🎛 Control

* Turn relay ON/OFF from browser
* Works on mobile & laptop

---

## 👥 Group Details

Group Number: Group 08

Members:


2022/E/035 	Naveenkumar E.
2022/E/136  Ketharan S.
2022/E/152 	Vithushan S.
2022/E/164 	Sahithyan U.
2022/E/165 	Soorjayan V.

---

## 📅 Submission

Course: Embedded Systems / Electronic Circuit and Devices
University: University of Jaffna

---

## 📌 Conclusion

This project demonstrates how embedded systems and IoT can be combined to build a smart monitoring and control system with real-time visualization.

---
