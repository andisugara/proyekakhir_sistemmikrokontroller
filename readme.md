# 🌱 Sistem Monitoring Nutrisi Tanaman (pH & EC)

## 📌 Informasi Umum

- **Mata Kuliah**: Sistem Mikrokontroler  
- **Kelas**: TIF RM 22 CID – Departemen Informatika, Universitas Teknologi Bandung  
- **Dosen Pengampu**: Muchamad Rusdan, S.T., M.T.  
- **Nama Mahasiswa**: Andi Sugara Putra  
- **NIM**: 22552011093  

---

## 📖 Penjelasan Proyek

Proyek ini merupakan implementasi sistem **monitoring nutrisi tanaman berbasis mikrokontroler ESP32 dan Internet of Things (IoT)**. Sistem ini dirancang untuk mengukur dua parameter penting dalam pertumbuhan tanaman hidroponik atau pertanian presisi, yaitu **pH** dan **Electrical Conductivity (EC)**.

Sistem akan secara otomatis membaca nilai pH dan EC, lalu:

- Menampilkan data secara *real-time* melalui dashboard **ThingSpeak**,
- Mengirim notifikasi ke **Telegram** jika nilai pH atau EC berada di luar batas normal,
- Mengaktifkan **relay** secara otomatis untuk simulasi pompa nutrisi jika diperlukan.

Sistem ini disimulasikan menggunakan platform **Wokwi** dan dapat dikembangkan lebih lanjut menggunakan sensor fisik dan kontrol aktuator nyata.

---

## 🧰 Komponen yang Digunakan

| Komponen        | Fungsi                                                                 |
|----------------|------------------------------------------------------------------------|
| **ESP32**       | Mikrokontroler utama, koneksi WiFi, pemrosesan data sensor             |
| **Breadboard**  | Media perakitan sirkuit elektronik                                      |
| **Potensiometer x2** | Simulasi sensor pH (A0) dan EC (A1) pada Wokwi/Tinkercad             |
| **DHT22**       | Mengukur suhu dan kelembaban lingkungan                                |
| **Relay Module**| Simulasi kontrol pompa/injeksi nutrisi otomatis berdasarkan nilai EC   |
| **Kabel Jumper**| Menghubungkan semua komponen pada breadboard                           |
| **ThingSpeak**  | Platform visualisasi data sensor IoT                                   |
| **Telegram Bot**| Memberikan notifikasi kondisi pH/EC ke pengguna                        |

---

## 🔗 Link Proyek

- 🎬 **YouTube (Demo Video)**: [https://youtu.be/eYkqEco4bU4](https://youtu.be/eYkqEco4bU4)  
- 🛠️ **Wokwi Simulation**: [https://wokwi.com/projects/435354038925716481](https://wokwi.com/projects/435354038925716481)  

---

## ✍️ Catatan

Proyek ini dibuat sebagai bagian dari evaluasi UAS pada mata kuliah Sistem Mikrokontroler.  
Semua sistem diuji secara simulatif dan dapat dikembangkan lebih lanjut dengan komponen fisik.

