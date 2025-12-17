# Arduino R4 WiFi Matrix Web Server

A web server for the Arduino UNO R4 WiFi that displays messages on the onboard LED matrix through a web interface.

## Requirements

- Arduino UNO R4 WiFi board
- USB cable

## Setup

### 1. Configure WiFi

Edit `main/arduino_secrets.h` with your WiFi credentials:

```cpp
#define SECRET_SSID "your-wifi-ssid"
#define SECRET_PASS "your-wifi-password"
```

### 2. Upload

1. Open `main/main.ino` in Arduino IDE
2. Select Arduino UNO R4 WiFi board
3. Upload to your board

### 3. Find IP Address

Open Serial Monitor (115200 baud) to see the IP address

## Usage

1. Open a web browser and navigate to the Arduino's IP address (e.g., `http://10.0.0.123`)
2. Enter your message in the text field (max 50 characters)
3. Click "Send to Matrix"
4. Watch your message scroll across the onbard LED matrix

## How It Works

- The Arduino connects to your WiFi network and starts a web server on port 80
- The LED matrix continuously scrolls the current message
- When you submit a message through the web form, it sends a GET request with the message as a URL parameter (`?msg=Hello`)
- The Arduino parses the request and updates the scrolling message
## Usage

1. Navigate to the Arduino's IP address in a web browser
2. Enter a message and click "Send to Matrix"
3. The message will scroll on the LED matrix

## Customization

Adjust scroll speed in `setup()`:

```cpp
matrix.textScrollSpeed(100); // Lower = faster
```

Change the default message:

```cpp
String currentMessage = " Your Message Here! ";
```