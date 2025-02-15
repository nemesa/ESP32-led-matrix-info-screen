class MainViewModel {
    constructor() {

        this.menuHandler = new MenuHandler();

        this.ajaxHandler = new AjaxHandler();
        this.connectionInfo = new ConnectionInfoViewModel(this.ajaxHandler);

        const menus = this.menuHandler.menuKeys;
        this.menuHandler.registerMenu(menus.home, 'menu-home', new MenuHomeViewModel(this.menuHandler));
        this.menuHandler.registerMenu(menus.connectToWifi, 'menu-connect-to-wifi', new MenuConnectToWifiViewModel(this.menuHandler, this.ajaxHandler, this.connectionInfo));
        this.menuHandler.registerMenu(menus.settings, 'menu-settings', new MenuSettingsViewModel(this.menuHandler, this.ajaxHandler, this.connectionInfo));
        this.menuHandler.registerMenu(menus.connectionInfo, 'menu-connecion-info', this.connectionInfo);

        this.menuHandler.setMenu(menus.home);
    }


};