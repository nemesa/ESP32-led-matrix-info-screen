class ConnectionInfoViewModel {
    constructor(ajaxHandler) {

        this.ajaxHandler = ajaxHandler;
        this.doConnectionPolling = ko.observable(false);

        this.afterMenuRequested = () => { };

        this.startPolling = () => {
            console.log("start polling");
            this.doConnectionPolling(true);
        }

        this.stopPolling = () => {
            console.log("stop polling");
            this.doConnectionPolling(false);
        }

        this.ipAddress = ko.observable('');
        this.rssi = ko.observable(null);
        this.last_status = 6
        this.status = ko.observable(6);

        this.hasIpAddress = ko.pureComputed(() => {
            if (this.doConnectionPolling() && this.ipAddress()) {
                return true;
            }
            return false;
        }, this);

        ko.computed(() => {
            if (this.doConnectionPolling()) {
                this.startPolling();
            }
            else {
                this.stopPolling();
            }
        }, this);

        this.inPoll = false;

        this.getStatusText = () => {
            switch (this.status()) {
                case 0: return "No Shiled";//"WL_NO_SHIELD";
                case 1: return "Idle";//"WL_IDLE_STATUS";
                case 2: return "No SSID avaliable";//"WL_NO_SSID_AVAIL";
                case 3: return "Connected";//"WL_CONNECTED";
                case 4: return "Connection Failed";//"WL_CONNECT_FAILED";
                case 5: return "Connection Lost";//"WL_CONNECTION_LOST";
                case 6: return "Disconnected";//"WL_DISCONNECTED";
                default: return "UNKNOWN";
            }
        }

        this.getInfo = () => {
            this.ajaxHandler.connectionInfo().then((response) => {
                this.ipAddress(response.ip);
                this.rssi(response.rssi);
                this.status(response.status);

                if (this.last_status !== response.status && response.status !== 3 /*WL_CONNECTED*/) {
                    toast(`Connection status: ${this.getStatusText()}`, "danger", true);
                }
                this.last_status = response.status;

            }).catch((e) => {
                toast_error("connection-status-pooller connectionInfo", e, true);
            }).finally(() => {
                this.inPoll = false;
            });
        }

        const interval = setInterval(() => {
            const doPolling = this.doConnectionPolling();
            if (doPolling) {
                try {
                    if (!this.inPoll) {
                        this.inPoll = true;
                        this.getInfo();
                    }
                } catch (error) {
                    toast_error("connection-status-pooller", e, true);
                    this.inPoll = false;
                }
            }
        }, 5000);

        this.killTimer = () => {
            clearInterval(interval);
        }

        this.getInfo();
    }
};