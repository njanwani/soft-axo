
from ble_serial.scan import main as scanner
import asyncio

async def main():
    ### general scan
    ADAPTER = "hci0"
    SCAN_TIME = 5 #seconds
    SERVICE_UUID = None # optional filtering

    devices = await scanner.scan(ADAPTER, SCAN_TIME, SERVICE_UUID)

    print() # newline
    scanner.print_list(devices, verbose=False)
    axo = None
    for device in devices:
        # print(devices[device])
        if devices[device][0].name == 'AXO':
            print('Found AXO')
            # return device.address

if __name__ == "__main__":
    asyncio.run(main())