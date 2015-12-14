'use strict';

angular.module('beeeOnWebApp')
  .controller(
  'UsersCtrl',['$scope','$translate','$log','$stateParams','$mdBottomSheet','SidePanel','Gateways','dialogs',
    function ($scope,$translate,$log,$stateParams,$mdBottomSheet,SidePanel,Gateways,dialogs) {
    var rightPanelOptions = SidePanel.getRightPanelOptions();
    //set right panel options
    //panel close action
    $translate('ACTIVE_GATEWAY.GATEWAY_USERS').then(function(text){
      rightPanelOptions.location = text;
    });
    rightPanelOptions.changeStateOptions.newState = 'active-gateway';
    rightPanelOptions.type = 'change_state_right';
    rightPanelOptions.changeStateOptions.params = {
      gatewayId:$stateParams.gatewayId
    };

    rightPanelOptions.more = [];
    //set other buttons
    $scope.showUser = function(){

    };

    //get gateway accounts
    Gateways.getAccounts().
      then(function(data){
        $scope.accounts = data;
      },function(err){
        $log.error(err);
      });
    //is user superuser on this gateway?
    $scope.isSuperuser = Gateways.isSuperuser;

    $scope.showGridBottomSheet = function($event) {
      $scope.alert = '';
      $mdBottomSheet.show({
        templateUrl: 'components/user/add-user-bottom-sheet/add-user-bottom-sheet.html',
        controller: 'AddUserBottomSheetCtrl',
        targetEvent: $event
      }).then(function(clickedItem) {

      });
    };

    $scope.showAddModal = function () {
      dialogs.create(
        'components/modal/accounts/add/add.html',
        'AddAccountCtrl',
        {
          action: 'add',
          account: {
            email: '',
            role : 'guest'
          }
        },
        {
          'keyboard': false,
          'backdrop': true,
          'size': 'sm',
          'windowClass': 'addAccountModal',
          'copy': true
        }
      ).result.then(function(account){
        Gateways.addAccount(account).
          then(function () {
            $log.debug('UsersCtrl - account added successfully')
          }, function (err) {
            $log.error('UsersCtrl - ' + err);
          })
        });
    };

    SidePanel.toggleRight();
  }]);
