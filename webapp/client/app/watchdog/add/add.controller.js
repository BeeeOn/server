'use strict';

angular.module('beeeOnWebApp')
  .controller('AddRuleCtrl',[ '$scope','$translate','$stateParams','$log','SidePanel', function ($scope,$translate,$stateParams,$log,SidePanel) {

    var rightPanelOptions = SidePanel.getRightPanelOptions();
    $translate('WATCHDOG.WATCHDOG_RULE').then(function(text){
      rightPanelOptions.location = text;
    });
    rightPanelOptions.changeStateOptions.newState = 'watchdog';
    rightPanelOptions.type = 'cancel_and_return';
    rightPanelOptions.changeStateOptions.params = {
      gatewayId:$stateParams.gatewayId
    };
    //panel buttons
    var save = function (){
      $log.warn('SAVING RULE');
    };
    rightPanelOptions.more = [
      {
        title : '',
        showTitle: true,
        icon : 'done',
        action : save
      }
    ];
    $translate('GLOBAL.SAVE_CHANGES').then(function(text){
      rightPanelOptions.more[0].title = text;
    });
    //sn = send notification, cs = change actuator state
    $scope.thenActions = [
      {
        action : 'sn',
        title : ''
      },
      {
        action : 'cs',
        title : ''
      }
    ];
    $translate(['WATCHDOG.SEND_NOTIFICATION','WATCHDOG.CHANGE_ACTUATOR']).then(function(translations){
      $scope.thenActions[0].title = translations['WATCHDOG.SEND_NOTIFICATION'];
      $scope.thenActions[1].title = translations['WATCHDOG.CHANGE_ACTUATOR'];
    });
    $scope.devices = [
      {
        name: 'Awesome temperature',
        room : 'Wherever',
        icon : 'b_val_temperature'
      },
      {
        name: 'Awesome humidity',
        room : 'Wherever',
        icon : 'b_val_temperature'
      }
    ];
    $scope.conditions = {
      'more': {
        id: 'more',
        icon: 'arrow_left',
        label: ''
      },
      'less' : {
        id: 'less',
        icon : 'arrow_right',
        label : ''
      }
    };
    $translate(['WATCHDOG.MORE_THEN','WATCHDOG.LESS_THEN']).then(function(translations){
      $scope.conditions['more'].label = translations['WATCHDOG.MORE_THEN'];
      $scope.conditions['less'].label = translations['WATCHDOG.LESS_THEN'];
    });
    $scope.selectedCondition = $scope.conditions['less'].id;
    $scope.selectedAction = 'sn';

    SidePanel.toggleRight();
  }]);
