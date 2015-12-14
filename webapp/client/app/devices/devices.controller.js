'use strict';

angular.module('beeeOnWebApp')
  .controller('DevicesCtrl',
  ['$scope','$translate','$timeout','$state','$stateParams','$log','Gateways','Devices','navbar','loadingModal','dialogs','SidePanel','Locations',
    function ($scope,$translate,$timeout,$state,$stateParams,$log,Gateways,Devices,navbar,loadingModal,dialogs,SidePanel,Locations) {
    $scope.devices = null;
    $scope.addFABisOpen = false;
    $scope.hideOnScroll = false;
    $scope.locations = null;
    $scope.showList = function(){
      return ($scope.devices && ($scope.devices.length > 0));
    };
    //if there are no gateways, don't show add module button
    Gateways.getGateways()
      .then(function(data){
        $scope.showAddModuleBtn =  !angular.equals({},data);
        Gateways.getLocationsForGateway($stateParams.gatewayId).then(function(locations){
          Locations.setCurrentLocations(locations);
          $scope.locations = Locations.getCurrentLocations();
        });
        Gateways.getDevicesForGateway($stateParams.gatewayId).then(function(devices){
          Devices.setCurrentDevices(devices);
          $scope.devices = Devices.getCurrentDevices();
        });
      })
      .catch(function(err){
        $log.error(err);
      });

    $scope.showAddGatewayModal = function(){
      //show about modal
      dialogs.create(
        'components/tutorial/add-gateway/add-gateway.html',
        'AddGatewayCtrl',
        {},
        {
          'keyboard': false,
          'backdrop': true,
          'size': 'lg',
          'copy': false
        }
      );
    };

    $scope.refreshList = function (){
      $translate('DEVICES.LOADING_DEVICES').then(function(text){
        loadingModal.show(text + ' (NOT actually)');
      });
      $timeout(function(){
        loadingModal.hide();
      },3000);
    };



    $scope.toggleRight= function (){
      SidePanel.toggleRight();
    };
    $scope.rightPanelOptions = SidePanel.getRightPanelOptions();

    navbar.setLocation('MENU.DEVICES');
  }]);
