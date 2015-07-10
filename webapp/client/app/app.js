'use strict';

angular.module('beeeOnWebApp', [
  'ngAnimate',
  'ngAria',
  'ngMaterial',
  'ngCookies',
  'ngResource',
  'ngSanitize',
  'ui.router',
  'ui.bootstrap'
])
    .config(['$urlRouterProvider', '$locationProvider', '$mdThemingProvider', '$mdIconProvider', '$logProvider',
    function ($urlRouterProvider, $locationProvider, $mdThemingProvider, $mdIconProvider, $logProvider) {
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
            .primaryPalette('cyan')
            .accentPalette('pink', {
                'default':'A200'
            });
      /**
       * Define icons
       */
        $mdIconProvider
          //general icons
          .icon('logo', '../assets/images/logo.svg')                                     // beeeOn logo full image
          .icon('logo-icon', '../assets/images/icons-svg/logo.svg')                             // beeeOn logo (white, no images)
          .icon('refresh', '../assets/images/icons-svg/ic_cached_48px.svg')                     // refresh icon
          .icon('drop_down_arrow', '../assets/images/icons-svg/ic_arrow_drop_down_48px.svg')    // drop down arrow icon
          .icon('user_profile_default', '../assets/images/icons-svg/ic_account_circle_48px.svg')// user profile default icon
          .icon('menu_stripes', '../assets/images/icons-svg/ic_menu_48px.svg')                  // 3-stripes menu icon
          .icon('notifications', '../assets/images/icons-svg/ic_notifications_48px.svg')        // Notifications bell icon
          .icon('arrow_back', '../assets/images/icons-svg/ic_arrow_back_white_48px.svg')        // Navigation back arrow
          .icon('person', '../assets/images/icons-svg/ic_person_white_48px.svg')                // Default user avatar
          .icon('done', '../assets/images/icons-svg/ic_done_48px.svg')                          // Checked icon
          //BeeeOn icons general
          .icon('b_info', '../assets/images/icons-svg/b_info.svg');                   // info
      /**
       * Set debug logging
       */
      $logProvider.debugEnabled(true);
    }])
    //workaround for using theming in non-angular-material components
    .value('clr_primary','#00bcd4')
    .value('clr_accent','#ff4081')
    .run(function ($rootScope, $location, Auth) {
      // Redirect to login if route requires auth and you're not logged in
      $rootScope.$on('$stateChangeStart', function (event, next) {
        Auth.isLoggedIn(function(loggedIn) {
          if (next.authenticate && !loggedIn) {
            $location.path('/login');
          }
        });
      });
    });