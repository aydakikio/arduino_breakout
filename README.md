# Arduino Breakout 🦭

Hook up your circut and play the classic breakout game on your arduino.

## Installation
### Prerequisites
- **Arduino IDE 1.8.x or higher** ([Download here](https://www.arduino.cc/en/software))
- USB cable (USB-A to USB-B for Uno, or appropriate for your board)
- Git (optional, for cloning method)

---

### Method 1: Download from GitHub Release (Easiest)

**Perfect for beginners who just want to use the game**

#### Step 1: Download the Code
1. Visit the [Releases page](https://github.com/aydakikio/arduino_breakout/releases)
2. Click on the latest release
3. Download the **flappy_bird_arduino.ino** file

#### Step 2: Install U8g2 Library
1. Open **Arduino IDE**
2. Go to **Sketch → Include Library → Manage Libraries...**
3. In the search box, type: `U8g2`
4. Find **U8g2 by olikraus**
5. Click **Install**
6. Wait for "Installed" confirmation

#### Step 3: Open the Project
1. In Arduino IDE, go to **File → Open**
2. Navigate to the project folder
3. Select `arduino_breakout.ino`
4. The code will open in the IDE

#### Step 4: Configure Arduino
1. Connect your Arduino to the computer via USB
2. Select Board: **Tools → Board → Arduino AVR Boards → Arduino Uno**
3. Select Port: **Tools → Port → COM3** (Windows) or **/dev/ttyUSB0** (Linux) or **/dev/cu.usbserial** (Mac)
   - The port with "(Arduino Uno)" next to it

#### Step 5: Upload
1. Click the **Upload** button (→ icon) or press `Ctrl+U`
2. Wait for compilation to complete
3. Look for "Done uploading" message at the bottom
4. The game should start immediately!

---

### Method 2: Clone from GitHub

**Perfect for developers who want to modify or contribute**

#### Step 1: Clone the Repository
Open terminal/command prompt and run:

```bash
# Clone using HTTPS
git clone https://github.com/aydakikio/arduino_breakout.git

# Navigate into project folder
cd arduino_breakout/
```

#### Step 2: Install U8g2 Library
1. Open **Arduino IDE**
2. **Sketch → Include Library → Manage Libraries...**
3. Search for `U8g2`
4. Install **U8g2 by olikraus**

#### Step 3: Open the Project
```bash
# Option A: Open from terminal (if arduino-cli installed)
arduino source_code/arduino_breakout.ino

# Option B: Open Arduino IDE manually
# File → Open → Navigate to cloned folder→ source_code folder → arduino_breakout.ino
```

#### Step 4: Configure and Upload
1. **Tools → Board → Arduino AVR Boards → Arduino Uno**
2. **Tools → Port** → Select your Arduino port
3. Click **Upload** (→) or press `Ctrl+U`

---

### Verify Installation
