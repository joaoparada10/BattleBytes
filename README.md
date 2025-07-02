# BattleBytes
Simple game written in C that integrates hardware I/O devices: Mouse, Keyboard, CPU Timer and Video Card.
![image](https://github.com/user-attachments/assets/dc542a0f-b260-4fb1-b0f8-8629d8d27e0f)

**Ported and adapted from my group's [Computer Laboratory](https://sigarra.up.pt/feup/en/ucurr_geral.ficha_uc_view?pv_ocorrencia_id=520323) GitLab repository.**

## Technical Features
- 800x600 Resolution @ 30 FPS.
- Double buffering via page flipping.
- Static & Animated Sprites.
- I/O devices interrupt handling.

## Installation
1. Install VirtualBox for your system.
2. Download the zipped VM image.
3. Extract the image from the zip.
4. Open the .vbox file using Virtual Box double-clicking the file should execute Virtual Box with the settings for the MINIX-LCOM VM.
5. Create shared folder under MINIX-LCOM/ (eg. shared) and clone this rep (minus the zip) into it.

## Instructions
1. Start MINIX-LCOM VM
2. Login with the credentials <b>lcom:lcom</b>
3. `cd labs/proj/src`
4. `make`
5. `lcom_run proj`
