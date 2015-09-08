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
  'pascalprecht.translate'
])
    .config(['$urlRouterProvider', '$locationProvider', '$mdThemingProvider', '$mdIconProvider', '$logProvider','$httpProvider','$translateProvider','$translatePartialLoaderProvider',
    function ($urlRouterProvider, $locationProvider, $mdThemingProvider, $mdIconProvider, $logProvider,$httpProvider,$translateProvider,$translatePartialLoaderProvider) {
      /**
       * Set translation service
       */
      $translateProvider.useStaticFilesLoader({
        prefix: 'assets/translations/',
        suffix: '.json'
      });
      /*$translateProvider.useLoader('$translatePartialLoader', {
        urlTemplate: 'assets/translations/{part}/{lang}.json'
      });
      $translatePartialLoaderProvider.addPart('global');
      $translatePartialLoaderProvider.addPart('login');
      $translatePartialLoaderProvider.addPart('menu');
      $translatePartialLoaderProvider.addPart('overview');
      $translatePartialLoaderProvider.addPart('settings');
      $translatePartialLoaderProvider.addPart('watchdog');
*/
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
          .otherwise('/overview');
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
          })

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
          //BeeeOn icons general
          .icon('b_info', '../assets/images/icons-svg/b_info.svg')                              // info
          .icon('b_val_temperature', '../assets/images/icons-svg/b_val_temperature.svg');       // temperature
      /**
       * Set debug logging
       */
      $logProvider.debugEnabled(true);
      /**
       * Set message interceptor, which adds token to every message header
       */
      $httpProvider.interceptors.push('authInterceptor');

    }])

    .constant('version','0.0.0')

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