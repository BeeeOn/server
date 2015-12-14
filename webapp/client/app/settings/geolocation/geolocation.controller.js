'use strict';

angular.module('beeeOnWebApp')
  .controller('GeolocationCtrl',['$scope','$translate','SidePanel', function ($scope,$translate,SidePanel) {
    $scope.rightPanelOptions = SidePanel.getRightPanelOptions();
    $translate('SETTINGS.GEOLOCATION.TITLE').then(function(geolocation){
      $scope.rightPanelOptions.location = geolocation;
    });

    SidePanel.toggleRight();
  }]);
