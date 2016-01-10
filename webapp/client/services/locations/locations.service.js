'use strict';

angular.module('beeeOnWebApp')
  .factory('Locations',['$log','$stateParams','$translate','Gateways','lodash', function ($log,$stateParams,$translate,Gateways,lodash) {

    var locationDefaults = {
      0 : { "icon" : "b_loc_unknown",name:"LOCATIONS.UNKNOWN"},
      1 : { "icon" : "b_loc_bathroom",name:"LOCATIONS.BATHROOM"},
      2 : { "icon" : "b_loc_bedroom",name:"LOCATIONS.BEDROOM"},
      3 : { "icon" : "b_loc_garden",name:"LOCATIONS.GARDEN"},
      4 : { "icon" : "b_loc_dining_room",name:"LOCATIONS.DINING_ROOM"},
      5 : { "icon" : "b_loc_living_room",name:"LOCATIONS.LIVING_ROOM"},
      6 : { "icon" : "b_loc_wc",name:"LOCATIONS.WC"}
    };

    var currentLocations = null;

    return {
      getCurrentLocations : function (){
        return currentLocations;
      },
      setCurrentLocations : function (locations) {
        currentLocations = locations;
      },
      getIconByType : function (type){
        return locationDefaults[type].icon;
      },
      getIconById : function(id){
        if(currentLocations == null) return;
        return locationDefaults[lodash.find(currentLocations,{'id':id.toString()}).type].icon;
      },
      getNameById : function(id){
        if(currentLocations == null) return "";
        var location = lodash.find(currentLocations,{'id':id.toString()});
        return (location.name && location.name !== "") ? location.name : $translate.instant(locationDefaults[location.type].name);
      },
      clearLocalData : function () {
        $log.debug("Clearing local locations data");
        currentLocations = null;
      }
    }
  }]);
