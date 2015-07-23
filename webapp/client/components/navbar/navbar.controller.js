'use strict';

angular.module('beeeOnWebApp')
  .controller('NavbarCtrl',[ '$scope', '$location', '$stateParams', '$log','SidePanel', function ($scope, $location,$stateParams,$log,SidePanel) {

    $scope.locationOptions = {
      location: $stateParams.locationName,
      type: $stateParams.locationButtonType
    };

    $scope.isCollapsed = true;

    $scope.isActive = function(route) {
      return route === $location.path();
    };

    $scope.showMenu = function(){
      SidePanel.toggleLeft();
    };
  }]);