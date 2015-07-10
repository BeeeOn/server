'use strict';

angular.module('beeeOnWebApp')
  .directive('locationButton',['Menu', function (Menu) {
    /**
     * Directive for location button can have two options in attribute type (menu,location).
     *
     * menu = location button will hide/show menu panel. Uses Menu service to determine, if menu is shown/hidden
     * location = location button will return to previous location/state
     */
    return {
      templateUrl: 'components/location-button/location-button.html',
      restrict: 'E',
      scope: {
        type: '@',
        location: '@'
      },
      controller: function($scope){
        //console.log('Location-button-directive='+'type: ' + $scope.type + ' + location: ' + $scope.location);
        $scope.icon = 'menu_stripes';
        $scope.action = angular.noop;
        switch ($scope.type){
          case 'menu_toggle':
            $scope.action = function(){
              Menu.toggle();
            };
            break;
          case 'menu_close':
            $scope.icon = 'arrow_back';
            $scope.action = function(){
              Menu.close();
            };
            break;
        }
      }
    };
  }]);