'use strict';

angular.module('beeeOnWebApp')
  .factory('Settings',[ '$log','$translate','$window','Modules','lodash','MODULES',function ($log,$translate,$window,Modules,lodash,MODULES) {

    //set timezone setting structure
    var timezone = {
      title : 'SETTINGS.TIMEZONE.TITLE',
      translationPrefix : '',
      selected : 'current',
      options : {
        'current': {id: 'current', name: 'SETTINGS.TIMEZONE.CURRENT'},
        'gateway': {id: 'gateway', name: 'SETTINGS.TIMEZONE.GATEWAY'}
      }
    };
    //ids in language options MUST correspond to translation file name
    //in "assets/translations/" folder
    var languages = {
      title : 'SETTINGS.LANGUAGE.TITLE',
      translationPrefix : '',
      selected : $translate.use() || 'en',
      options : {
        'en' : {id:'en',name:'SETTINGS.LANGUAGE.ENGLISH'},
        'sk' : {id:'sk',name:'SETTINGS.LANGUAGE.SLOVAK'}
      }
    };
    var settingsList = [
      { link : 'timezone',title: 'SETTINGS.TIMEZONE.TITLE',additional:timezone.options[timezone.selected].name},
      { link : 'units',title: 'SETTINGS.UNITS.TITLE'} ,
      { link : 'geolocation',title: 'SETTINGS.GEOLOCATION.TITLE'},
      { link : 'language',title: 'SETTINGS.LANGUAGE.TITLE',additional:'languages.options[languages.selected].name'}
    ];

    var units = {
      "0x01": {
        id: MODULES.ENUM,
        title: '',
        visible: false
      },
      "0x02":{
        id : MODULES.TEMPERATURE,
        title: 'SETTINGS.UNITS.TEMPERATURE.TITLE',
        visible: true
      },
      "0x03":{
        id : MODULES.HUMIDITY,
        title: '',
        visible: false
      },
      "0x04":{
        id : MODULES.PRESSURE,
        title: '',
        visible: false
      },
      "0x05":{
        id : MODULES.LIGHT,
        title: '',
        visible: false
      },
      "0x06":{
        id : MODULES.NOISE,
        title: 'SETTINGS.UNITS.NOISE.TITLE',
        visible: true
      },
      "0x07":{
        id : MODULES.EMISSIONS,
        title: '',
        visible: false
      },
      "0x08":{
        id : MODULES.BATTERY,
        visible: false
      },
      "0x09":{
        id : MODULES.RSSI,
        title: '',
        visible: false
      },
      "0x0A":{
        id : MODULES.REFRESH,
        title: '',
        visible: false
      }
    };
    var debugShowWatchdogList = true;
    // Public API here
    return {
      getUnits: function () {
        return units;
      },
      setUnit : function(unit,option){
        //FIXME implement option choice
      },
      getSettingsList : function () {
        return settingsList;
      },
      getTimezone : function () {
        return timezone;
      },
      setSelectedTimezone : function (option){
        timezone.selected = option;
      },
      getShowWatchdogList : function(){
        return debugShowWatchdogList;
      },
      setShowWatchdogList : function(option){
        debugShowWatchdogList = option;
      },
      getLanguages : function(){
        return languages;
      },
      setSelectedLanguage : function(option){
        languages.selected = option;
        $translate.use(option).then(function(){
          $window.location.reload();
        });
        //
      },
      getSelectedLanguage : function(){
        return languages.selected;
      },
      getUnitFor : function(type){
        var unit = units[type];
        return unit.options[unit.selected].unit;
      }
    };
  }]);
