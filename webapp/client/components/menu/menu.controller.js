'use strict';

angular.module('beeeOnWebApp')
  .controller('MenuCtrl',
  [ '$scope','$location','$log','$state','$stateParams','Gateways','User','Auth','SidePanel','dialogs','Menu','loadingModal',
    function ($scope,$location,$log,$state,$stateParams,Gateways,User,Auth,SidePanel,dialogs,Menu,loadingModal) {
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

      $scope.avatar = $scope.user.pictureURL || '/assets/images/icons-svg/ic_person_white_24px.svg';
      Gateways.getGateways().then(function(data){
        if(!data || angular.equals({}, data)){
          $log.warn('No gateways');
          return;
        }
        //assign retrieved gateway data
        $scope.gateways = data;
        //if no gateway is selected, select first
        if(!$scope.selectedGateway){
          $scope.changeLocation($state.current.name.split('.')[0]+'.show',data[0].aid);
        }
        //check if gateway number in URL is in his gateways
        if($stateParams.gatewayId && !Gateways.findGatewayById($stateParams.gatewayId)){
          $log.warn('User doesn\'t own requested gateway ('+$stateParams.gatewayId+')');
          $state.go('overview');
        }
      });

      $scope.management = Menu.getManagementItems();
      $scope.applications = Menu.getApplicationsItems();
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
        if(checkIfCanChange($scope.management,link) || checkIfCanChange($scope.applications,link)){
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
