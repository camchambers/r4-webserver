#ifndef MATRIX_PAGE_H
#define MATRIX_PAGE_H

const char MATRIX_PAGE[] PROGMEM = R"rawliteral(
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
      padding: 20px; 
    }
    h1 { margin-bottom: 30px; }
    .matrix-container {
      display: inline-block;
      background: #111;
      padding: 20px;
      border-radius: 10px;
      box-shadow: 0 4px 20px rgba(0,0,0,0.5);
    }
    .matrix-grid {
      display: grid;
      grid-template-columns: repeat(12, 30px);
      gap: 5px;
    }
    .led {
      width: 30px;
      height: 30px;
      background: #333;
      border: 1px solid #555;
      border-radius: 4px;
      cursor: pointer;
      transition: all 0.2s;
    }
    .led:hover {
      border-color: #00979d;
    }
    .led.on {
      background: #00979d;
      box-shadow: 0 0 10px #00979d;
    }
    .controls {
      margin-top: 20px;
    }
    button {
      padding: 10px 20px;
      font-size: 1rem;
      background: #00979d;
      color: white;
      border: none;
      border-radius: 5px;
      cursor: pointer;
      margin: 5px;
    }
    button:hover {
      background: #007d82;
    }
    .back-link {
      display: block;
      margin-top: 20px;
      color: #00979d;
      text-decoration: none;
    }
  </style>
  <script>
    let matrix = Array(8).fill(null).map(() => Array(12).fill(0));
    
    function toggleLED(row, col) {
      matrix[row][col] = matrix[row][col] === 1 ? 0 : 1;
      const led = document.getElementById('led_' + row + '_' + col);
      led.classList.toggle('on');
      sendMatrix();
    }
    
    function clearAll() {
      matrix = Array(8).fill(null).map(() => Array(12).fill(0));
      document.querySelectorAll('.led').forEach(led => led.classList.remove('on'));
      sendMatrix();
    }
    
    function fillAll() {
      matrix = Array(8).fill(null).map(() => Array(12).fill(1));
      document.querySelectorAll('.led').forEach(led => led.classList.add('on'));
      sendMatrix();
    }
    
    function sendMatrix() {
      const matrixStr = matrix.map(row => row.join('')).join(',');
      fetch('/matrix?data=' + encodeURIComponent(matrixStr))
        .catch(err => console.error('Error:', err));
    }
  </script>
</head>
<body>
  <h1>R4 LED Matrix Control</h1>
  <div class="matrix-container">
    <div class="matrix-grid" id="matrixGrid"></div>
  </div>
  <div class="controls">
    <button onclick="fillAll()">Fill All</button>
    <button onclick="clearAll()">Clear All</button>
  </div>
  
  <script>
    // Generate the matrix grid
    const grid = document.getElementById('matrixGrid');
    for (let row = 0; row < 8; row++) {
      for (let col = 0; col < 12; col++) {
        const led = document.createElement('div');
        led.className = 'led';
        led.id = 'led_' + row + '_' + col;
        led.onclick = () => toggleLED(row, col);
        grid.appendChild(led);
      }
    }
  </script>
</body>
</html>
)rawliteral";

#endif
