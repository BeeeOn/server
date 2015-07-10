'use strict';

angular.module('beeeOnWebApp')
  .controller('NavbarCtrl',[ '$scope', '$location', '$stateParams', '$log', function ($scope, $location,$stateParams,$log) {

    $scope.locationName = $stateParams.locationName;
    $scope.locationButtonType = $stateParams.locationButtonType;

    $scope.isCollapsed = true;

    $scope.isActive = function(route) {
      return route === $location.path();
    };
  }]);