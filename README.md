# 🕸️ LoRa Mesh Network for FOV Drones  
**STM32F103 • LoRa RA-02 • FreeRTOS • Ad-Hoc Routing**

This repository contains a lightweight **mesh networking implementation** for FOV drones using **LoRa RA-02** radios and an **STM32F103** microcontroller.  
The design focuses on **ad-hoc routing**, **reliability**, and **deterministic FreeRTOS task structure**.

---

## 🚀 Features

### 🔄 Ad-Hoc Mesh Routing
Fully decentralized routing.  
Nodes discover neighbors dynamically and forward packets without a root or coordinator.

### 📡 Long-Range LoRa Communication
Utilizes the SX1278 (RA-02) module for long-range, low-bandwidth communication ideal for drones and sensor networks.

### 🧵 FreeRTOS Task Architecture

The communication pipeline:

xTX_task  →  txQueue  →  router_task  →  rxQueue  →  xRX_task

Supporting tasks:

- **ping_task** – Periodically sends PING frames to discover live nodes  
- **packet_loss_task** – Tracks unacknowledged packets and schedules retransmissions  

### 🧬 Reliability & Deduplication
- ACK per message  
- Retransmissions via `packet_loss_task`  
- Duplicate filtering using `(dst_id ^ msg_id)`  
- Optional packet compression layer  

---
---

## 🧠 Mesh Logic Overview

### Packet Routing Flow
1. If packet **destination == local_id** → deliver to application  
2. If packet is **not for me** and **TTL > 0** → forward  
3. Store `(src_id, msg_id)` to detect duplicates  
4. Update neighbor table when hearing packets  

### PING System
- Nodes broadcast PING every `PING_INTERVAL_MS`  
- Nodes reply with PONG  
- Builds a list of currently reachable nodes  

### Packet Loss Handler
A packet is added to a retry buffer when TX doesn't receive an ACK:

- Timeout triggers retry  
- Retries limited by `MAX_RETRIES`  
- If still no ACK → mark as dropped  

---

## 🛠️ Hardware

- **STM32F103C8T6** or compatible F1 MCU  
- **LoRa RA-02 (SX1278)** module  
- SPI + NSS + DIO0 wiring  
- Optional drone FC integration  

---

## ▶️ Getting Started

1. Clone the repository  
2. Open in **STM32CubeIDE** or **CLion (ARM toolchain)**  
3. Configure the SPI pins to match your LoRa RA-02 wiring  
4. Build and flash  
5. Power 2 or more nodes → mesh forms automatically  

---

## 📡 Example Task Diagram

     ┌──────────┐          ┌───────────────┐
     │ xTX_task │ ───────► │   txQueue     │
     └──────────┘          └───────────────┘
                                │
                                ▼
                         ┌────────────┐
                         │ router_task│
                         └────────────┘
                                │
                                ▼
     ┌──────────┐          ┌───────────────┐
     │ xRX_task │ ◄─────── │   rxQueue     │
     └──────────┘          └───────────────┘

     ┌──────────────┐    ┌────────────────────┐
     │  ping_task   │    │ packet_loss_task   │
     └──────────────┘    └────────────────────┘


     
