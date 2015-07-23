'use strict';

angular.module('beeeOnWebApp')
  .controller('MenuCtrl',
  [ '$scope','$location','$log','$state','Gateway','User','Auth','SidePanel','dialogs',
    function ($scope,$location,$log,$state,Gateway,User,Auth,SidePanel,dialogs) {
      $scope.locationOptions = {
        location: 'BeeeOn',
        type: 'panel_close_left',
        hideBtn: false
      };

      $scope.userPanel = {};
      /**
       * Swipe left on menu action
       */
      $scope.onSwipeLeft = function(){
        SidePanel.closeLeft();
      };
      //get current user
      $scope.user = Auth.getCurrentUser();
      //selected gateway from URL
      $scope.selectedGateway = Gateway.getSelectedId();
      //current location from
      $scope.currentLocation = $state.current.name;

      $scope.avatar = $scope.user.pictureURL || '/assets/images/icons-svg/ic_person_white_24px.svg';

      Gateway.getGateways().then(function(data){
        $scope.gateways = data;
      });

      $scope.management = [
        {
          id: 'overview',
          enabled: true,
          title: 'Overview',
          img: [
            '../assets/images/icons-png/ic_menu_overview.png',
            '../assets/images/icons-png/ic_menu_overview_active.png'
          ],
          link:'overview.show'},
        {
          id: 'graphs',
          enabled: true,
          title: 'Graphs',
          img:[
            '../assets/images/icons-png/ic_menu_dashboard.png',
            '../assets/images/icons-png/ic_menu_dashboard_active.png'
            ],
          link:'graphs.show'}
      ];

      $scope.applications = [
        {
          id: 'watchdog',
          enabled: true,
          title: 'Watchdog',
          img: [
            '../assets/images/icons-png/ic_menu_watchdog.png',
            '../assets/images/icons-png/ic_menu_watchdog_active.png'
          ],
          link:'watchdog.show'
        }
      ];

      $scope.default = [
        {
          id: 'settings',
          enabled: true,
          title: 'Settings',
          link: 'settings'
        },
        {
          id: 'about',
          enabled: true,
          title: 'About',
          link: 'about'
        },
        {
          id: 'logout',
          enabled: true,
          title: 'Logout',
          link: 'logout'
        }
      ];

      $scope.changeLocation = function(link){
        if(checkIfCanChange($scope.management,link) || checkIfCanChange($scope.applications,link)){
          $log.debug('Opening: /' + link+ '/' + $scope.selectedGateway);
          $state.go(link,{gatewayId:$scope.selectedGateway});
        }else{
          switch (link){
            case 'settings':
              $state.go('settings');
              break;
            case 'about':
              showAbout();
              break;
            case 'logout':
              logout();
              break;
          }
        }
      };

      var showAbout = function(){
        dialogs.create(
          'components/about/about.html',
          'AboutCtrl',
          {},
          {
            'keyboard': false,
            'backdrop': true,
            'size': 'sm',
            'windowClass': 'aboutModal',
            'copy': false
          }
        );
      };

      /**
       * User logout action
       */
      var logout = function(){
        Auth.logout()
          .then( function() {
            $location.path('/login');
          })
          .catch( function(err) {
            $scope.errors = err;
            $log.error(err);
          });
      };

      var checkIfCanChange = function(section,link){
        var enabled = false;
        angular.forEach(section, function(value,key){
          if(link.split('.')[0].match(value.link.split('.')[0])){
            enabled = true;
          }
        });
        return enabled;
      }
  }]);
