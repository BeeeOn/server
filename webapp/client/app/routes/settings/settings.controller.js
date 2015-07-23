'use strict';

angular.module('beeeOnWebApp')
  .controller('SettingsCtrl',['$state','$stateParams','$scope','$log','SidePanel', function ($state,$stateParams,$scope,$log,SidePanel) {
    //TODO collect settings from service
    $scope.settingsList = [
      { link : 'timezone',title: 'Time zone',additional:'Current time zone'},
      { link : 'units',title: 'Units'} ,
      { link : 'geolocation',title: 'Geolocation'}
    ];
    $log.debug($state.current.name);

    $scope.isEmpty = $stateParams.isEmpty;
    $scope.show = function(option){
      $log.debug('Showing settings for: ' + option.link);
      //set right panel title
      $scope.rightPanelOptions.locationOptions.location = option.title;
      $scope.isEmpty = false;
      $state.go("settings."+option.link);
    };

    $scope.rightPanelOptions = {
      lockedMedia: '$mdMedia(\'gt-lg\')',
      locationOptions : {
        location: '',
        type: 'change_state_right',
        changeStateOptions : {
          newState: 'settings',
          params:{

          }
        },
        hideBtn: true
      }
    };

    $scope.toggleRight= function (){
      SidePanel.toggleRight();
    };

  }]);
