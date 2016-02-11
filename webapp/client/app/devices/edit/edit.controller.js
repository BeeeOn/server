'use strict';

angular.module('beeeOnWebApp')
  .controller('EditDeviceCtrl',[ '$scope','$translate','$log','$stateParams','$state','$timeout','SidePanel','Gateways','lodash', function ($scope,$translate,$log,$stateParams,$state,$timeout,SidePanel,Gateways,lodash) {
    var rightPanelOptions = SidePanel.getRightPanelOptions();
    SidePanel.openRight();
    //set right panel options
    //panel close action
    rightPanelOptions.location = '';
    rightPanelOptions.changeStateOptions.newState = $state.current.name.split('.')[0] + '.detail';
    rightPanelOptions.type = 'cancel_and_return';
    rightPanelOptions.changeStateOptions.params = {
      gatewayId:$stateParams.gatewayId,
      deviceId:$stateParams.deviceId
    };
    //panel buttons
    var save = function (){
      Gateways.setSelectedInfo(lodash.pick($scope.gatewayInfo,['id','name','utc']))
        .then(function(){
        SidePanel.getRightMainAction()();
      },function(err){
        $log.error('EditCtrl - ' + err);
      });
    };
    rightPanelOptions.more = [
      {
        title : '',
        showTitle: false,
        icon : 'done',
        action : save
      }
    ];
    $translate('GLOBAL.SAVE_CHANGES').then(function(text){
      rightPanelOptions.more[0].title = text;
    });
    Gateways.getSelectedInfo($stateParams.gatewayId).
      then(function(info){
        $scope.gatewayInfo = angular.copy(info);
      },function(err){
        $log.error('EditCtrl - '+ err);
      });
  }]);
