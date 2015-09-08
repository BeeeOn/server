'use strict';

angular.module('beeeOnWebApp')
  .controller('DescriptionCtrl',[ '$scope','$state','$stateParams','$log','$mdBottomSheet','$mdComponentRegistry' ,'Gateways','SidePanel','dialogs',
    function ($scope,$state,$stateParams,$log,$mdBottomSheet,$mdComponentRegistry,Gateways,SidePanel,dialogs) {
    //retrieve gateway info
    Gateways.getSelectedInfo($stateParams.gatewayId).then(function(data){
      $scope.gatewayInfo = data;
    },function(err){
      //TODO error handling
      $log.error('DescriptionCtrl - ' + err.data);
    });

    $scope.showDevices = function(){
      $state.go('overview.show',{gatewayId:$scope.gatewayInfo.aid});
    };

    $scope.showRightPanel = function(option){
      var newLocation = 'overview.description.' + option;
      if(newLocation === $state.current.name){
        SidePanel.toggleRight();
      }else{
        $state.go(newLocation, {}, { location: false });
      }
    };
    $scope.showGridBottomSheet = function($event) {
      $mdBottomSheet.show({
        templateUrl: 'components/gateway/gateway-bottom-sheet/gateway-bottom-sheet.html',
        controller: 'GatewayBottomSheetCtrl',
        targetEvent: $event
      }).then(function(clickedItem) {
        if(clickedItem.id === 'delete'){
          $scope.showDeleteModal();
        }else{
          $scope.showRightPanel(clickedItem.id);
        }
      });
    };
    $scope.showDeleteModal = function () {
      dialogs.create(
        'components/modal/gateway/delete/delete.html',
        'DeleteGatewayCtrl',
        {
          name : Gateways.findGatewayById(Gateways.getSelectedId()).aname
        },
        'lg'
      ).result.then(function(){
          Gateways.deleteGateway(Gateways.getSelectedId()).
            then(function(){
              $log.info('Adapter successfully deleted.');
              $state.go('overview');
            },function(err){
              $log.error(err);
            });
        });
    };
  }]);
