'use strict';

angular.module('beeeOnWebApp')
  .factory('Settings',[ '$log','$translate','$window',function ($log,$translate,$window) {
    var settingsList = [
      { link : 'timezone',title: '',additional:''},
      { link : 'units',title: ''} ,
      { link : 'geolocation',title: ''},
      { link : 'language',title: '',additional:''}
    ];
    $translate(['SETTINGS.TIMEZONE.TITLE','SETTINGS.UNITS.TITLE','SETTINGS.GEOLOCATION.TITLE','SETTINGS.LANGUAGE.TITLE']).then(function(translations){
      settingsList[0].title = translations['SETTINGS.TIMEZONE.TITLE'];
      timezone.title = translations['SETTINGS.TIMEZONE.TITLE'];
      settingsList[1].title = translations['SETTINGS.UNITS.TITLE'];
      settingsList[2].title = translations['SETTINGS.GEOLOCATION.TITLE'];
      settingsList[3].title = translations['SETTINGS.LANGUAGE.TITLE'];
      languages.title = translations['SETTINGS.LANGUAGE.TITLE'];
    });
    //set timezone setting structure
    var timezone = {
      title : '',
      selected : 'current',
      options : {
        'current': {id: 'current', name: ''},
        'gateway': {id: 'gateway', name: ''}
      }
    };
    //load translation
    $translate(['SETTINGS.TIMEZONE.TITLE','SETTINGS.TIMEZONE.CURRENT','SETTINGS.TIMEZONE.GATEWAY']).then(function(translations){
      timezone.options['current'].name = translations['SETTINGS.TIMEZONE.CURRENT'];
      timezone.options['gateway'].name = translations['SETTINGS.TIMEZONE.GATEWAY'];
      //show selected timezone in settings list
      settingsList[0].additional = timezone.options[timezone.selected].name;
    });
    //ids in language options MUST correspond to translation file name
    //in "assets/translations/" folder
    var languages = {
      title : '',
      selected : $translate.use() || 'en',
      options : {
        'en' : {id:'en',name:''},
        'sk' : {id:'sk',name:''}
      }
    };
    //load translation
    $translate(['SETTINGS.LANGUAGE.ENGLISH','SETTINGS.LANGUAGE.SLOVAK']).then(function(translations){
      languages.options['en'].name = translations['SETTINGS.LANGUAGE.ENGLISH'];
      languages.options['sk'].name = translations['SETTINGS.LANGUAGE.SLOVAK'];
      //show selected language in settings list
      settingsList[3].additional = languages.options[languages.selected].name;
    });
    var units = [
      {
        id : 'temperature',
        title: '',
        selected : 'celsius',
        options : {
          'celsius': {id: 'celsius', name: '', unit: '°C'},
          'fahrenheit': {id: 'fahrenheit', name: '', unit: '°F'},
          'kelvin': {id: 'kelvin', name: 'Kelvin', unit: 'K'}
        }
      },
      {
        id : 'noise',
        title: '',
        selected : 'decibel',
        options : {
          'decibel': {id: 'decibel',name: 'Decibel', unit: 'dB'},
          'bel': {id: 'bel',name: 'Bel', unit: 'B'},
          'neper': {id: 'neper',name: 'Neper', unit: 'Np'}
        }
      }
    ];
    $translate([
      'SETTINGS.UNITS.TEMPERATURE.TITLE',
      'SETTINGS.UNITS.NOISE.TITLE',
      'SETTINGS.UNITS.TEMPERATURE.CELSIUS',
      'SETTINGS.UNITS.TEMPERATURE.FAHRENHEIT',
      'SETTINGS.UNITS.TEMPERATURE.KELVIN'
    ])
      .then(function(translations){
        //temperature translation
        units[0].title = translations['SETTINGS.UNITS.TEMPERATURE.TITLE'];
        units[0].options['celsius'].name = translations['SETTINGS.UNITS.TEMPERATURE.CELSIUS'];
        units[0].options['fahrenheit'].name = translations['SETTINGS.UNITS.TEMPERATURE.FAHRENHEIT'];
        units[0].options['kelvin'].name = translations['SETTINGS.UNITS.TEMPERATURE.KELVIN'];
        //noise translation
        units[1].title = translations['SETTINGS.UNITS.NOISE.TITLE'];
    });
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
      }
    };
  }]);
