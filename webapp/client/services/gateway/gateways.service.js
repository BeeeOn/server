'use strict';

angular.module('beeeOnWebApp')
  .factory('Gateways',['$stateParams','$translate','$q','$http','$log','lodash','loadingModal', function ($stateParams,$translate,$q,$http,$log,lodash,loadingModal) {
    /* selected gateway ID*/
    var selectedGateway = null;
    /* short gateways description*/
    var gateways = null;
    /* short gateways description*/
    var gatewaysInfo = [];

    lodash.mixin({
      mergeById: function mergeById(arr, obj, idProp) {
        var index = _.findIndex(arr, function (elem) {
          // double check, since undefined === undefined
          return typeof elem[idProp] !== "undefined" && elem[idProp] === obj[idProp];
        });

        if (index > -1) {
          arr[index] = obj;
        } else {
          arr.push(obj);
        }

        return arr;
      }
    });

    var findGatewayInfoById = function (id) {
      return lodash.find(gatewaysInfo,{'id':id});
    };

    var findGatewayById = function (id) {
      return lodash.find(gateways,{'id':id});
    };

    /* Public Gateway service API*/
    return {
      findGatewayById : findGatewayById
      ,
      getGateways : function(forceGet){
        var deferred = $q.defer();
        // return local adapters, if exists
        if(gateways && !forceGet){
          $log.debug('Gateway - Getting local gateways');
          deferred.resolve(gateways);
        }else{
          //else get from server
          $translate('GLOBAL.LOADING_GATEWAYS').then(function(text){
            loadingModal.show(text);
          });
          this.refreshGateways().then(function(){
            $log.debug('Gateway - Getting gateways from server');
            deferred.resolve(gateways);
          },function(err){
            deferred.reject(err);
          }).finally(function(){
            loadingModal.hide();
          });
        }
        return deferred.promise;
      },

      refreshGateways : function() {
        return $http.get('api/v1/gateway').
          success(function(data) {
            if(gateways){
              //clear existing information, but keep the reference to gateways array
              gateways.length = 0;
              gateways = gateways.concat(data);
            }else{
              gateways = data;
            }
          }).
          error(function(err) {
            $log.error('Gateway - error code: ' + err.errcode);
          });
      },

      getGatewaysCount : function(){
        var count;
        if(gateways){
          count = gateways.length;
        }else{
          count = 0;
        }
        return count;
      },

      getSelectedId : function(){
        if(!selectedGateway && $stateParams.gatewayId){
          selectedGateway = $stateParams.gatewayId;
        }
        return selectedGateway;
      },

      setSelected : function(gatewayId){
        selectedGateway = gatewayId;
      },

      getSelectedInfo : function(gateway,force){
        var deferred = $q.defer();
        $log.debug('Gateway - Retrieving gateway info (' + gateway + ')');
        // return local adapters, if exists
        var info = findGatewayInfoById(gateway);
        if((gatewaysInfo.length !== 0) && info && !force){
          $log.debug('Gateway - Getting local gateway info');
          deferred.resolve(info);
        }else{
          $log.debug('Gateway - Getting gateway info from server');
          $translate('ACTIVE_GATEWAY.LOADING_INFO').then(function(text){
            loadingModal.show(text);
          });
          //else get from server
          $http.get('api/v1/gateway/' + gateway).
            then(function(data){
              lodash.mergeById(gatewaysInfo,data.data,"id");
              deferred.resolve(data.data);
            },function(err){
              deferred.reject(err);
            }).finally(function(){
              loadingModal.hide();
            });
        }
        return deferred.promise;
      },

      setSelectedInfo : function(info){
        var deferred = $q.defer();
        $log.debug('Gateway - trying to update info about gateway (' + info.id + ')');
        $translate('ACTIVE_GATEWAY.UPDATING_GATEWAY').then(function(text){
          loadingModal.show(text);
        });
        $http.put('api/v1/gateway/' + info.id,info).
          then(function(){
            var oldInfo = findGatewayInfoById(info.id);
            oldInfo.name = info.name;
            oldInfo.timezone = info.timezone;
            oldInfo = findGatewayById(info.id);
            oldInfo.name = info.name;
            oldInfo.timezone = info.timezone;
            deferred.resolve();
          },function(err){
            deferred.reject(err);
          }).finally(function(){
            loadingModal.hide();
          });
        return deferred.promise;
      },

      getSelectedRole : function () {
        return lodash.result(lodash.find(gatewaysInfo,{'id':this.getSelectedId()}),'permission');
      },

      getAccounts : function(force) {
        var deferred = $q.defer();
        var id = this.getSelectedId();
        var gateway = findGatewayInfoById(id);
        if((gatewaysInfo.length !== 0) && gateway && gateway.accounts && !force){
          $log.debug('Gateways - getting local accounts');
          deferred.resolve(gateway.accounts)
        }else{
          $log.debug('Gateway - getting accounts from server (' + id + ')');
          $translate('ACTIVE_GATEWAY.LOADING_ACCOUNTS').then(function(text){
            loadingModal.show(text);
          });
          $http.get('api/v1/gateway/' + id + '/accounts').
            then(function(res){
              if(gateway.accounts){
                gateway.accounts.length = 0;
                gateway.accounts = gateway.accounts.concat(res.data);
              }else {
                gateway.accounts = [];
                gateway.accounts = res.data;
              }
              deferred.resolve(gateway.accounts);
            },function(err){
              deferred.reject(err);
            }).finally(function(){
              loadingModal.hide();
            });
        }
        return deferred.promise;
      },

      isSuperuser : function () {
        return lodash.result(lodash.find(gateways,{'id':selectedGateway}),'permission') === 'owner';
      },

      addAccount : function(account) {
        //TODO store accounts info
        var deferred = $q.defer();
        var id = this.getSelectedId();
        $log.debug('Gateway - trying add account for (' + id + ')');
        $translate('ACTIVE_GATEWAY.ADDING_ACCOUNT').then(function(text){
          loadingModal.show(text);
        });
        $http.post('api/v1/gateway/' + id + '/accounts',account).
          then(function(){
            deferred.resolve();
          },function(err){
            deferred.reject(err);
          }).finally(function(){
            loadingModal.hide();
          });
        return deferred.promise;
      },

      deleteGateway : function (gateway) {
        var deferred = $q.defer();
        var id = this.getSelectedId();
        $log.debug('Gateway - trying add account for (' + id + ')');
        $translate('ACTIVE_GATEWAY.DELETING_GATEWAY').then(function(text){
          loadingModal.show(text);
        });
        $http.delete('api/v1/gateway/' + id).
          then(function(){
            lodash.remove(gateways,function(g){
              return g.id === id;
            });
            lodash.remove(gatewaysInfo,function(g){
              return g.id === id;
            });
            $log.debug(gatewaysInfo);
            deferred.resolve();
          },function(err){
            deferred.reject(err);
          }).finally(function(){
            loadingModal.hide();
          });
        return deferred.promise;
      },

      getLocationsForGateway : function (id){
        var deferred = $q.defer();
        var gateway = findGatewayById(id);
        if(gateway && gateway.locations){
          $log.debug('Gateways - getting local locations');
          deferred.resolve(gateway.locations)
        }else{
          $log.debug('Gateway - getting locations from server (' + id + ')');
          $translate('DEVICES.LOADING_LOCATIONS').then(function(text){
            loadingModal.show(text);
          });
          $http.get('api/v1/gateway/' + id + '/locations').
            then(function(res){
              //add default "Not Defined Location" to beginning of list
              res.data.push({
                "id" : "0",
                "name" : "",
                "type" : "0"
              });
              if(gateway.locations){
                gateway.locations.length = 0;
                gateway.locations = gateway.locations.concat(res.data);
              }else {
                gateway.locations = [];
                gateway.locations = res.data;
              }
              deferred.resolve(gateway.locations);
            },function(err){
              deferred.reject(err);
            }).finally(function(){
              loadingModal.hide();
            });
        }
        return deferred.promise;
      },
      getDevicesForGateway : function (id){
        var deferred = $q.defer();
        var gateway = findGatewayById(id);
        if(gateway && gateway.devices){
          $log.debug('Gateways - getting local devices');
          deferred.resolve(gateway.devices)
        }else{
          $log.debug('Gateway - getting devices from server (' + id + ')');
          $translate('DEVICES.LOADING_DEVICES').then(function(text){
            loadingModal.show(text);
          });
          $http.get('api/v1/gateway/' + id + '/devices').
            then(function(res){
              if(gateway.devices){
                gateway.devices.length = 0;
                gateway.devices = gateway.devices.concat(res.data);
              }else {
                gateway.devices = [];
                gateway.devices = res.data;
              }
              deferred.resolve(gateway.devices);
            },function(err){
              deferred.reject(err);
            }).finally(function(){
              loadingModal.hide();
            });
        }
        return deferred.promise;
      },
      clearLocalData : function(){
        $log.debug("Clearing local gateways data");
        gateways = null;
        gatewaysInfo = [];
        selectedGateway = null;
      },
      registerGateway : function(options){
        var deferred = $q.defer();
        $log.debug('Gateway - trying to add gateway (' + options.id + ')');
        $http.post('api/v1/gateway',options).
          then(function(){
            $log.debug("Gateway registered successfully (" + options.id + ")");
            deferred.resolve();
          },function(err){
            deferred.reject(err);
          });
        return deferred.promise;
      }
    }
  }]);
