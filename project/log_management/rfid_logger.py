import serial
import time
import openpyxl
from openpyxl import Workbook

# ---------- Configuration ----------
PORT = '/dev/ttyUSB0'   # Change this (e.g., 'COM3' on Windows)
BAUD_RATE = 115200
FILE_NAME = 'rfid_log.xlsx'
# -----------------------------------

def create_excel_file():
    """Create a new Excel file with headers if not exists."""
    wb = Workbook()
    ws = wb.active
    ws.title = "RFID Logs"
    ws.append(["Timestamp", "Card UID", "Name / Status"])
    wb.save(FILE_NAME)
    print(f"[+] Created new log file: {FILE_NAME}")

def append_to_excel(uid, name_status):
    """Append a new log entry to the Excel file."""
    wb = openpyxl.load_workbook(FILE_NAME)
    ws = wb.active
    ws.append([time.strftime("%Y-%m-%d %H:%M:%S"), uid, name_status])
    wb.save(FILE_NAME)
    print(f"[✓] Saved: {uid} -> {name_status}")

def read_serial():
    """Read RFID data from serial and save to Excel."""
    try:
        ser = serial.Serial(PORT, BAUD_RATE, timeout=1)
        time.sleep(2)  # Wait for serial connection to stabilize
        print(f"[+] Listening on {PORT}...")
    except Exception as e:
        print(f"[!] Error opening serial port: {e}")
        return

    if not os.path.exists(FILE_NAME):
        create_excel_file()

    while True:
        line = ser.readline().decode('utf-8').strip()
        if line:
            print(f"[Serial] {line}")

            # Detect UID line
            if line.startswith("Card UID:"):
                uid = line.replace("Card UID:", "").strip()
            # Detect known name or status
            elif line.startswith("Hello,"):
                name = line.replace("Hello,", "").strip()
                append_to_excel(uid, f"Access Granted ({name})")
            elif "Unknown card" in line:
                append_to_excel(uid, "Access Denied (Unknown Card)")

if __name__ == "__main__":
    import os
    if not os.path.exists(FILE_NAME):
        create_excel_file()
    read_serial()
