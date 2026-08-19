# 3 — Hello World

Mengirim teks dari chip ke komputer lewat UART, ditampilkan di terminal serial.

Prasyarat: [2 — Blink](2-blink.md) selesai.

---

## Cara kerjanya

```
printf("Hello")
     │
     ▼  memecah jadi huruf satuan: 'H' 'e' 'l' 'l' 'o'
__io_putchar()          ← fungsi yang kita tulis sendiri
     │
     ▼
HAL_UART_Transmit()
     │
     ▼
USART1  ─▶  pin TX  ─▶  chip ST-LINK  ─▶  USB  ─▶  komputer
```

`printf` adalah fungsi standar C. Ia tidak tahu apa-apa tentang UART — ia hanya
memanggil `__io_putchar()` untuk setiap huruf. Di komputer biasa fungsi itu
disediakan sistem operasi. Di mikrokontroler tidak ada sistem operasi, jadi
fungsi itu kosong dan teksnya hilang.

Menulis ulang `__io_putchar()` mengisi fungsi kosong itu. Setelah itu **semua**
`printf` di seluruh program otomatis mengalir ke komputer.

### VCP — kenapa satu kabel cukup

Board Discovery/Nucleo punya chip ST-LINK tertanam yang berfungsi ganda:
memprogram chip **dan** menjadi jembatan USB↔serial. Jadi satu kabel USB
dipakai untuk flash sekaligus membaca teks.

Di komputer muncul sebagai:

- **Windows** — `COM3`, `COM4`, dst
- **Linux** — `/dev/ttyACM0`

### Satu byte = 10 bit

UART mengirim tiap byte sebagai 10 pulsa: 1 start + 8 data + 1 stop.

```
byte per detik = baud ÷ 10

115200 baud  →  11.520 byte/detik
```

Kedua sisi harus memakai baud yang sama. Tidak ada kabel jam — masing-masing
menghitung waktu sendiri. Beda setelan → yang muncul huruf sampah.

---

## Langkah 1 — Pastikan UART aktif

Kalau proyek dibuat dengan *Initialize all peripherals = Yes*, UART biasanya
sudah aktif.

Cek di `Core/Src/main.c` — harus ada pemanggilan init:

```c
MX_USART1_UART_Init();
```

Nomor USART berbeda antar board. Cek juga pin yang terhubung ke ST-LINK:

| Board | USART | Pin TX/RX |
|---|---|---|
| STM32U5A9J-DK | USART1 | PA9 / PA10 |
| Nucleo-F401RE | USART2 | PA2 / PA3 |
| Nucleo-F103RB | USART2 | PA2 / PA3 |

Cari di file `.ioc` label `VCP_TX` / `VCP_RX` — itu penanda pin yang tersambung
ke ST-LINK.

**Kalau belum aktif:** buka `.ioc` → **Connectivity → USARTx** → Mode =
**Asynchronous** → Baud Rate = **115200** → GENERATE CODE.

---

## Langkah 2 — Tulis kode

Tiga sisipan di `Core/Src/main.c`.

**Sisipan 1** — `USER CODE BEGIN Includes`:

```c
/* USER CODE BEGIN Includes */
#include <stdio.h>
/* USER CODE END Includes */
```

**Sisipan 2** — `USER CODE BEGIN 0`:

```c
/* USER CODE BEGIN 0 */
int __io_putchar(int ch)
{
  HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
  return ch;
}
/* USER CODE END 0 */
```

Ganti `huart1` sesuai USART board (`huart2` untuk Nucleo, dst).

**Sisipan 3** — `USER CODE BEGIN WHILE`:

```c
  /* USER CODE BEGIN WHILE */
  printf("\r\nHello World!\r\n");
  printf("Clock: %lu Hz\r\n\r\n", HAL_RCC_GetSysClockFreq());

  uint32_t hitung = 0;

  while (1)
  {
    printf("Hello World ke-%lu\r\n", (unsigned long)(++hitung));
    HAL_Delay(1000);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
```

Kode lengkap: [`kode/hello_world.c`](kode/hello_world.c)

