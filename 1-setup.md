# 1 — Setup

Memasang alat kerja STM32 di Windows dan Linux.

Kebutuhan disk: **± 5 GB**. Waktu: **30–60 menit** (sebagian besar mengunduh).

---

## Alat yang dipasang

| Alat | Fungsi |
|---|---|
| STM32CubeMX | Mengatur pin & clock, menghasilkan kerangka proyek |
| STM32CubeIDE | Menulis kode, compile, flash |
| STM32Cube Firmware | Pustaka HAL untuk seri chip yang dipakai |
| Terminal serial | Membaca teks dari chip (dipakai di tutorial 3) |

Semua gratis. Perlu akun ST (bisa daftar saat mengunduh, atau pakai *Download as guest*).

---

##  CubeIDE 2.x tidak memuat CubeMX

**CubeIDE 2.0.0**, CubeMX dipisah jadi program berdiri sendiri.


| | CubeIDE 1.x | CubeIDE 2.x |
|---|---|---|
| CubeMX | menyatu | terpisah |
| Buat proyek | dari dalam CubeIDE | dari CubeMX |


---

## Langkah 1 — Pasang CubeIDE

Unduh: <https://www.st.com/en/development-tools/stm32cubeide.html>

### Windows

1. Unduh installer `.exe`
2. Jalankan, terima semua pilihan bawaan
3. Saat diminta memasang driver ST-LINK → **izinkan**

### Linux

```bash
cd ~/Downloads
unzip -q st-stm32cubeide_*.zip
chmod +x st-stm32cubeide_*.sh
sudo ./st-stm32cubeide_*.sh
```

Terima lisensi, izinkan pemasangan udev rules (untuk akses ST-LINK).

---

## Langkah 2 — Pasang CubeMX

Unduh: <https://www.st.com/en/development-tools/stm32cubemx.html> → bagian **Get Software**

⚠️ Ada dua baris di halaman itu:

| Baris | Untuk | Pakai? |
|---|---|---|
| **STM32CubeMX** | semua MCU **kecuali** seri STM32C5 | ✅ |
| STM32CubeMX2 | seri STM32C5 saja | ❌ |


### Windows

Jalankan `SetupSTM32CubeMX-x.x.x.exe`, terima pilihan bawaan.

### Linux

```bash
# Java diperlukan
sudo apt install default-jre      # Debian/Ubuntu
java -version

cd ~/Downloads
unzip -q SetupSTM32CubeMX-*.zip -d cubemx_install
cd cubemx_install
chmod +x SetupSTM32CubeMX-*
./SetupSTM32CubeMX-*
```

---

## Langkah 3 — Izin akses board

### Windows

Driver ST-LINK ikut terpasang bersama CubeIDE. Tidak ada langkah tambahan.

Kalau board tidak terdeteksi, pasang driver terpisah:
<https://www.st.com/en/development-tools/stsw-link009.html>

### Linux

```bash
# udev rules — biasanya sudah terpasang bersama CubeIDE
ls /etc/udev/rules.d/ | grep stlink

# akses port serial (untuk tutorial 3)
sudo usermod -aG dialout $USER
```

Lalu **logout dan login lagi**. 
Kalau udev rules belum ada, salin dari folder CubeIDE:

```bash
sudo cp /opt/st/stm32cubeide_*/plugins/com.st.stm32cube.ide.mcu.externaltools.stlink-gdb-server.linux64_*/tools/bin/*.rules /etc/udev/rules.d/
sudo udevadm control --reload
```

---

## Langkah 4 — Colok board

Board Discovery/Nucleo umumnya punya **dua port USB**. Pakai yang berlabel
**`STLK`** atau **`ST-LINK`** — bukan yang berlabel `USB_OTG` / `UCPD`.

Cek terdeteksi:

**Windows** — Device Manager → cari `STMicroelectronics STLink Virtual COM Port`

**Linux**
```bash
lsusb | grep -i stmicro
ls /dev/ttyACM*
```

---

## Langkah 5 — Buat proyek pertama

1. Buka **CubeMX**
2. **ACCESS TO BOARD SELECTOR** (kalau pakai board jadi)
   atau **ACCESS TO MCU SELECTOR** (kalau pakai chip lepas)
3. Ketik nama board di kotak pencarian, contoh `U5A9` atau `F401`
4. Klik barisnya → **Start Project**
5. Dialog *"Initialize all peripherals with their default Mode?"* → **Yes**
   → ini otomatis mengatur pin LED beserta namanya
6. jika ada*TrustZone** → **Disabled / Without TrustZone**

---

## Langkah 6 — Atur nama & toolchain

Tab **Project Manager** → sub-tab **Project**:

| Isian | Isi |
|---|---|
| Project Name | `blink` |
| Project Location | folder workspace CubeIDE |
| **Toolchain / IDE** | **`STM32CubeIDE`** |

⚠️ Toolchain wajib `STM32CubeIDE`.

Sub-tab **Code Generator** → centang
**"Generate peripheral initialization as a pair of '.c/.h' files per peripheral"**.

---

## Langkah 7 — Generate

Klik **GENERATE CODE**.

Pertama kali akan menawarkan mengunduh firmware seri chip → **Yes**.
**10–25 menit.** Jangan tutup CubeMX. Hanya sekali; proyek berikutnya memakai
firmware yang sama.

Selesai → **Open Project**.

### Verifikasi

Folder proyek harus berisi:

```
blink.ioc     Core/     Drivers/     *.ld
```

Kalau yang muncul `Inc/ Src/ Startup/` → proyek salah, ulangi dari Langkah 5.

Cek nama pin LED:

**Windows (PowerShell)**
```powershell
Select-String -Path Core\Inc\main.h -Pattern "LED"
```

**Linux**
```bash
grep LED Core/Inc/main.h
```

Contoh keluaran pada STM32U5A9J-DK:

```c
#define LED_GREEN_Pin        GPIO_PIN_0
#define LED_GREEN_GPIO_Port  GPIOE
```

Catat nama ini — dipakai di tutorial berikutnya. Setiap board berbeda.

---

## Aturan wajib: USER CODE

Kode buatan sendiri **harus** ditulis di antara penanda:

```c
/* USER CODE BEGIN ... */
   ← tulis di sini
/* USER CODE END ... */
```

---

## Kalau macet

| Gejala | Tindakan |
|---|---|
| `File > New > STM32 Project` tidak ada | Normal di CubeIDE 2.x. Buat proyek dari CubeMX. |
| Proyek tanpa `.ioc` / `Drivers/` | Terpakai wizard Empty Project. Hapus, ulangi. |
| `HAL_Delay` undefined | Tidak ada HAL → proyek Empty. |
| Board tidak terdeteksi | Kabel charging-only, atau salah port USB. |
| ST-LINK "device not found" (Linux) | udev rules / grup `dialout` — lihat Langkah 3. |
| Board tidak muncul di CubeMX | Salah unduh CubeMX2. Pakai CubeMX biasa. |
| Ditawari ST-LINK firmware upgrade | Terima. Normal untuk board baru. |

---

Lanjut ke [2 — Blink](2-blink.md).
