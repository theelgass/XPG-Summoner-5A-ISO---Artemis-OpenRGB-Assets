# XPG Summoner 5A (ISO) – Artemis / OpenRGB Assets
This repository contains an **layout definition for Artemis software (`rgb.net`)** and **OpenRGB controller** for the **XPG Summoner 5A keyboard (ISO layout)**.

## Overview

The controller implementation is **based on the original controller for ANSI layout** included in the **OpenRGB project**, which is licensed under **GPL-2.0-or-later**.

## Known Issues

- ❌ **Static color setting does not persist** - keyboard **reverts to its default hardware lighting mode**

## Controller build instructions

To build and use this controller:

1. Clone the **OpenRGB** source code repository.
2. Replace the files related to the keyboard controller in the OpenRGB source tree with the modified versions provided in this repository.
3. Build OpenRGB **according to the official OpenRGB build instructions** for your platform.

## License

This project is distributed under the **GPL-2.0-or-later** license, in accordance with the license of the original OpenRGB controller code it is based on.

## Disclaimer

This project is **not officially affiliated** with:
- ADATA / XPG
- OpenRGB
- Artemis (`rgb.net`)

All trademarks belong to their respective owners.
