'use strict';

angular.module('beeeOnWebApp')
  .controller('SettingsCtrl',['$state','$stateParams','$scope','$log','$translate','navbar','SidePanel','Settings','dialogs', function ($state,$stateParams,$scope,$log,$translate,navbar,SidePanel,Settings,dialogs) {
    //setup right panel
    var options = {
      lockedMedia: '',
      location: '',
      type: 'change_state_right',
      changeStateOptions : {
        newState: 'settings',
        params:{
        }
      },
      more: [],
      hideBtn: false
    };
    SidePanel.setRightPanelOptions(options);
    $scope.rightPanelOptions = SidePanel.getRightPanelOptions();

    $scope.settingsList = Settings.getSettingsList();

    $scope.checkAdditional = function (item){
      if(!item.additional){
        return false;
      }
      switch (item.link){
        case 'timezone':
          item.additional = Settings.getTimezone().options[Settings.getTimezone().selected].name;
          return true;
          break;
        case 'language':
          item.additional = Settings.getLanguages().options[Settings.getLanguages().selected].name;
          return true;
          break;
        default :
          return false;
      }
    };

    $scope.show = function(option){
      switch(option.link){
        case 'timezone':
          $scope.showModal(Settings.getTimezone(),option.link);
          break;
        case 'language':
          $scope.showModal(Settings.getLanguages(),option.link);
          break;
        default :
          $log.debug('SettingsCtrl - Showing settings for: ' + option.link);
          //set right panel title
          var newLocation = "settings."+option.link;
          if(newLocation === $state.current.name){
            SidePanel.toggleRight();
          }else{
            $state.go(newLocation, {}, { location: false });
          }
          break;
      }
    };

    $scope.showModal = function(data,type){
      dialogs.create(
        'components/modal/radio-button/radio-button.html',
        'RadioButtonModalCtrl',
        {
          item : data
        },
        {
          'keyboard': false,
          'backdrop': true,
          'size': 'sm',
          'windowClass': 'radio-button-modal',
          'copy': true
        }
      ).result.then(function(selected){
          switch (type){
            case 'timezone':
              Settings.setSelectedTimezone(selected.id);
              $log.debug('SettingsCtrl - user selected \"' + Settings.getTimezone().options[selected.id].name + '\" as selected timezone');
              break;
            case 'language':
              Settings.setSelectedLanguage(selected.id);
              $log.debug('SettingsCtrl - user selected \"' + Settings.getLanguages().options[selected.id].name + '\" as selected language');
              break;
          }
        });
    };


    $scope.toggleRight= function (){
      SidePanel.toggleRight();
    };
    navbar.setLocation('MENU.SETTINGS');
    /*$translate('MENU.SETTINGS').then(function(text){
      console.log("NAVBAR- location set to : " + text);
      navbar.setLocation(text);
    });*/

  }]);
