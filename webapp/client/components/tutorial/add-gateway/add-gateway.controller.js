'use strict';

angular.module('beeeOnWebApp')
  .controller('AddGatewayCtrl', ['$scope','$state','$log','$modalInstance','Gateways','Auth','dialogs','lodash', function ($scope,$state,$log,$modalInstance,Gateways,Auth,dialogs,lodash) {
    $scope.currentIndex = 0;
    $scope.close = function(){
      $modalInstance.close();
    };
    $scope.goToNext = function(){
      $scope.currentIndex++;
    };
    $scope.skipTutorial = function() {
      $scope.currentIndex = 2;
    };
    $scope.onSwipeLeft = function(){
      if($scope.currentIndex > 0 && $scope.currentIndex < 2){
        $scope.currentIndex++;
      }
    };
    $scope.showAddModal = function(){
      dialogs.create(
        'components/modal/gateway/add/add.html',
        'AddGatewayModalCtrl',
        'lg'
      ).result.then(function(gateId){
          //close the tutorial
          addGateway(gateId);
          //try to add gateway
        });
    };

    var addGateway = function(id){
      Gateways.registerGateway({id:id,name:composeNewGatewayName(),timezone:0})
        .then(function(data){
          $log.debug("AddGatewayCtrl - gateway added successfully.Refreshing gateways");
          $modalInstance.close();
          Gateways.getGateways(true).then(function(gateways){
            $state.go('devices', {gatewayId:lodash.last(gateways).id,message:"Gateway successfully added"});
          });
        })
        .catch(function(err){
          $log.error(err);
        });
    };

    var composeNewGatewayName = function(){
      return Auth.getCurrentUser().name+'\'s gateway #' + (Gateways.getGatewaysCount()+1);
    };
  }]);
