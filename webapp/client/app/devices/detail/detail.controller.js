'use strict';

angular.module('beeeOnWebApp')
  .controller('DeviceDetailCtrl',[ '$scope','$translate','$log','$stateParams','$state','SidePanel','Gateways','Locations','Devices','lodash','dialogs', function ($scope,$translate,$log,$stateParams,$state,SidePanel,Gateways,Locations,Devices,lodash,dialogs) {
    var rightPanelOptions = SidePanel.getRightPanelOptions();
    //set right panel options
    //panel close action
    //if there are no gateways, don't show add module button
    Gateways.getGateways()
      .then(function(data){
        Gateways.getLocationsForGateway($stateParams.gatewayId).then(function(locations){
          Locations.setCurrentLocations(locations);
          $scope.locations = Locations.getCurrentLocations();
        });
        Gateways.getDevicesForGateway($stateParams.gatewayId).then(function(devices){
          Devices.setCurrentDevices(devices);
          //get device info
          $scope.device = Devices.getDeviceById($stateParams.deviceId);
          //set header
          if(!$scope.device.name || $scope.device.name == ""){
            //if device doesn't have own name, use default (need to be translated)
            var deviceNameTranslation = 'DEVICES.' + Devices.getDefaultNameByType($scope.device.type)
            $translate([deviceNameTranslation]).then(function(translations){
              $scope.device.name = translations[deviceNameTranslation];
              rightPanelOptions.location = $scope.device.name;
            });
          }else {
            rightPanelOptions.location = $scope.device.name;
          }
        });
      })
      .catch(function(err){
        $log.error(err);
      });
    rightPanelOptions.changeStateOptions.newState = $state.current.name.split('.')[0];
    rightPanelOptions.type = 'change_state_right';
    rightPanelOptions.hideBtn = false;
    rightPanelOptions.changeStateOptions.params = {
      gatewayId:$stateParams.gatewayId
    };
    //panel buttons
    var refresh = function (){
      $log.debug("DeviceDetailCtrl - refresh");
    };
    var editDevice = function(){
      $log.debug("DeviceDetailCtrl - Opening edit device for: " + $stateParams.deviceId);
      $state.go($state.current.name.split('.')[0]+'.detail.edit', {deviceId: $stateParams.deviceId},{location:false});
    };
    rightPanelOptions.more = [
      {
        title : '',
        showTitle: false,
        icon : 'refresh',
        action : refresh
      },
      {
        title : 'More',
        showTitle: false,
        icon : 'more_vert',
        hasSubmenu: true,
        submenu : [
          {
            title : 'DEVICES.BTNS.EDIT_DEVICE',
            showTitle: true,
            icon : 'edit',
            action : editDevice
          }
        ]
      }
    ];
    $translate('GLOBAL.SAVE_CHANGES').then(function(text){
      rightPanelOptions.more[0].title = text;
    });

    $scope.showGraph = function () {

    };

    SidePanel.openRight();
    $scope.selectedIndex = 0;
  }]);
