'use strict';

angular.module('beeeOnWebApp')
  .controller('MenuCtrl',
  [ '$scope','$location','$log','$state','$stateParams','$window','Gateways','User','Auth','SidePanel','dialogs','Menu','loadingModal',
    function ($scope,$location,$log,$state,$stateParams,$window,Gateways,User,Auth,SidePanel,dialogs,Menu,loadingModal) {
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
      $scope.selectedGateway = Gateways.getSelectedId();
      //current location from
      $scope.currentLocation = $state.current.name;
      console.log($scope.user);
      $scope.user.$promise.then(function(){
        $scope.avatar = $scope.user.imgurl || '/assets/images/icons-svg/ic_person_white_24px.svg';
      });
      Gateways.getGateways().then(function(data){
        if(!data || angular.equals([], data)){
          $log.info('No gateways');
          //url fail safe, if trying to access exact gateway, but no gateways are registered
          if($scope.selectedGateway){
            $state.go("home");
          }
          return;
        }
        //assign retrieved gateway data
        $scope.gateways = data;

        //if no gateway is selected, select first
        if(!$scope.selectedGateway){
          //if state is home and gateways exist, redirect to "modules" state
          if($state.current.name.match("home")){
            $scope.changeLocation("devices",data[0].id);
          }else {
            $scope.changeLocation($state.current.name.split('.')[0],data[0].id);
          }
        }
        //check if gateway number in URL is in his gateways
        if($stateParams.gatewayId && !Gateways.findGatewayById($stateParams.gatewayId)){
          $log.warn('User doesn\'t own requested gateway ('+$stateParams.gatewayId+')');
          $scope.changeLocation($state.current.name.split('.')[0],data[0].id);
          /*$state.go('modules');
          //FIXME find another way to reload page
          $window.location.reload();*/
        }
      });

      $scope.overview = Menu.getOverviewItems();
      $scope.management = Menu.getManagementItems();
      $scope.default = Menu.getDefaultItems();

      $scope.selectedGateway = Gateways.getSelectedId();

      $scope.changeLocation = function(link,gateway){
        //if no gateway is given get it from radio group
        if(!gateway){
          gateway = $scope.selectedGateway;
        }else {
          $scope.selectedGateway = gateway;
        }
        //save new gateway number
        Gateways.setSelected(gateway);
        //check, if desired state is in enabled locations (management, applications)
        if(checkIfCanChange($scope.overview,link) || checkIfCanChange($scope.management,link)){
          $log.debug('MenuCtrl - Opening: /' + link+ '/' + $scope.selectedGateway);
          SidePanel.closeLeft();
          $state.go(link,{gatewayId:Gateways.getSelectedId()});
        }else{
          //is desired state one of default ?
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
        //show about modal
        dialogs.create(
          'components/modal/about/about.html',
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
            Auth.clearLocalData();
            $location.path('/login');
          })
          .catch( function(err) {
            $scope.errors = err;
            $log.error('MenuCtrl - ' + err);
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
