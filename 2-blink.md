# 2 — Blink

Menyalakan dan memadamkan LED di board.

 membuktikan rantai kerja berfungsi:

```
CubeMX  ─▶  CubeIDE  ─▶  compiler ARM  ─▶  ST-LINK  ─▶  chip menjalankan kode
```

Prasyarat: [1 — Setup](1-setup.md) selesai.

---

## Langkah 1 — Cari nama pin LED

Buka `Core/Inc/main.h`, cari `LED`.

Contoh beberapa board:

| Board | LED | Pin |
|---|---|---|
| STM32U5A9J-DK | `LED_GREEN` | PE0 |
| Nucleo-F401RE | `LD2` | PA5 |
| Nucleo-F103RB | `LD2` | PA5 |
| STM32F4-Discovery | `LD4` | PD12 |

Kalau tidak ada define `LED` sama sekali, berarti board belum mengatur pin LED.
Atur manual di CubeMX: klik pin LED di gambar chip → **GPIO_Output** →
klik kanan → **Enter User Label** → beri nama `LED_GREEN` → GENERATE CODE.

---

## Langkah 2 — Tulis kode

Buka `Core/Src/main.c`. Cari `USER CODE BEGIN WHILE` (Ctrl+F).

Ganti menjadi:

```c
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET);
    HAL_Delay(500);

    HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);
    HAL_Delay(500);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
```

Ganti `LED_GREEN` dengan nama pin board masing-masing.

Kode lengkap: [`kode/blink.c`](kode/blink.c)

### Arti tiap fungsi

| Fungsi | Arti |
|---|---|
| `HAL_GPIO_WritePin(port, pin, GPIO_PIN_SET)` | Beri tegangan ke pin → LED nyala |
| `HAL_GPIO_WritePin(port, pin, GPIO_PIN_RESET)` | Putus tegangan → LED mati |
| `HAL_Delay(500)` | Diam 500 milidetik |

Versi lebih ringkas, satu fungsi untuk membalik keadaan pin:

```c
  while (1)
  {
    HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
    HAL_Delay(500);
```

> **Catat:** kurung `}` penutup `while` **sudah disediakan** CubeMX setelah
> `USER CODE BEGIN 3`. Jangan menambah `}` sendiri di dalam blok WHILE.

Simpan: **Ctrl+S**

---

## Langkah 3 — Build

Klik ikon **palu 🔨** (Ctrl+B).

Build pertama lama karena mengkompilasi seluruh HAL.

Console harus berakhir:

```
Build Finished. 0 errors, 0 warnings
```

---

## Langkah 4 — Flash

Colok board ke port **`STLK`**, lalu klik **▶ hijau** (Run).

Dialog *Edit Configuration* muncul pertama kali:

| Isian | Isi |
|---|---|
| Debug probe | ST-LINK (ST-LINK GDB server) |
| Interface | SWD |

Sisanya biarkan → **OK**.

Kalau ditawari **ST-LINK firmware upgrade** → terima, tunggu, ulangi Run.

Console harus berakhir:

```
Download verified successfully
```

---

## Hasil

**LED berkedip ± 1 kali per detik.**

Yang terbukti:

- CubeMX bisa menghasilkan proyek untuk board ini
- CubeIDE bisa mengkompilasi kode untuk inti ARM Cortex-M
- ST-LINK bisa menulis ke flash chip
- Chip menjalankan kode buatan sendiri

---

## Ubah kecepatan kedip

Ganti angka `500`:

| Nilai | Hasil |
|---|---|
| `100` | cepat |
| `500` | 1 kedip per detik |
| `2000` | lambat |

Build ulang, flash ulang.

---

## Kalau macet

| Gejala | Tindakan |
|---|---|
| `HAL_GPIO_WritePin` undefined | Proyek Empty tanpa HAL. Ulangi dari setup Langkah 5. |
| `LED_GREEN_Pin` undefined | Nama pin beda di board ini. Cek `Core/Inc/main.h`. |
| Build 0 errors tapi LED mati | Lupa flash. Palu hanya compile; Run yang menyalin ke chip. |
| Kode hilang setelah GENERATE CODE | Ditulis di luar `USER CODE BEGIN/END`. Tulis ulang di dalam penanda. |
| Error `expected declaration` saat build | Ada `}` berlebih di blok WHILE. CubeMX sudah menyediakan penutupnya. |
| LED nyala terus, tidak berkedip | `HAL_Delay` hilang, atau `SysTick` tidak aktif. |
| Board tidak terdeteksi | Kabel charging-only, atau salah port USB. |

---

Lanjut ke [3 — Hello World](3-hello-world.md).
