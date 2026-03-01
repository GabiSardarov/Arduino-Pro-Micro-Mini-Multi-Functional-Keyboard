# Physical Calculator & Multi-task Keyboard

This project is a small physical device that functions as a **calculator** and a **multi-task keyboard**, using an Arduino Nano or Pro Micro (any board with ATmega32U4) and an OLED display.

---

## Features

### Physical Calculator
- Supports **two modes simultaneously**:
  1. Numbers
  2. Operations

### Multi-task Keyboard
- Includes an **additional 3rd mode** for extra functions
- Not all keys are programmed — **you can add custom functions** to some buttons

---

## Optional Improvements

### Installation / Setup
- Connect your Arduino Pro Micro (or Leonardo — any ATmega32U4 board works).

### Usage
- Use the **ExtraBtn** to switch between modes.  
- The 3rd mode (*keyboard*) is locked at the start; only number and operations modes work.  
- To activate the third mode, hold **ExtraBtn** and (in my case) **1** simultaneously.

### Customization
- The code contains **commented sections** for easier orientation.  
- You can change the function of any button by modifying (or creating) the **key map** to suit your preferences.

---

## Notes
- Use only letters, numbers, and underscores `_` for file and folder names when working with Arduino sketches.
- Make sure the `.ino` file has the **same name as its folder**.

### Images
![IMG_1581](https://github.com/user-attachments/assets/414fc18f-06c3-4d26-98a8-df3a6d3b9f5d)

![IMG_1583](https://github.com/user-attachments/assets/589758fa-0835-43f8-985a-8db57b83c1dd)




https://github.com/user-attachments/assets/7ec3c0c1-ce3e-487d-bc73-983fae32b0cc



