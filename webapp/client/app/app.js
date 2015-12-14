'use strict';

angular.module('beeeOnWebApp', [
  'ngAnimate',
  'ngAria',
  'ngMaterial',
  'ngCookies',
  'ngResource',
  'ngSanitize',
  'ui.router',
  'ui.bootstrap',
  'dialogs.main',
  'vAccordion',
  'ngLodash',
  'pascalprecht.translate',
  'slick',
  'ngMessages',
  'angular-gestures'
])
    .config(['$urlRouterProvider', '$locationProvider', '$mdThemingProvider', '$mdIconProvider', '$logProvider','$httpProvider','$translateProvider','$translatePartialLoaderProvider','hammerDefaultOptsProvider',
    function ($urlRouterProvider, $locationProvider, $mdThemingProvider, $mdIconProvider, $logProvider,$httpProvider,$translateProvider,$translatePartialLoaderProvider,hammerDefaultOptsProvider) {
      /**
       * Set translation service
       */
      $translateProvider.useStaticFilesLoader({
        prefix: 'assets/translations/',
        suffix: '.json'
      });
      $translateProvider.useMissingTranslationHandlerLog();
      $translateProvider.useSanitizeValueStrategy('escapeParameters');
      // remember language
      $translateProvider.preferredLanguage('en');
      //$translateProvider.fallbackLanguage('en');
      $translateProvider.useLocalStorage();
      //$translateProvider.forceAsyncReload(true);
        /**
         * Default redirecting
         */
        $urlRouterProvider
          .otherwise('/home');
        /**
         * Using HTML 5 mode
         */
        $locationProvider.html5Mode(true);
        /**
         * Set angular-material default theme (cyan and pink)
         */
        $mdThemingProvider.theme('default')
          .primaryPalette('cyan', {
            'default': '700', //primary
            'hue-1': '600',   //primary medium
            'hue-2': '900',   //primary dark
            'hue-3': '300'    //primary light
          })
          .accentPalette('pink', {
            'default':'A200', //accent
            'hue-1': 'A100'   //accent light
          });

      /**
       * Define icons
       */
        $mdIconProvider
          //general icons
          .icon('logo', '../assets/images/logo_new.svg')                                              // beeeOn logo full image
          .icon('logo_reversed', '../assets/images/logo_new_reversed.svg')                            // beeeOn logo reversed full image
          .icon('logo-icon', '../assets/images/icons-svg/logo.svg')                                   // beeeOn logo (white, no images)
          .icon('refresh', '../assets/images/icons-svg/ic_cached_48px.svg')                           // refresh icon
          .icon('drop_down_arrow', '../assets/images/icons-svg/ic_arrow_drop_down_48px.svg')          // drop down arrow icon
          .icon('user_profile_default', '../assets/images/icons-svg/ic_account_circle_48px.svg')      // user profile default icon
          .icon('menu_stripes', '../assets/images/icons-svg/ic_menu_48px.svg')                        // 3-stripes menu icon
          .icon('notifications', '../assets/images/icons-svg/ic_notifications_48px.svg')              // Notifications bell icon
          .icon('arrow_back', '../assets/images/icons-svg/ic_arrow_back_white_48px.svg')              // Navigation back arrow
          .icon('done', '../assets/images/icons-svg/ic_done_48px.svg')                                // Checked icon
          .icon('add', '../assets/images/icons-svg/ic_add_white_48px.svg')                            // Add icon (plus sign)
          .icon('g_plus_signin', '../assets/images/login_btn_google.svg')                             // G+ login
          .icon('fb_signin', '../assets/images/login_btn_facebook.svg')                               // Facebook login
          .icon('delete', '../assets/images/icons-svg/ic_delete_white_24px.svg')                      // delete ( basket )
          .icon('description', '../assets/images/icons-svg/ic_description_white_24px.svg')            // page with two stripes
          .icon('edit', '../assets/images/icons-svg/ic_edit_white_24px.svg')                          // edit ( pencil )
          .icon('group', '../assets/images/icons-svg/ic_group_white_24px.svg')                        // two people
          .icon('info', '../assets/images/icons-svg/ic_info_white_24px.svg')                          // info ( filled circle )
          .icon('language', '../assets/images/icons-svg/ic_language_white_24px.svg')                  // globe
          .icon('person', '../assets/images/icons-svg/ic_person_white_24px.svg')                      // person
          .icon('router', '../assets/images/icons-svg/ic_router_white_24px.svg')                      // router
          .icon('supervisor', '../assets/images/icons-svg/ic_supervisor_account_white_24px.svg')      // two people, left bigger
          .icon('more_vert', '../assets/images/icons-svg/ic_more_vert_white_24px.svg')                // vertical more ( three dots )
          .icon('close', '../assets/images/icons-svg/ic_close_white_24px.svg')                        // x
          .icon('arrow_left', '../assets/images/icons-svg/ic_keyboard_arrow_left_white_24px.svg')     // left arrow
          .icon('arrow_right', '../assets/images/icons-svg/ic_keyboard_arrow_right_white_24px.svg')   // right arrow
          .icon('border_color', '../assets/images/icons-svg/ic_border_color_white_24px.svg')          // pen with line in bottom
          .icon('autorenew', '../assets/images/icons-svg/ic_autorenew_white_24px.svg')                // refresh (arrow up and down)
          .icon('battery_full', '../assets/images/icons-svg/ic_battery_full_white_24px.svg')          // full battery
          .icon('query_builder', '../assets/images/icons-svg/ic_query_builder_white_24px.svg')        // clock
          .icon('wifi', '../assets/images/icons-svg/ic_wifi_white_24px.svg')                          // wifi full signal
          //BeeeOn icons
          .icon('b_info', '../assets/images/icons-svg/b_info.svg')                                           // info
          .icon('b_val_temperature', '../assets/images/icons-svg/b_val_temperature.svg')                     // temperature
          .icon('b_dev_unknown', '../assets/images/icons-svg/b_dev_unknown.svg')                             // unknown module
          .icon('b_dev_emission', '../assets/images/icons-svg/b_dev_emission.svg')                           // emission module
          .icon('b_dev_humidity', '../assets/images/icons-svg/b_dev_humidity.svg')                           // humidity module
          .icon('b_dev_light_off', '../assets/images/icons-svg/b_dev_light_off.svg')                         // light module - off
          .icon('b_dev_light_on', '../assets/images/icons-svg/b_dev_light_on.svg')                           // light module - on
          .icon('b_dev_lux', '../assets/images/icons-svg/b_dev_lux.svg')                                     // light intensity module
          .icon('b_dev_noise', '../assets/images/icons-svg/b_dev_noise.svg')                                 // noise intensity module
          .icon('b_dev_pressure', '../assets/images/icons-svg/b_dev_pressure.svg')                           // pressure module
          .icon('b_dev_state', '../assets/images/icons-svg/b_dev_state.svg')                                 // state module
          .icon('b_dev_temperature', '../assets/images/icons-svg/b_dev_temperature.svg')                     // temperature module
          .icon('b_dev_win_closed', '../assets/images/icons-svg/b_dev_win_closed.svg')                       // window module closed
          .icon('b_dev_win_opened', '../assets/images/icons-svg/b_dev_win_opened.svg')                       // window module opened
          .icon('b_dev_temperature_actuator', '../assets/images/icons-svg/b_dev_temperature_actuator.svg')   // temperature actuator
          .icon('b_dev_boiler', '../assets/images/icons-svg/b_dev_boiler.svg')                               // boiler - used for VPT regulator
          //BeeeOn location icons
          .icon('b_loc_living_room', '../assets/images/icons-svg/b_loc_living_room.svg')                     // living rooom
          .icon('b_loc_bathroom', '../assets/images/icons-svg/b_loc_bathroom.svg')                           // bathroom
          .icon('b_loc_bedroom', '../assets/images/icons-svg/b_loc_bedroom.svg')                             // bedroom
          .icon('b_loc_dining_room', '../assets/images/icons-svg/b_loc_dining_room.svg')                     // dining room
          .icon('b_loc_garden', '../assets/images/icons-svg/b_loc_garden.svg')                               // garden
          .icon('b_loc_unknown', '../assets/images/icons-svg/b_loc_unknown.svg')                             // unknown
          .icon('b_loc_wc', '../assets/images/icons-svg/b_loc_wc.svg')                                       // wc
          //TUTORIAL
          //add gateway
          .icon('AA_first_step', '../assets/images/tutorial/AA_first_step.svg')          // connect gateway to internet
          .icon('AA_second_step', '../assets/images/tutorial/AA_second_step.svg')        // connect gateway to AC
          .icon('AA_third_step', '../assets/images/tutorial/AA_third_step.svg');         //
      /**
       * Set debug logging
       */
      $logProvider.debugEnabled(true);
      /**
       * Set message interceptor, which adds token to every message header
       */
      $httpProvider.interceptors.push('authInterceptor');

      hammerDefaultOptsProvider.set({
        recognizers: [[Hammer.Swipe, {enable:true}]]
      });

    }])

    .constant('version','0.0.0')
    .constant('MODULES',{
    'ENUM' : "0x01",
    'TEMPERATURE' : "0x02",
    'HUMIDITY' : "0x03",
    'PRESSURE' : "0x04",
    'LIGHT' : "0x05",
    'NOISE' : "0x06",
    'EMISSIONS' : "0x07",
    'BATTERY' : "0x08",
    'RSSI' : '0x09',
    'REFRESH' : '0x0A'
    })
    .factory('authInterceptor', function ($rootScope, $q, $cookieStore, $location) {
      return {
        // Add authorization token to headers
        request: function (config) {
          config.headers = config.headers || {};
          if ($cookieStore.get('token')) {
            config.headers.Authorization = 'Bearer ' + $cookieStore.get('token');
          }
          return config;
        },

        // Intercept 401s and redirect you to login
        responseError: function(response) {
          if(response.status === 401) {
            $location.path('/login');
            // remove any stale tokens
            $cookieStore.remove('token');
            return $q.reject(response);
          }
          else {
            return $q.reject(response);
          }
        }
      };
    })

    .run(function ($rootScope,$stateParams, $location,$log, $state, Auth,Gateways,SidePanel,$translate) {
      $rootScope.$state = $state;
      $rootScope.$stateParams = $stateParams;
      // Redirect to login if route requires auth and you're not logged in
      $rootScope.$on('$stateChangeStart', function (event, next,params) {
        Auth.isLoggedInAsync(function(loggedIn) {
          if(!loggedIn){
            $log.warn('App - User logged status: ' + loggedIn);
          }
          if (next.authenticate && !loggedIn) {
            $location.path('/login');
          }
          if (next.redirectTo) {
            evt.preventDefault();
            $state.go(next.redirectTo, params);
          }
        });
      });
      //right sidenav tend to stay opened after browser back button click. Close it
      //every time state changes and let controllers to open it themselves
      $rootScope.$watch(function () {return $location.path()}, function (newLocation, oldLocation) {
        SidePanel.closeRight();
      });
      /*$rootScope.$on('$translatePartialLoaderStructureChanged', function () {
        $translate.refresh();
      });*/
    });