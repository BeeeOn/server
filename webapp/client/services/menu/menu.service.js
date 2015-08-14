'use strict';

angular.module('beeeOnWebApp')
  .factory('Menu',[ '$translate','$translatePartialLoader', function ($translate,$translatePartialLoader) {

    var management = [
      {
        id: 'overview',
        enabled: true,
        title: '',
        img: [
          '../assets/images/icons-png/ic_menu_overview.png',
          '../assets/images/icons-png/ic_menu_overview_active.png'
        ],
        link:'overview.show'},
      {
        id: 'graphs',
        enabled: true,
        title: '',
        img:[
          '../assets/images/icons-png/ic_menu_dashboard.png',
          '../assets/images/icons-png/ic_menu_dashboard_active.png'
        ],
        link:'graphs.show'}
    ];
    $translate(['MENU.OVERVIEW','MENU.GRAPHS']).then(function(translations){
      management[0].title  = translations['MENU.OVERVIEW'];
      management[1].title  = translations['MENU.GRAPHS'];
    });
    var applications = [
      {
        id: 'watchdog',
        enabled: true,
        title: '',
        img: [
          '../assets/images/icons-png/ic_menu_watchdog.png',
          '../assets/images/icons-png/ic_menu_watchdog_active.png'
        ],
        link:'watchdog.show'
      }
    ];
    $translate(['MENU.WATCHDOG']).then(function(translations){
      applications[0].title  = translations['MENU.WATCHDOG'];
    });
    var defaultItems = [
      {
        id: 'settings',
        enabled: true,
        title: '',
        link: 'settings'
      },
      {
        id: 'about',
        enabled: true,
        title: '',
        link: 'about'
      },
      {
        id: 'logout',
        enabled: true,
        title: '',
        link: 'logout'
      }
    ];
    $translate(['MENU.SETTINGS','MENU.ABOUT','MENU.LOGOUT']).then(function(translations){
      defaultItems[0].title  = translations['MENU.SETTINGS'];
      defaultItems[1].title  = translations['MENU.ABOUT'];
      defaultItems[2].title  = translations['MENU.LOGOUT'];
    });
    // Public API here
    return {
      getManagementItems: function () {
        return management;
      },
      getApplicationsItems : function () {
        return applications;
      },
      getDefaultItems : function (){
        return defaultItems;
      }
    };
  }]);
