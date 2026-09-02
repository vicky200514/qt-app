# 1. Steps for flashing the Firmware to the node module
1. Go to the [Meshtastic Web Flasher](https://flasher.meshtastic.org/) to flash the node.
2. Select the target device
   
<img width="1882" height="912" alt="Screenshot 2026-09-01 174319" src="https://github.com/user-attachments/assets/3682fb11-bfed-4cb8-b52c-e3fbc7b0d61a" />

4. Select the node module ( Eg: Seed XIAO ESP32-S3 )
   
<img width="1575" height="512" alt="image" src="https://github.com/user-attachments/assets/d75f4d7f-41b5-40ee-aa3b-890d1054a1f8" />

4. After the selection of node module the firmware automatically gets selected , for further changing firmware by clicking the file upload widget and select the stable version firmware.
   
<img width="1917" height="862" alt="image" src="https://github.com/user-attachments/assets/ebcd1d1d-fe10-4a76-916e-bfb19a938ee4" />

5. Click Flash then click Continue.
6. Put your controller into the bootloader mode while flashing.
7. Select the Baud rate and Enable **Full erase and Install** Widget if you are flashing for the first time, then click **Erase flash and Install**.

   <img width="1198" height="276" alt="Screenshot 2026-09-01 220148" src="https://github.com/user-attachments/assets/8591513d-d810-4a1c-9ca6-2a67e6faf852" />
   <img width="1221" height="532" alt="image" src="https://github.com/user-attachments/assets/6d9894ea-ff41-4d68-9360-c0eece23de8f" />

 A successful flash log ends with:
   ```
   Writing at 0x670000... (100%)
   Wrote 1572864 bytes (1875 compressed) at 0x670000 in ...
   Leaving...
   Hard resetting via RTS pin...
   ```

# 2. Steps for Download and Installation of Meshapp 
1. Ready-made packages are published on the [Gitea Release Packages](https://git.privatepractice.app/covox/meshapp/releases/) 
2. Select the Appropriate Package version (eg: MeshApp v2.3.12)

<img width="1627" height="772" alt="image" src="https://github.com/user-attachments/assets/931baefb-bca4-4d7a-8cf7-283cc2f93ea8" />

4. Select the Download Package based on your Operating systems
  * macOS: download the `.dmg`, open it, and move MeshApp to Applications.
  * Windows: download and run the `.msi` installer.
  * Debian / Ubuntu: download the `.deb` package and install it with apt or your package          manager.
  * Linux AppImage: download the `.AppImage`, make it executable, and run it.
5. After Download Install the MessApp in your System.

# 3. Setup for MessApp
1. Open the MessApp
2. Go to Connections and Click add (+)

   <img width="1917" height="605" alt="image" src="https://github.com/user-attachments/assets/b96038df-c2fd-41e8-b633-03d8426bdaf6" />

3. Select the `Serial/USB` in the Connection type
4. Select the `Meshtastic` in the Protocol
5. Add Name for the Node and Click Autoconnect
6. Select the connected USB Port that shows in your Device Manager.
   
8. Select the Baud rate as `115200`
9. Select the Auto in `DTR/RTS lines`
10. Click Save

<img width="416" height="697" alt="Screenshot 2026-09-02 075205" src="https://github.com/user-attachments/assets/e51d7036-2550-4dad-a604-133a2968dc87" />

10. Click Connect 

<img width="1852" height="276" alt="image" src="https://github.com/user-attachments/assets/58ee52db-1ddf-4224-ba39-b1926e608d37" />

* Note: 
If you facing connection issue
Linux: USB Serial Access
If the USB port is visible but the connection fails with Permission denied, the user does not have access to /dev/ttyUSB* or /dev/ttyACM*.
```bash
ls -l /dev/ttyUSB0
sudo usermod -aG dialout "$USER"
```
On some distributions the group is named `uucp` or `lock`; use the group shown by `ls -l`. After changing groups, log out and log back in.

If the error looks like `Device or resource busy`, the port is already open in another process: serial monitor, CLI, or ModemManager.

# 4 Steps for Node Configuration
After the Deice Get connected, wait for some minute because the configuration settings get process

<img width="1850" height="227" alt="image" src="https://github.com/user-attachments/assets/1b2874ff-112c-414b-90d2-31c10d42e032" />

1. Go to Settings
2. Click LoRa
3. Select Modem preset as LONG_FAST
4. Select Bandwidth
5. Select Region as `IN`(Indian Band)
6. Select Channel Number

<img width="1215" height="822" alt="image" src="https://github.com/user-attachments/assets/a890d59b-a24a-4c64-b21e-0d67d25d662d" />

This all should be same as for another node that you are going to communicate    

7. Click upload that send the changed settings to the node

<img width="1257" height="381" alt="image" src="https://github.com/user-attachments/assets/3c70e911-d40a-4980-ada8-e577d5d29543" />

# 5. Steps for Send and view Message 
1. Go to `Nodes` list in MeshApp
2. Select the Node you are going to communicate and Click `private chat`

<img width="1917" height="1021" alt="Screenshot 2026-09-02 093945" src="https://github.com/user-attachments/assets/975d633f-884a-42d3-bf67-2beb4e6f7443" />

3. Type Message (eg: Hi hello) and Click `Send`

<img width="1250" height="717" alt="Screenshot 2026-09-01 125503" src="https://github.com/user-attachments/assets/b6d068e7-d945-49ec-913d-2dca91d902f0" />

4. Go to receiver node Meshapp and click the message , you will received the message (eg:Hi hello)

<img width="1917" height="1021" alt="Screenshot 2026-09-01 124934" src="https://github.com/user-attachments/assets/4fab0d31-2060-46a0-a076-6b8ca4ce6f50" />

   
