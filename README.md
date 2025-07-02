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
2. Clone this repository.
3. Extract MINIX-LCOM.zip.
4. Open the .vbox file using Virtual Box double-clicking the file should execute Virtual Box with the settings for the MINIX-LCOM VM.
5. Create shared folder under MINIX-LCOM/ (eg. shared) and select it in the settings (should be like this by default) ![image](https://github.com/user-attachments/assets/69379836-5d51-4414-a810-763f1dcc99dd)
6. Copy the other folders (proj & .minix-src) into shared.


## Instructions
1. Start MINIX-LCOM VM
2. Login with the credentials <b>lcom:lcom</b>
3. `cd labs/proj/src`
4. `make`
5. `lcom_run proj`
6. Use Mouse and Keyboard to play!
