# 1. Steps for flashing the Firmware to the node module
1. Go to the [Meshtastic Web Flasher](https://flasher.meshtastic.org/) to flash the node.
2. Select the target device
<img width="1882" height="912" alt="Screenshot 2026-09-01 174319" src="https://github.com/user-attachments/assets/3682fb11-bfed-4cb8-b52c-e3fbc7b0d61a" />
3. Select the node module ( Eg: Seed XIAO ESP32-S3 )
<img width="1575" height="512" alt="image" src="https://github.com/user-attachments/assets/d75f4d7f-41b5-40ee-aa3b-890d1054a1f8" />
4. After the selection of node module the firmware automatically gets selected , for further changing firmware by clicking the file upload widget and select the stable version firmware.
<img width="1917" height="862" alt="image" src="https://github.com/user-attachments/assets/ebcd1d1d-fe10-4a76-916e-bfb19a938ee4" />
5. Click Flash then click Continue.
6. Put your controller into the bootloader mode while flashing.
7. Select the Baud rate and Enable **Full erase and Install** Widget if you are flashing for the first time, then click **Erase flash and Install**.

   <img width="1198" height="276" alt="Screenshot 2026-09-01 220148" src="https://github.com/user-attachments/assets/8591513d-d810-4a1c-9ca6-2a67e6faf852" />
   <img width="1221" height="532" alt="image" src="https://github.com/user-attachments/assets/6d9894ea-ff41-4d68-9360-c0eece23de8f" />

# 2. Steps for Download and Installation of Meshapp 
1. Ready-made packages are published on the [Gitea Release Packages](https://git.privatepractice.app/covox/meshapp/releases/) 
2. Select the Appropriate Package version (eg: MeshApp v2.3.12)
<img width="1627" height="772" alt="image" src="https://github.com/user-attachments/assets/931baefb-bca4-4d7a-8cf7-283cc2f93ea8" />
3. Select the Download Package based on your Operating systems
       * macOS: download the .dmg, open it, and move MeshApp to Applications.
       * Windows: download and run the .msi installer.
       * Debian / Ubuntu: download the .deb package and install it with apt or your package          manager.
       * Linux AppImage: download the .AppImage, make it executable, and run it.
