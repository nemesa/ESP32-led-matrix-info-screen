class AjaxHandler {
    constructor() {

    }

    async dealay(ms) {
        return new Promise(resolve => setTimeout(resolve, ms));
    }

    async scanWifi() {
        const response = await this.scanWifiNetworks();        
        if (response == "OK") {
            let hasResult = false;
            while (!hasResult) {
                const results = await this.scanWifiNetworkResults()
                if (results && results.networks && results.networks.length > 0) {
                    hasResult = true;
                    return results;
                }
                await this.dealay(1000);
            }
        }
    }

    async scanWifiNetworks() {
        const response = await fetch('/scan-wifi', {
            method: 'GET',
            headers: {
                'Content-Type': 'application/json'
            }
        });
        return response.text();
    }

    async connectionInfo() {
        const response = await fetch('/connection-info', {
            method: 'GET',
            headers: {
                'Content-Type': 'application/json'
            }
        });
        return response.json();
    }

    

    async scanWifiNetworkResults() {
        const response = await fetch('/scan-wifi-result', {
            method: 'GET',
            headers: {
                'Content-Type': 'application/json'
            }
        });
        return response.json();
    }

    async settings() {
        const response = await fetch('/settings', {
            method: 'GET',
            headers: {
                'Content-Type': 'application/json'
            }
        });
        return response.json();
    }

    async connectoToNetwork(ssid, password) {
        const response = await fetch('/connect-wifi', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json'
            },
            body: JSON.stringify({
                ssid: ssid,
                password: password
            })
        });
        return response.text();
    }

    async saveSettings(settings) {
        const response = await fetch('/settings', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json'
            },
            body: JSON.stringify(settings)
        });
        return response.json();
    }
}

