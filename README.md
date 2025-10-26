[![DOI](https://zenodo.org/badge/DOI/10.5281/zenodo.17449335.svg)](https://doi.org/10.5281/zenodo.17449335)
[![Documentation](https://img.shields.io/badge/docs-on%20website-blue)](https://www.camemake.eu/shop/cm-dvp-gc2145-universal-gc2145-dvp-camera-module-2mp-ov2640-replacement-2154)


# GC2145 DVP Camera Driver for the ESP32 Family

**Drop‑in 2‑megapixel replacement for the end‑of‑life OV2640**

The GalaxyCore **GC2145** has the same 8‑bit DVP pin‑out and SCCB/I²C
control bus as OmniVision’s legendary OV2640, but *unlike* the OV2640 it is
**still in production** and easy to source in 2025.  If your ESP32 project
was built around an OV2640 and is now hitting supply‑chain brick‑walls, this
driver lets you swap sensors with almost no code changes.

| Reason the GC2145 is a clean OV2640 alternative |
| --- |
| The OV2640 has been officially **EOL for more than a decade** yet the hobby market kept it alive with old stock — new stock is drying up.﻿ |
| GC2145 matches the 2 MP resolution (1600 × 1200), uses the same **parallel DVP** bus, needs no extra RAM or MIPI, and sells for under $4 in volume. |
| Lower dark current & better low‑light noise than OV2640 and ~15 % lower power. |
| GC2145 silicon is **RoHS, REACH, CE** certified and shipped in volume for smartphones, so long‑term supply is realistic.﻿ |

---

## Supported ESP chips

| SoC | Capture peripheral | Max tested PCLK | Notes |
|-----|--------------------|-----------------|-------|
| **ESP32 (original)** | I²S *camera mode* | 8 MHz | Uses same pin‑map as OV2640 modules |
| **ESP32‑S2** | I²S DVP | 32 MHz | FIFO shared with LCD block |
| **ESP32‑S3** | LCD_CAM | 40 MHz | Dedicated camera engine |

*(Other variants such as ESP32‑C3/C6/H2 lack a parallel camera port and are **not supported**.)*

---

## Driver features

* **One‑shot init** – `gc2145_dvp_init()` boots the sensor (≈ 850 regs),
  patches the `esp32‑camera` sensor object and starts DMA capture.
* Works out‑of‑the‑box with **esp32‑camera** upper‑layers (JPEG streaming,
  face‑detection demo, etc.).
* Runtime helpers: mirror, flip, coarse gain (brightness/contrast tables
  can be added exactly like in the OV2640 driver).
* Example project for ESP32, ESP32‑S2 and ESP32‑S3: grabs one JPEG and logs
  its size.

---

## Quick start

```bash
# clone / add the component
git submodule add https://github.com/<you>/gc2145-dvp.git components/gc2145

idf.py set-target esp32s3   # or esp32 / esp32s2
idf.py menuconfig           # “GC2145 Camera” → set pin map & XCLK
idf.py build flash monitor
```

On reset you should see something like

```
I (123) gc2145_sensor: GC2145 detected (id=0x2145)
I (456) example: Captured 16543 bytes (800x600)
```

---

## Wiring guide (typical AI‑Thinker ESP32‑Cam pin‑out)

| Signal | GC2145 pin | ESP32‑Cam GPIO |
|--------|-----------|----------------|
| XCLK   | MCLK      | GPIO0 / LEDC |
| PCLK   | PCLK      | GPIO22 |
| VSYNC  | VSYNC     | GPIO25 |
| HSYNC  | HREF      | GPIO23 |
| D0–D7  | Parallel bus | GPIO5,18,19,21,36,39,34,35 |
| SCCB SDA | SBDA | GPIO26 |
| SCCB SCL | SBCL | GPIO27 |
| PWDN   | PWDN | GPIO32 |
| RESET  | RESETB | GPIO33 |

*(ESP32‑S2/S3 users: see `example/main.c` for alternate pin‑maps.)*

---

## Extending the driver

The GC2145 offers the same feature set as OV2640 (auto exposure/white
balance, colour matrix, special effects, test pattern, etc.).  To add a new
function:

1. Find the register definition in the **GC2145 datasheet**.
2. Drop a table of `{reg, val}` pairs into `gc2145_sensor.c`.
3. Expose a wrapper similar to `gc2145_sensor_set_gain_db()`.


