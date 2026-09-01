# ESP32S3 LoRa Mesh Setup with MeshApp

This guide documents the full setup: flashing Meshtastic firmware onto a
Seeed XIAO ESP32S3 + Wio-SX1262 LoRa module, installing MeshApp on a laptop,
connecting the module to the app, and communicating over the LoRa mesh.

---

## 1. Firmware Installation (ESP32S3 + Wio-SX1262)

Hardware used: **Seeed XIAO ESP32S3** paired with the **Wio-SX1262** LoRa
expansion module.

1. **Attach the antenna before powering the board on.** Running the SX1262
   radio without an antenna connected can damage the chip.
2. Open the [Meshtastic Web Flasher](https://flasher.meshtastic.org) in
   Chrome or Edge.
3. Connect the board via a **data-capable USB-C cable** (not charge-only).
   Click **Connect** and select the board's serial/COM port.
   - If the board isn't detected, force download mode: hold **BOOT**, plug
     in USB, then release **BOOT**.
4. From the device list, select **"Seeed XIAO S3"** — this is the correct
   target for the XIAO ESP32S3 + Wio-SX1262 combo.
5. Choose the latest **Stable** firmware release and click **Flash**. Do not
   unplug during flashing — the board reboots automatically when done.
6. **Security keys — backup/restore prompt:** on a first-time flash there is
   no prior identity to restore. Skip/decline the restore and let the
   device generate a fresh key pair. (Only restore keys when re-flashing a
   node that already had an identity you want to preserve.)
7. A successful flash log ends with:
   ```
   Writing at 0x670000... (100%)
   Wrote 1572864 bytes (1875 compressed) at 0x670000 in ...
   Leaving...
   Hard resetting via RTS pin...
   ```
   A `NetworkError: The device has been lost` line right after this is
   expected — it's just the browser losing the stale USB serial handle as
   the board resets. It does **not** mean the flash failed.
8. Repeat this whole process for each additional ESP32S3 node.

---

## 2. Installing MeshApp on the Laptop

Ready-made packages are published on the project's releases on tj

- **Windows:** download and run the `.msi` installer.
- **macOS:** download the `.dmg`, open it, and move MeshApp to Applications.
- **Debian / Ubuntu:** download the `.deb` package and install with `apt` or
  your package manager.
  
Repeat installation on each laptop/system that will run its own node.

---

## 3. Connecting the Module to the Laptop

MeshApp supports three connection types. Pick whichever fits your setup —
you can also keep more than one profile per node and switch between them.

### Option A — USB / Serial (most reliable, used for initial setup)

1. Plug the ESP32S3 into the laptop over USB.
2. In MeshApp, go to **Connections → + (Add)**.
3. Set **Protocol: Meshtastic**, **Connection type: Serial/USB**, and select
   the port the board shows up on (MeshApp auto-detects common USB-UART
   chips: CH340, CP210x, FTDI).
4. Click **Connect**.

### Option B — Bluetooth (BLE)

1. In Device Settings → **Bluetooth**, set the pairing mode. `RANDOM_PIN`
   (default) generates a new PIN each pairing attempt, which you'd need to
   catch in a serial log. **FIXED_PIN** is simpler for repeated use — set
   `mode = FIXED_PIN` and a `fixed_pin` value (e.g. `123456`), save, and let
   the device reboot.
2. In MeshApp, add a new connection: **Connection type: BLE**, select the
   device (shows as `Meshtastic_xxxx`).
3. Confirm the pairing prompt on the OS side with the fixed PIN.
4. If you hit a `GATT error` or `pairing not completed` error: forget/remove
   the device from your OS Bluetooth settings first, power-cycle the node,
   then retry — this clears stale bonding records left by earlier failed
   attempts.

### Option C — TCP / WiFi

1. Connect the node over USB first to configure networking.
2. In Device Settings → **Network**: enable `wifi_enabled`, set `wifi_ssid`
   and `wifi_psk` to your WiFi credentials. Save — the device reboots and
   BLE is disabled while WiFi is on (they're mutually exclusive on this
   chip).
   - **Use a 2.4GHz SSID.** The ESP32S3 does not support 5GHz. If your
     router has a combined/band-steered SSID, use the network's separate
     2.4GHz-only SSID if one exists, or disable band steering.
3. Find the node's IP address once it joins the network:
   - Preferred: check your router's admin panel under "Connected Devices" /
     "DHCP Client List".
   - No router access: run `arp -a` in Command Prompt on a laptop on the
     same WiFi,The node's WiFi MAC address will be
     one digit off from its BLE MAC (e.g. BLE `...1A:79` vs WiFi
     `...1A:78`).
4. In MeshApp, add a new connection: **Connection type: TCP**, enter the
   node's IP address, port **4403** (Meshtastic's default TCP port),
   **Protocol: Meshtastic**.
5. Click **Connect**. The USB cable can now be unplugged — MeshApp keeps
   talking to the node wirelessly as long as both stay on the same network.

---

## 4. Invoking the App and Communicating Over LoRa

1. **Launch MeshApp** and confirm the connection card shows **"Connected
   (Meshtastic)"** for your node.
2. **Set the LoRa Region** (required before the radio will transmit):
   - Device Settings → **LoRa** → **Region** → select your region (e.g.
     `IN` for India). It shows `UNSET` by default — the radio stays
     disabled until this is set.
   - Leave **Frequency offset** at `0` unless you have a specific hardware
     calibration reason to change it.
3. **Match settings across both nodes.** For two nodes to hear each other:
   - Same **Region**
   - Same **Modem preset** (default: `LONG_FAST`)
   - Same **Bandwidth**
   - Same **Channel** name.   
4. **Set up each system with its own node**, following the connection steps
   in Section 3, one node per laptop.
5. **Send a message:**
   - Open the **Chats** tab in MeshApp.
   - Select the device name for making communication.
   - On the other system's MeshApp, the message should appear in Chats on
     the same channel within a few seconds.
   - You can verify delivery in the **Packet Table / Logs** view: a
     `TEXT_MESSAGE_APP` packet going out, followed by a `ROUTING_APP` ack
     with `error=NONE` confirms clean delivery.

---

## Note

- Nodes default to **role = CLIENT** — a normal mesh participant that
  sends, receives, and relays messages. No change needed for basic chat.
