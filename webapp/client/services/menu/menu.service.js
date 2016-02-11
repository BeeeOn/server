'use strict';

angular.module('beeeOnWebApp')
  .factory('Menu',[ '$translate','$translatePartialLoader', function ($translate,$translatePartialLoader) {

    var overview = [
      {
        id: 'modules',
        enabled: false,
        title: '',
        img: [
          '../assets/images/icons-png/ic_menu_overview.png',
          '../assets/images/icons-png/ic_menu_overview_active.png'
        ],
        link:'modules'
      },
      {
        id: 'devices',
        enabled: true,
        title: '',
        img: [
          '../assets/images/icons-png/ic_menu_a_gate.png',
          '../assets/images/icons-png/ic_menu_a_gate_active.png'
        ],
        link:'devices'
      },
      {
        id: 'graphs',
        enabled: true,
        title: '',
        img:[
          '../assets/images/icons-png/ic_menu_dashboard.png',
          '../assets/images/icons-png/ic_menu_dashboard_active.png'
        ],
        link:'graphs'
      },
      {
        id: 'active-gateway',
        enabled: true,
        title: '',
        img: [
          '../assets/images/icons-png/ic_menu_a_gate.png',
          '../assets/images/icons-png/ic_menu_a_gate_active.png'
        ],
        link:'active-gateway'
      },
      {
        id: 'watchdog',
        enabled: false,
        title: '',
        img: [
          '../assets/images/icons-png/ic_menu_watchdog.png',
          '../assets/images/icons-png/ic_menu_watchdog_active.png'
        ],
        link:'watchdog'
      }
    ];
    $translate(['MENU.MODULES','MENU.DEVICES','MENU.GRAPHS','MENU.ACTIVE_GATEWAY','MENU.WATCHDOG']).then(function(translations){
      overview[0].title  = translations['MENU.MODULES'];
      overview[1].title  = translations['MENU.DEVICES'];
      overview[2].title  = translations['MENU.GRAPHS'];
      overview[3].title  = translations['MENU.ACTIVE_GATEWAY'];
      overview[4].title  = translations['MENU.WATCHDOG'];
    });
    var management = [
      /*{
        id: 'active-gateway',
        enabled: true,
        title: '',
        img: [
          '../assets/images/icons-png/ic_menu_a_gate.png',
          '../assets/images/icons-png/ic_menu_a_gate_active.png'
        ],
        link:'active-gateway'
      },
      {
        id: 'devices',
        enabled: true,
        title: '',
        img: [
          '../assets/images/icons-png/ic_menu_a_gate.png',
          '../assets/images/icons-png/ic_menu_a_gate_active.png'
        ],
        link:'devices'
      }*/
    ];
    /*$translate(['MENU.ACTIVE_GATEWAY','MENU.DEVICES']).then(function(translations){
      management[0].title  = translations['MENU.ACTIVE_GATEWAY'];
      management[1].title  = translations['MENU.DEVICES'];
    });*/
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
      getOverviewItems: function () {
        return overview;
      },
      getManagementItems : function () {
        return management;
      },
      getDefaultItems : function (){
        return defaultItems;
      }
    };
  }]);
