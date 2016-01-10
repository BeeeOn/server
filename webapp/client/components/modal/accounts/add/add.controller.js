'use strict';

angular.module('beeeOnWebApp')
  .controller('AddAccountCtrl',['$scope','$modalInstance','$translate','Auth','data', function ($scope,$modalInstance,$translate,Auth,data) {

    var actions = {
      'add' : {
        title : ''
      },
      'edit' : {
        title : ''
      }
    };
    $translate(['GLOBAL.ADD','GLOBAL.EDIT']).then(function(translations){
      actions['add'].title = translations['GLOBAL.ADD'];
      actions['edit'].title = translations['GLOBAL.EDIT'];
      $scope.action = actions[data.action].title;
    });
    $scope.roles = Auth.getRoles();
    $scope.account = data.account;

    $scope.add = function(){
      if($scope.account.email===''){
        $log.error("AddAccountCtrl - email must be filled");
      }else{
        $modalInstance.close($scope.account);
      }
    }
  }]);
