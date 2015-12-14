'use strict';

angular.module('beeeOnWebApp')
  .controller(
  'ModulesCtrl',['$scope','$translate','$timeout','$log','$stateParams','$state','Gateways','Locations','Devices','navbar','loadingModal','dialogs','SidePanel',
    function ($scope,$translate,$timeout,$log,$stateParams,$state,Gateways,Locations,Devices,navbar,loadingModal,dialogs,SidePanel) {
      $scope.devices = null;
      $scope.addFABisOpen = false;
      $scope.hideOnScroll = false;
      $scope.showList = function(){
        return ($scope.devices && ($scope.devices.length > 0));
      };
      Gateways.getGateways()
        .then(function(data){
          //if there are no gateways, don't show add module button
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
        $translate('MODULES.LOADING_MODULES').then(function(text){
          loadingModal.show(text + ' (NOT actually)');
        });
        $timeout(function(){
          loadingModal.hide();
        },3000);
      };

      navbar.setLocation('MENU.MODULES');

      $scope.toggleRight= function (){
        SidePanel.toggleRight();
      };
      $scope.rightPanelOptions = SidePanel.getRightPanelOptions();
  }]);
