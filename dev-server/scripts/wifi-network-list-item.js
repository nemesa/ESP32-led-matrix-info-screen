class WifiNetworkListItem {
    constructor(scanResult, onConnectCallback) {
        this.ssid = scanResult.SSID;
        this.rssi = parseInt(scanResult.RSSI);
        this.encryptionType = scanResult.EncryptionType;
        this.onConnectClick = async () => {
            if (onConnectCallback) {
                await onConnectCallback(this.ssid, this.encryptionType);
            }
        }
    }
}