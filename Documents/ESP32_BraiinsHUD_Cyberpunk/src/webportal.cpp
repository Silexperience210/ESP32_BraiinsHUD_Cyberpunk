#include "webportal.h"

WebPortal::WebPortal(SettingsManager* settings) : server(80) {
  settingsManager = settings;
  portalActive = false;
  configSaved = false;
}

void WebPortal::startPortal() {
  Serial.println("[PORTAL] Starting captive portal...");
  
  // Start AP mode
  WiFi.mode(WIFI_AP);
  WiFi.softAP(DEFAULT_AP_SSID, DEFAULT_AP_PASS);
  
  IPAddress apIP = WiFi.softAPIP();
  Serial.printf("[PORTAL] AP IP: %s\n", apIP.toString().c_str());
  Serial.printf("[PORTAL] SSID: %s\n", DEFAULT_AP_SSID);
  Serial.printf("[PORTAL] Pass: %s\n", DEFAULT_AP_PASS);
  
  // Start DNS server (captive portal redirect)
  dnsServer.start(53, "*", apIP);
  
  // Setup web server routes
  server.on("/", HTTP_GET, [this]() { handleRoot(); });
  server.on("/save", HTTP_POST, [this]() { handleSave(); });
  server.on("/scan", HTTP_GET, [this]() { handleScan(); });
  server.onNotFound([this]() { handleNotFound(); });
  
  server.begin();
  portalActive = true;
  configSaved = false;
  
  Serial.println("[PORTAL] Web server started!");
}

void WebPortal::stopPortal() {
  server.stop();
  dnsServer.stop();
  WiFi.softAPdisconnect(true);
  portalActive = false;
  Serial.println("[PORTAL] Portal stopped");
}

void WebPortal::handle() {
  if (portalActive) {
    dnsServer.processNextRequest();
    server.handleClient();
  }
}

void WebPortal::handleRoot() {
  server.send(200, "text/html", getConfigPage());
}

void WebPortal::handleScan() {
  String json = "[";
  int n = WiFi.scanNetworks();
  for (int i = 0; i < n; i++) {
    if (i > 0) json += ",";
    json += "{\"ssid\":\"" + WiFi.SSID(i) + "\",\"rssi\":" + String(WiFi.RSSI(i)) + "}";
  }
  json += "]";
  server.send(200, "application/json", json);
}

void WebPortal::handleSave() {
  String ssid = server.arg("ssid");
  String password = server.arg("password");
  String token = server.arg("token");
  String username = server.arg("username");
  String brightness = server.arg("brightness");
  String duration = server.arg("duration");
  
  Serial.println("[PORTAL] Saving configuration...");
  Serial.printf("[PORTAL] SSID: %s\n", ssid.c_str());
  Serial.printf("[PORTAL] Token: %s***\n", token.substring(0, 4).c_str());
  
  // Validate required fields
  if (ssid.length() == 0 || token.length() == 0) {
    server.send(400, "text/html", "<html><body><h1>Error: SSID and Token are required!</h1><a href='/'>Back</a></body></html>");
    return;
  }
  
  // Save settings
  settingsManager->setWifiSSID(ssid.c_str());
  settingsManager->setWifiPassword(password.c_str());
  settingsManager->setBraiinsToken(token.c_str());
  settingsManager->setBraiinsUsername(username.length() > 0 ? username.c_str() : "Miner");
  settingsManager->setBrightness(brightness.length() > 0 ? brightness.toInt() : DEFAULT_BRIGHTNESS);
  settingsManager->setScreenDuration(duration.length() > 0 ? duration.toInt() : DEFAULT_SCREEN_DURATION);
  settingsManager->setConfigured(true);
  settingsManager->save();
  
  configSaved = true;
  
  server.send(200, "text/html", getSuccessPage());
}

void WebPortal::handleNotFound() {
  // Redirect all requests to root (captive portal behavior)
  server.sendHeader("Location", "/", true);
  server.send(302, "text/plain", "");
}