> **Pakai `\r\n`, bukan `\n` saja.** `\n` pindah baris, `\r` kembali ke kolom
> kiri. Terminal serial butuh keduanya. Kalau hanya `\n`, teks muncul menjenjang
> seperti tangga.

### Kenapa cukup menulis ulang satu fungsi

`Core/Src/syscalls.c` bawaan CubeIDE sudah berisi:

```c
__attribute__((weak)) int _write(int file, char *ptr, int len)
{
  for (DataIdx = 0; DataIdx < len; DataIdx++)
    __io_putchar(*ptr++);
  return len;
}
```

`printf` memanggil `_write()`, `_write()` menyerahkan tiap huruf ke
`__io_putchar()`. Fungsi itu belum berisi, jadi tinggal didefinisikan sendiri.
`syscalls.c` tidak perlu disentuh.

---

## Langkah 3 — Siapkan terminal serial

### Windows

Pilih salah satu:

- **PuTTY** — <https://www.putty.org/> → Connection type: **Serial**,
  Serial line: `COM3`, Speed: `115200`
- **Tera Term** — <https://teratermproject.github.io/>
- **Terminal bawaan CubeIDE** — `Window → Show View → Terminal` → ikon monitor →
  Serial Terminal

Cek nomor COM di **Device Manager → Ports (COM & LPT)**.

### Linux

```bash
# pilih salah satu
sudo apt install picocom
sudo apt install python3-serial
```

Izin akses port:

```bash
sudo usermod -aG dialout $USER
```

Lalu **logout dan login lagi**. Cek:

```bash
test -r /dev/ttyACM0 && test -w /dev/ttyACM0 && echo BISA
```

Jalan pintas tanpa logout, berlaku untuk satu terminal saja:

```bash
newgrp dialout
```

---

## Langkah 4 — Build, flash, dengarkan

1. **Ctrl+S**
2. **Palu 🔨** → `0 errors`
3. **▶ hijau** → `Download verified successfully`
4. Buka terminal serial

**Windows** — PuTTY: Serial, `COM3`, `115200` → Open

**Linux**
```bash
picocom -b 115200 /dev/ttyACM0
# atau
python3 -m serial.tools.miniterm /dev/ttyACM0 115200
```

Keluar picocom: **Ctrl+A** lalu **Ctrl+X**
Keluar miniterm: **Ctrl+]**

### Yang muncul

```
Hello World!
Clock: 48000000 Hz

Hello World ke-1
Hello World ke-2
Hello World ke-3
```

---

## Hasil

Chip bisa mengirim data ke komputer. Ini dasar untuk:

- **Debugging** — mencetak nilai variabel tanpa perlu debugger
- **Mengukur** — mencetak waktu eksekusi, frekuensi clock, pemakaian memori
- **Mengirim data** — hasil sensor, hasil olahan, audio

`printf("Clock: ...")` di atas sekaligus berguna: ia menunjukkan chip berjalan
di frekuensi berapa. Banyak board Discovery menyala di frekuensi rendah secara
bawaan demi hemat daya — tanpa mencetaknya, hal itu tidak akan diketahui.

---

## Kalau macet

| Gejala | Tindakan |
|---|---|
| Port serial sunyi total | Lupa flash. Palu hanya compile. |
| Huruf sampah `ÿØ¿þ` | Baud tidak sama. Samakan jadi 115200 di kedua sisi. |
| `huart1` undefined | Nama handle beda. Cek `MX_USARTx_UART_Init` di `main.c`. |
| Teks menjenjang seperti tangga | Pakai `\r\n`, bukan `\n`. |
| `/dev/ttyACM0` tidak ada (Linux) | Board tidak terdeteksi, atau kabel charging-only. |
| Permission denied (Linux) | Belum masuk grup `dialout`, atau belum logout-login. |
| COM port tidak muncul (Windows) | Pasang driver ST-LINK (STSW-LINK009). |
| Port terpakai program lain | Tutup terminal serial lain yang masih terbuka. |
| Teks muncul lalu berhenti | Chip hang atau masuk `Error_Handler()`. |

> **Windows:** tutup terminal serial sebelum flash. Port yang sedang dipakai
> bisa menghalangi proses flash.
