'use strict';

angular.module('beeeOnWebApp')
  .controller('EditCtrl',[ '$scope','$translate','$log','$stateParams','SidePanel','Gateways','lodash', function ($scope,$translate,$log,$stateParams,SidePanel,Gateways,lodash) {
    var rightPanelOptions = SidePanel.getRightPanelOptions();
    //set right panel options
    //panel close action
    rightPanelOptions.location = '';
    rightPanelOptions.changeStateOptions.newState = 'active-gateway';
    rightPanelOptions.type = 'cancel_and_return';
    rightPanelOptions.changeStateOptions.params = {
      gatewayId:$stateParams.gatewayId
    };
    //panel buttons
    var save = function (){
      Gateways.setSelectedInfo(lodash.pick($scope.gatewayInfo,['id','name','timezone']))
        .then(function(){
          $log.debug("EditCtrl - gateway info successfully updated");
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

    SidePanel.toggleRight();
  }]);
