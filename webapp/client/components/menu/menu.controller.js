'use strict';

angular.module('beeeOnWebApp')
  .controller('MenuCtrl', [ '$scope','$log','User','clr_primary','clr_accent',function ($scope,$log,User,clr_primary,clr_accent) {

    $scope.clr_primary = clr_primary;
    $scope.clr_accent = clr_accent;

    $scope.userPanel = {};
    $scope.user = User.get();

  }]);