String WebPortal::getConfigPage() {
  String html = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>Braiins HUD Setup</title>
  <style>
    * { box-sizing: border-box; font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, sans-serif; }
    body { background: #1a1a2e; color: #eee; margin: 0; padding: 20px; min-height: 100vh; }
    .container { max-width: 400px; margin: 0 auto; }
    h1 { color: #ff6600; text-align: center; margin-bottom: 30px; }
    h2 { color: #00ffff; font-size: 14px; margin-top: 25px; margin-bottom: 10px; text-transform: uppercase; }
    .logo { text-align: center; font-size: 48px; margin-bottom: 10px; }
    label { display: block; margin-bottom: 5px; color: #888; font-size: 12px; text-transform: uppercase; }
    input, select { width: 100%; padding: 12px; margin-bottom: 15px; border: 1px solid #333; border-radius: 8px; 
                    background: #16213e; color: #fff; font-size: 16px; }
    input:focus { outline: none; border-color: #ff6600; }
    button { width: 100%; padding: 15px; background: #ff6600; color: #fff; border: none; border-radius: 8px; 
             font-size: 16px; font-weight: bold; cursor: pointer; margin-top: 10px; }
    button:hover { background: #ff8833; }
    button.scan { background: #16213e; border: 1px solid #00ffff; color: #00ffff; margin-bottom: 15px; }
    .networks { max-height: 150px; overflow-y: auto; margin-bottom: 15px; }
    .network { padding: 10px; background: #16213e; border-radius: 4px; margin-bottom: 5px; cursor: pointer; }
    .network:hover { background: #1f3460; }
    .signal { float: right; color: #888; }
    .required { color: #ff6600; }
    .optional { color: #666; font-size: 10px; }
    .info { background: #16213e; padding: 15px; border-radius: 8px; margin-bottom: 20px; font-size: 12px; line-height: 1.6; }
    .info a { color: #00ffff; }
    input[type="range"] { padding: 0; }
    .range-value { text-align: center; color: #ff6600; font-weight: bold; }
  </style>
</head>
<body>
  <div class="container">
    <div class="logo">⛏️</div>
    <h1>Braiins HUD Setup</h1>
    
    <div class="info">
      <strong>Welcome!</strong> Configure your mining dashboard below.<br>
      Get your API token from <a href="https://pool.braiins.com/settings/access" target="_blank">pool.braiins.com</a>
    </div>
    
    <form action="/save" method="POST">
      <h2>📶 WiFi Settings</h2>
      <button type="button" class="scan" onclick="scanNetworks()">Scan for Networks</button>
      <div id="networks" class="networks"></div>
      
      <label>WiFi Network <span class="required">*</span></label>
      <input type="text" name="ssid" id="ssid" required placeholder="Your WiFi name">
      
      <label>WiFi Password</label>
      <input type="password" name="password" id="password" placeholder="WiFi password">
      
      <h2>🔑 Braiins Pool</h2>
      <label>API Token <span class="required">*</span></label>
      <input type="text" name="token" required placeholder="Your Braiins API token">
      
      <label>Username <span class="optional">(optional - for display)</span></label>
      <input type="text" name="username" placeholder="Your mining name">
      
      <h2>⚙️ Display Settings</h2>
      <label>Brightness</label>
      <input type="range" name="brightness" min="10" max="255" value="255" oninput="updateBrightness(this.value)">
      <div class="range-value" id="brightnessVal">100%</div>
      
      <label>Screen Rotation (seconds)</label>
      <input type="number" name="duration" min="5" max="300" value="30" placeholder="30">
      
      <button type="submit">💾 Save & Connect</button>
    </form>
  </div>
  
  <script>
    function scanNetworks() {
      document.getElementById('networks').innerHTML = '<div class="network">Scanning...</div>';
      fetch('/scan')
        .then(r => r.json())
        .then(networks => {
          let html = '';
          networks.forEach(n => {
            let signal = n.rssi > -50 ? '📶' : (n.rssi > -70 ? '📶' : '📶');
            html += '<div class="network" onclick="selectNetwork(\'' + n.ssid + '\')">' + 
                    n.ssid + '<span class="signal">' + n.rssi + ' dBm</span></div>';
          });
          document.getElementById('networks').innerHTML = html || '<div class="network">No networks found</div>';
        })
        .catch(() => {
          document.getElementById('networks').innerHTML = '<div class="network">Scan failed</div>';
        });
    }
    
    function selectNetwork(ssid) {
      document.getElementById('ssid').value = ssid;
    }
    
    function updateBrightness(val) {
      document.getElementById('brightnessVal').innerText = Math.round(val / 255 * 100) + '%';
    }
  </script>
</body>
</html>
)rawliteral";
  return html;
}

String WebPortal::getSuccessPage() {
  String html = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>Setup Complete!</title>
  <style>
    body { background: #1a1a2e; color: #eee; font-family: -apple-system, sans-serif; 
           display: flex; align-items: center; justify-content: center; min-height: 100vh; margin: 0; }
    .container { text-align: center; padding: 40px; }
    .icon { font-size: 64px; margin-bottom: 20px; }
    h1 { color: #00ff00; }
    p { color: #888; margin-top: 20px; }
  </style>
</head>
<body>
  <div class="container">
    <div class="icon">✅</div>
    <h1>Configuration Saved!</h1>
    <p>Your Braiins HUD will restart in a few seconds...</p>
    <p>Connecting to your WiFi network...</p>
  </div>
</body>
</html>
)rawliteral";
  return html;
}
