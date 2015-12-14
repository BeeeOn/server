'use strict';

angular.module('beeeOnWebApp')
  .controller('NavbarCtrl',[ '$scope', '$location', '$stateParams', '$log','SidePanel','navbar', function ($scope, $location,$stateParams,$log,SidePanel,navbar) {

    $scope.locationOptions = {
      location: '',
      type: $stateParams.locationButtonType,
      hideBtn : true
    };
    navbar.getLocation().then(function(loc){
      $scope.locationOptions.location = loc;
    });

    $scope.isCollapsed = true;

    $scope.isActive = function(route) {
      return route === $location.path();
    };

    $scope.showMenu = function(){
      SidePanel.toggleLeft();
    };
  }]);