'use strict';

angular.module('beeeOnWebApp')
  .controller(
  'UsersCtrl',['$scope','$translate','$log','$stateParams','$mdBottomSheet','SidePanel','Gateways','dialogs','Auth',
    function ($scope,$translate,$log,$stateParams,$mdBottomSheet,SidePanel,Gateways,dialogs,Auth) {
    var rightPanelOptions = SidePanel.getRightPanelOptions();
    //set right panel options
    //panel close action
    $translate('ACTIVE_GATEWAY.GATEWAY_USERS').then(function(text){
      rightPanelOptions.location = text;
    });
    rightPanelOptions.changeStateOptions.newState = 'active-gateway';
    rightPanelOptions.type = 'change_state_right';
    rightPanelOptions.changeStateOptions.params = {
      gatewayId:$stateParams.gatewayId,
      reloadData : false
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
            permission : 'guest'
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
            if(Auth.getCurrentUser().name === 'Demo'){
              $log.warn("Cannot add accounts in demo mod");
            }else{
              rightPanelOptions.changeStateOptions.params.reloadData = true;
              $log.debug('UsersCtrl - account added successfully');
              Gateways.getAccounts(true).
                then(function(data){
                  $scope.accounts = data;
                },function(err){
                  $log.error(err);
                });
            }
          }, function (err) {
            $log.error(err);
          })
        });
    };

    SidePanel.toggleRight();
  }]);
