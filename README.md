# Magang-ICDEC — Tutorial STM32 ARM Cortex-M

Panduan memulai STM32 dari nol: pasang alat, nyalakan LED, kirim teks ke komputer.
Untuk **Windows** dan **Linux**.

Board acuan: **STM32U5A9J-DK** (Cortex-M33).
Board STM32 lain bisa mengikuti; yang berbeda hanya nama pin LED dan nomor USART.

---

## Isi

| Tutorial | Hasil |
|---|---|
| [1 — Setup](1-setup.md) | CubeMX + CubeIDE terpasang, board terdeteksi |
| [2 — Blink](2-blink.md) | LED berkedip |
| [3 — Hello World](3-hello-world.md) | Teks muncul di terminal komputer |

Kode jadi: [`kode/blink.c`](kode/blink.c) · [`kode/hello_world.c`](kode/hello_world.c)

---

## Urutan

```
Setup  ─▶  Blink  ─▶  Hello World
           │          │
           │          └─ chip bisa bicara ke komputer
           └─ rantai compile → flash → jalan sudah terbukti
```

Kerjakan berurutan. Blink membuktikan alat kerja berfungsi. Hello World membuktikan
jalur komunikasi berfungsi. Keduanya prasyarat sebelum program yang lebih besar.

---

## Istilah

- **MCU** — *Microcontroller Unit*. Satu chip berisi prosesor, memori, dan periferal.
- **HAL** — *Hardware Abstraction Layer*. Pustaka ST berisi fungsi siap pakai seperti `HAL_Delay()`.
- **CubeMX** — pengatur pin dan clock. Menghasilkan kerangka proyek.
- **CubeIDE** — editor + compiler + flasher.
- **ST-LINK** — chip pemrogram. Pada board Discovery/Nucleo sudah tertanam.
- **UART** — jalur komunikasi serial dua kabel (TX dan RX).
- **VCP** — *Virtual COM Port*. UART yang lewat kabel USB sehingga terlihat sebagai port serial di komputer.
- **Flash** — memori permanen di dalam chip. Isinya bertahan meski listrik dicabut.
- **Baud** — jumlah bit per detik pada jalur serial.

---

## Lisensi

MIT.
