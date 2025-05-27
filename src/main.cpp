#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <DHTesp.h>

DHTesp dhtSensor;
TempAndHumidity data;
const int DHT_PIN = 15;

const int LED_PIN = 2;
bool ledState = LOW;

#define WIFI_SSID "Wokwi-GUEST"
#define WIFI_PASSWORD ""
#define BOTtoken "7842057993:AAE6x7EAfXLGRtADqiDRl3OGees8lynXh3o"
#define CHAT_ID "1823573625"

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

int botRequestDelay = 1000; 
unsigned long lastTimeBotRan;
bool sendDataEnabled = false; 
const unsigned long sendInterval = 5000; 
unsigned long lastSendTime = 0;

String npm_nama = "\n\n---\n5220411084\nAhmad Sulaeman";
String keyboardJson = "[[{ \"text\" : \"✅ Nyalakan Lampu\", \"callback_data\" : \"/nyalakan\" }, { \"text\" : \"❌ Matikan Lampu\", \"callback_data\" : \"/matikan\" }], "
                    "[{ \"text\" : \"▶️ Start Status\", \"callback_data\" : \"/start_send\" }, { \"text\" : \"⏹️ Stop Status\", \"callback_data\" : \"/stop_send\" }]]";


String getReadings() {
  data = dhtSensor.getTempAndHumidity();
  if (isnan(data.temperature) || isnan(data.humidity)) {
      return "Gagal membaca sensor DHT!\n";
  }
  String message = "Suhu: " + String(data.temperature, 1) + " °C \n";
  message += "Kelembapan: " + String(data.humidity, 1) + " %\n";
  return message;
}

String getFormattedStatus() {
    String message = getReadings();
    message += "Status Lampu: " + String(ledState ? "NYALA" : "MATI");
    message += "\nPengiriman Data: " + String(sendDataEnabled ? "AKTIF" : "NONAKTIF");
    message += npm_nama;
    return message;
}

void sendStatusWithKeyboard(String chat_id, String from_name, String title = "") {
    String message;
    if (title == "") {
      message = "Selamat datang, " + from_name + ".\n";
      message += "Silakan gunakan tombol di bawah:\n";
    } else {
      message = title + "\n";
    }
    message += getFormattedStatus();
    bot.sendMessageWithInlineKeyboard(chat_id, message, "", keyboardJson);
}


void handleNewMessages(int numNewMessages) {
  for (int i = 0; i < numNewMessages; i++) {
    String chat_id = String(bot.messages[i].chat_id);
    String query_id = bot.messages[i].query_id;
    String text = bot.messages[i].text;
    String from_name = bot.messages[i].from_name;
    String user_id = String(bot.messages[i].from_id); 
    if (from_name == "") from_name = bot.messages[i].from_name;
    if (chat_id != CHAT_ID) {
      bot.sendMessage(chat_id, "Anda tidak diizinkan.", "");
      continue;
    }
    if (bot.messages[i].type == "callback_query") {
        Serial.println("Callback Diterima: " + text);
        String action_message = "";

        if (text == "/nyalakan") {
            ledState = HIGH;
            digitalWrite(LED_PIN, ledState);
            action_message = "Lampu dinyalakan.";
        }
        else if (text == "/matikan") {
            ledState = LOW;
            digitalWrite(LED_PIN, ledState);
            action_message = "Lampu dimatikan.";
        }
        else if (text == "/start_send") {
            sendDataEnabled = true;
            lastSendTime = millis(); 
            action_message = "Pengiriman status dimulai.";
        }
        else if (text == "/stop_send") {
            sendDataEnabled = false;
            action_message = "Pengiriman status dihentikan.";
        }
        
        bot.answerCallbackQuery(query_id, action_message, false); 
        sendStatusWithKeyboard(chat_id, from_name, action_message); 
    }
    else if (bot.messages[i].type == "message") {
        Serial.println("Pesan Teks Diterima: " + text);
        if (text == "/start") {
            sendStatusWithKeyboard(chat_id, from_name);
        }
        else {
            bot.sendMessage(chat_id, "Ketik /kontrol untuk menampilkan pilihan.", "");
        }
    }
  }
}

void setup(void) {
  Serial.begin(115200);
  Serial.println("Memulai Setup...");

  dhtSensor.setup(DHT_PIN, DHTesp::DHT22);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, ledState);

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Menghubungkan ke WiFi ");
  Serial.print(WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nTerhubung!");
  Serial.print("Alamat IP: ");
  Serial.println(WiFi.localIP());

#ifdef ESP32
  client.setCACert(TELEGRAM_CERTIFICATE_ROOT);
#endif

  Serial.println("Setup Selesai. Ketik /start di Telegram.");
}

void loop() {
  if (millis() > lastTimeBotRan + botRequestDelay) {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    while (numNewMessages) {
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }

  if (sendDataEnabled && (millis() > lastSendTime + sendInterval)) {
    String messageToSend = "Update Otomatis:\n" + getFormattedStatus();
    bot.sendMessageWithInlineKeyboard(CHAT_ID, messageToSend, "", keyboardJson); 
    lastSendTime = millis(); 
  }
}