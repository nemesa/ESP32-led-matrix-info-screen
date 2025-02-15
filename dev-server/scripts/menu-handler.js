class MenuHandler {
    constructor() {

        this.menus = {};

        this.menuKeys = {
            home: 'menu-home',
            connectToWifi: 'menu-connect-to-wifi',
            settings: 'menu-settings',
            connectionInfo: 'menu-connecion-info',
        }

        this.registerMenu = (menuName, menuItem) => {
            this.menus[menuName] = menuItem;
        }

        this.getMenukey = (key) => {
            if (this.menuKeys[key]) {
                return this.menuKeys[key];
            }
            return this.menuKeys.home;
        }

        this.registerMenu = (key, templateName, viewModel) => {
            this.menus[key] = {
                templateName: templateName,
                viewModel: viewModel
            }
        };

        this.activeMenuTemplateName = ko.observable(null);
        this.activeMenuViewModel = ko.observable(null);


        this.setMenu = (key) => {
            const menuItem = this.menus[key];
            this.activeMenuTemplateName('menu-empty');
            this.activeMenuViewModel(menuItem.viewModel); //set VM first!!!
            this.activeMenuTemplateName(menuItem.templateName);

            if (menuItem.viewModel.afterMenuRequested) {
                menuItem.viewModel.afterMenuRequested();
            }
        }

    }
}