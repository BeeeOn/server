'use strict';

angular.module('beeeOnWebApp')
  .controller('AboutCtrl', [ '$scope','$modalInstance','version',
    function($scope,$modalInstance,version) {
      $scope.version = version;
      $scope.close = function(){
        $modalInstance.close();
      };
  }]);
