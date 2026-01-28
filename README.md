
# ⚡ Electricity Theft & Overcurrent Detection System using ESP32 and GSM

This project implements a **smart electricity monitoring and protection system** using an **ESP32**, **ACS712 current sensor**, **relay**, **SIM800L GSM module**, and a **16×2 I²C LCD**.
The system continuously measures RMS current, detects **overcurrent or electricity theft**, automatically **cuts off the supply**, and **sends an SMS alert** to the user.

---

## 📌 Key Features

* ✅ Accurate **RMS current measurement**
* 🔍 **Electricity theft / overcurrent detection**
* 🔌 **Automatic power cutoff** using relay
* 📩 **SMS alert** via GSM (SIM800L)
* 📟 **16×2 I²C LCD** for real-time monitoring
* 🧠 Auto **zero-current calibration**
* 🔁 Prevents repeated SMS alerts for same fault
* 🛠 Modular and easy-to-modify code

---

## ⚙️ Working Principle

1. ACS712 measures load current continuously
2. ESP32 calculates **RMS current**
3. If current exceeds the **set threshold (2A)**:

   * Relay trips → supply cut off
   * SMS alert is sent via GSM
   * LCD displays overcurrent warning
4. When current returns to safe range:

   * Relay turns ON again
   * System resumes normal monitoring

---

## 🧩 Hardware Components

| Component             | Description                     |
| --------------------- | ------------------------------- |
| ESP32                 | Main controller                 |
| ACS712                | Current sensor (5A / 20A / 30A) |
| SIM800L               | GSM module for SMS              |
| Relay Module          | Load cutoff                     |
| 16×2 I²C LCD          | Display                         |
| Resistors             | Voltage divider                 |
| External Power Supply | 4.0–4.2V for GSM                |

---

## 🔌 Pin Configuration

### ACS712

| Signal | ESP32 Pin |
| ------ | --------- |
| OUT    | GPIO 34   |
| VCC    | 5V        |
| GND    | GND       |

### Relay

| Signal | ESP32 Pin |
| ------ | --------- |
| IN     | GPIO 27   |
| VCC    | 5V        |
| GND    | GND       |

### GSM (SIM800L)

| SIM800L | ESP32               |
| ------- | ------------------- |
| TX      | GPIO 16             |
| RX      | GPIO 17             |
| VCC     | 4.0–4.2V (External) |
| GND     | GND                 |

⚠️ **Do NOT power SIM800L from ESP32 3.3V**

### LCD (I²C)

| LCD | ESP32   |
| --- | ------- |
| SDA | GPIO 21 |
| SCL | GPIO 22 |
| VCC | 5V      |
| GND | GND     |

---

## 📟 LCD Display Output

**Normal Mode**

```
Current: 1.25A
Relay: ON
```

**Overcurrent Mode**

```
Overcurrent!
2.45A Relay OFF
```

---

## 📩 SMS Alert Example

```
⚠️ Overcurrent detected!
Supply cutoff activated.
```

---

## 🧪 Software Details

* Platform: **ESP32 (Arduino IDE)**
* ADC Resolution: **12-bit**
* Sampling Window: **1000 ms**
* Threshold Current: **2.0 A**
* LCD Library: `LiquidCrystal_I2C`
* Communication:

  * Serial Monitor → Debug
  * Serial2 → GSM

---

## 🧠 Applications

* ⚡ Electricity theft detection
* 🏠 Smart energy monitoring
* 🏭 Industrial load protection
* 🔌 Overcurrent safety system
* 🎓 Engineering & academic projects

---

## 🔒 Safety Notes

* Use **proper isolation** for mains voltage
* Use **rated relays** for AC loads
* Ensure **common ground**
* Provide **stable power** to GSM module

---

## 🚀 Future Improvements

* Remote relay ON/OFF via SMS
* Energy consumption (kWh) calculation
* IoT cloud dashboard
* Mobile app integration
* Multi-phase monitoring

---

## 👨‍💻 Author

**Bhushan Patil**
Electronics & Telecommunication Engineering
Core Comet Industries 🚀

---

