class MenuConnectToWifiViewModel {
    constructor(menuHandler, ajaxHandler, connectionInfo) {
        this.ajaxHandler = ajaxHandler
        this.connectionInfo = connectionInfo;

        this.ajaxHandler = ajaxHandler
        this.scanWifiNetworksButtonDisable = ko.observable(false);
        this.isTryToConnectToWifi = ko.observable(false);
        this.lastScanResults = ko.observableArray([]);
        this.isLoaderVisible = ko.observable(false);

        this.transformWifiScanResultToViewModel = (response, onConnectCallback) => {
            if (response && response.networks && response.networks.length > 0) {

                const resultLookup = {}
                response.networks.forEach(network => {
                    const key = network.SSID + "_" + network.EncryptionType;
                    if (!resultLookup[key]) {
                        resultLookup[key] = network;
                    } else {
                        if (parseInt(network.RSSI) > parseInt(resultLookup[key].RSSI)) {
                            resultLookup[key] = network;
                        }
                    }
                });

                return Object.values(resultLookup).map((scanResult) => new WifiNetworkListItem(scanResult, onConnectCallback));
            }
            return [];
        }

        this.scanWifiNetworks = async () => {
            try {
                this.isLoaderVisible(true);
                this.lastScanResults.removeAll();
                this.scanWifiNetworksButtonDisable(true);
                this.isTryToConnectToWifi(false);
                const response = await this.ajaxHandler.scanWifi();
                this.lastScanResults(this.transformWifiScanResultToViewModel(response, this.onConnectCallback));
            }
            catch (e) {
                toast_error("scanWifiNetworks", e);
            }
            finally {
                this.isLoaderVisible(false);
                this.scanWifiNetworksButtonDisable(false);
                this.isTryToConnectToWifi(false);
            }
        };

        this.onConnectCallback = async (ssid, encryptionType) => {
            let password = "";
            if (encryptionType !== "None") {
                password = prompt("Please enter the password for " + ssid, "");
            }
            if (password !== null) {
                this.isTryToConnectToWifi(true);
                this.scanWifiNetworksButtonDisable(true);
                this.lastScanResults.removeAll();

                try {
                    await this.ajaxHandler.connectoToNetwork(ssid, password)
                    this.connectionInfo.doConnectionPolling(true);
                    menuHandler.setMenu(menuHandler.menuKeys.connectionInfo);
                } catch (e) {
                    console.error(e);
                }
                finally {
                    this.isTryToConnectToWifi(false);
                    this.scanWifiNetworksButtonDisable(false);
                }
            }
        }

        this.afterMenuRequested = () => {
            this.scanWifiNetworks();
        }
    }
};