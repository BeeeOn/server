'use strict';

angular.module('beeeOnWebApp')
  .factory('Gateway',['$stateParams','$q','$http','$log', function ($stateParams,$q,$http,$log) {

    var selectedGateway = null;
    var gateways = null;

    /* Public Gateway service API*/
    return {
      getGateways : function(callback){
        var cb = callback || angular.noop;
        var deffered = $q.defer();
        // return local adapters, if exists
        if(gateways){
          $log.debug('MenuCtrl - Getting local gateways');
          deffered.resolve(gateways);
        }else{
          //else get from server
          this.refreshGateways().then(function(){
            $log.debug('MenuCtrl - Getting gateways from server');
            deffered.resolve(gateways);
          },function(err){
            deffered.reject(err);
          });
        }
        return deffered.promise;
      },

      refreshGateways : function() {
        return $http.get('api/v1/gateway').
          success(function(data) {
            gateways = data;
          }).
          error(function(err) {
            //TODO error notification
            $log.error('MenuCtrl - '+err);
          });
      },

      getSelectedId : function(){
        /*if(urlGateway){
          selectedGateway = urlGateway;
        }*/
        if($stateParams.gatewayId){
          selectedGateway = $stateParams.gatewayId;
        }
        return selectedGateway;
      },

      setSelected : function(gatewayId){
        selectedGateway = gatewayId;
      },

      getSelectedInfo : function(gateway,callback){
        var cb = callback || angular.noop;
        var deffered = $q.defer();

        $log.debug('Retrieving gateway info (' + gateway + ')');

        $http.get('api/v1/gateway/' + gateway).
          then(function(data){
            deffered.resolve(data.data);
          },function(err){
            deffered.reject(err);
          });

        return deffered.promise;
      }
    }
  }]);
