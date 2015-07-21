'use strict';

angular.module('beeeOnWebApp')
  .controller('ChangelogCtrl', [ '$scope','$http','$log' , function ($scope,$http,$log) {
    $scope.logs = {};
    $http.get('changelog.json')
      .then(function(res){
        $scope.logs = res.data.logs;
        $log.debug($scope.logs);
      });
  }]);
