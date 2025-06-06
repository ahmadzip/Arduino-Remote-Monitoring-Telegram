# IOT MONITORING SUHU & KELEMBABAN DAN CONTROL LED DENGAN TELEGRAM BOT

GITHUB : https://github.com/ahmadzip/Arduino-Remote-Monitoring-Telegram

**NPM**: 5220411084  
**Nama**: Ahmas Sulaeman

---

## 📋 Overview Proyek

Proyek ini bertujuan untuk membuat sistem monitoring suhu dan kelembaban menggunakan sensor DHT22 yang terhubung dengan ESP32. Data yang diperoleh akan dikirimkan ke Telegram Bot, memungkinkan pengguna untuk memantau kondisi lingkungan secara real-time. Selain itu, proyek ini juga menyediakan kontrol LED sebagai simulasi pengendalian perangkat jarak jauh.

- **Sensor DHT22**
- **ESP32** (Modul WiFi)
- **LED**
- **TELEGRAM BOT**

---

## 🛠 RANGKAIAN

![Diagram Rangkaian](https://i.imgur.com/9p6l2Ae.png)
**Komponen**:

- ESP32 → Mikrokontroler utama
- DHT22 → Sensor suhu & kelembaban
- LED → Sebagai ganti dari lampu 220v

**penjelasan**:

- **ESP32**: Modul WiFi yang menghubungkan sensor ke internet.
- **DHT22**: Sensor yang mengukur suhu dan kelembaban.
- **LED**: Digunakan untuk simulasi kontrol perangkat, sebagai pengganti lampu 220V.
- **Telegram Bot**: Untuk kontrol dan monitoring jarak jauh.

## 📦 Perbandingan Fitur

| **Fitur**                    | **Kode 1 (Dari Gambar)**                       | **Kode 2 (Teks Diberikan)**                                                                                                      |
| ---------------------------- | ---------------------------------------------- | -------------------------------------------------------------------------------------------------------------------------------- |
| **PIN DHT Sensor**           | `const int DHT_PIN = 25;`                      | `const int DHT_PIN = 15;`                                                                                                        |
| **Kontrol LED**              | Tidak ada                                      | Ada, `const int LED_PIN = 2;`, `bool ledState = LOW;`                                                                            |
| **Token & Chat ID Bot**      | `BOTtoken` dan `CHAT_ID` kosong (perlu diisi)  | `BOTtoken` dan `CHAT_ID` sudah terisi (contoh)                                                                                   |
| **Pembacaan Sensor**         | Fungsi `getReadings()` sederhana               | Fungsi `getReadings()` dengan pengecekan `isnan` untuk error pembacaan                                                           |
| **Format Pesan Status**      | Hanya suhu dan kelembapan                      | Fungsi `getFormattedStatus()`: suhu, kelembapan, status lampu, status pengiriman data, dan info NPM/Nama                         |
| **Interaksi Telegram**       | Hanya mengirim data sensor secara periodik     | Menggunakan Inline Keyboard (`keyboardJson`) untuk interaksi: menyalakan/mematikan lampu, memulai/menghentikan pengiriman status |
| **Penanganan Pesan Masuk**   | Tidak ada penanganan pesan masuk dari pengguna | Fungsi `handleNewMessages()` untuk memproses perintah dari pengguna melalui teks dan callback query                              |
| **Pengiriman Data Otomatis** | Mengirim data setiap `delay(6000);` (6 detik)  | Pengiriman data otomatis dapat diaktifkan/dinonaktifkan (`sendDataEnabled`), interval `sendInterval = 5000;` (5 detik)           |
| **Struktur `loop()`**        | Sederhana: baca sensor, kirim pesan, delay     | Lebih kompleks: cek pesan baru dari bot, kirim data otomatis jika diaktifkan                                                     |
| **Variabel Tambahan**        | Minimal                                        | `botRequestDelay`, `lastTimeBotRan`, `sendDataEnabled`, `sendInterval`, `lastSendTime`, `npm_nama`, `keyboardJson`               |
| **Fungsi Tambahan**          | Hanya `getReadings()`                          | `getFormattedStatus()`, `sendStatusWithKeyboard()`, `handleNewMessages()`                                                        |
| **Inisialisasi `setup()`**   | Inisialisasi dasar WiFi dan DHT                | Inisialisasi WiFi, DHT, dan pin LED. Pesan serial lebih detail.                                                                  |
| **Keamanan Chat ID**         | Tidak ada pemeriksaan `CHAT_ID`                | Memeriksa `CHAT_ID` pada `handleNewMessages()`, menolak pesan dari ID yang tidak diizinkan                                       |
| **Feedback ke Pengguna**     | Tidak ada feedback untuk aksi                  | Memberikan feedback (`action_message`) setelah aksi callback query diproses                                                      |

## 📊 Hasil Pengujian

![Hasil Pengujian 1](https://i.imgur.com/QmN9EwC.png)
Ketika simulator di jalankan, belum terjadi apa apa karna pada bot utama belum di berikan perintah start

![Hasil Pengujian 2](https://i.imgur.com/OnWdmrL.png)
Setelah di start, maka akan muncul pesan yang berisi button

- tombol untuk menghidupkan/mematikan lampu, dan tombol untuk mengirim status sensor
- tombol untuk mengirim informasi tentang npm dan nama pembuat

![Hasil Pengujian 3](https://i.imgur.com/rU99t3I.png)
Ketika kita menekan tombol "Menyalakan Lampu", maka lampu akan menyala, begitu juga sebaliknya ketika menekan tombol "Mematikan Lampu"

![Hasil Pengujian 4](https://i.imgur.com/6Yaaz3Z.png)
Ketika kita menekan tombol "Start Mengirim Status", maka bot akan mengirimkan status sensor secara otomatis setiap 5 detik terus menerus sampai kita menekan tombol "Stop Status"

## 📊 Analisis

Proyek ini menunjukkan integrasi yang baik antara sensor, mikrokontroler, dan platform komunikasi. Beberapa poin penting:

- **Interaktivitas**: Pengguna dapat mengontrol perangkat (LED) dan mendapatkan informasi sensor secara real-time melalui Telegram.
- **Stabilitas**: Penanganan error pada pembacaan sensor memastikan sistem tetap berjalan meskipun ada masalah dengan sensor.
- **Fleksibilitas**: Pengguna dapat mengaktifkan atau menonaktifkan pengiriman data otomatis, memberikan kontrol lebih besar atas penggunaan data dan sumber daya.

## 📜 Kesimpulan

Proyek ini berhasil mengimplementasikan sistem monitoring suhu dan kelembaban yang interaktif dan responsif. Dengan menggunakan Telegram Bot, pengguna dapat dengan mudah mengontrol perangkat dan mendapatkan informasi penting secara real-time. Sistem ini juga menunjukkan bagaimana IoT dapat digunakan untuk aplikasi praktis dalam kehidupan sehari-hari.
