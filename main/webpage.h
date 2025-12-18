#ifndef WEBPAGE_H
#define WEBPAGE_H

const char WEBPAGE[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>
<head>
  <meta name='viewport' content='width=device-width, initial-scale=1'>
  <style>
    body { 
      font-family: sans-serif; 
      text-align: center; 
      background-color: #222; 
      color: #fff; 
      padding: 50px; 
    }
    input { 
      padding: 10px; 
      font-size: 1.2rem; 
      width: 80%; 
      border-radius: 5px; 
      border: none; 
    }
    button { 
      padding: 10px 20px; 
      font-size: 1.2rem; 
      background: #00979d; 
      color: white; 
      border: none; 
      border-radius: 5px; 
      cursor: pointer; 
      margin-top: 10px; 
    }
  </style>
</head>
<body>
  <h1>R4 WiFi Matrix</h1>
  <form action='/' method='GET'>
    <input type='text' name='msg' placeholder='Type a message...' maxlength='50'>
    <br><button type='submit'>Send to Matrix</button>
  </form>
  <br>
  <a href='/matrix' style='color: #00979d; text-decoration: none;'>→ LED Matrix Control</a>
</body>
</html>
)rawliteral";

#endif
