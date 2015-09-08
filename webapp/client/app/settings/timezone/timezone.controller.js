'use strict';

angular.module('beeeOnWebApp')
  .controller('TimezoneCtrl',['$scope','$translate','SidePanel', function ($scope,$translate,SidePanel) {
    $scope.rightPanelOptions = SidePanel.getRightPanelOptions();
    $translate('SETTINGS.TIMEZONE.TITLE').then(function(timezone){
      $scope.rightPanelOptions.locationOptions.location = timezone;
    });
    SidePanel.toggleRight();
  }]);
