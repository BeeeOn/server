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
      return lodash.find(gatewaysInfo,{'aid':id});
    };

    var findGatewayById = function (id) {
      return lodash.find(gateways,{'aid':id});
    };

    /* Public Gateway service API*/
    return {
      findGatewayById : findGatewayById
      ,
      getGateways : function(){
        var deferred = $q.defer();
        // return local adapters, if exists
        if(gateways){
          $log.debug('Gateway - Getting local gateways');
          deferred.resolve(gateways);
        }else{
          //else get from server
          $translate('OVERVIEW.LOADING_GATEWAYS').then(function(text){
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
            gateways = data;
          }).
          error(function(err) {
            //TODO error notification
            $log.error('Gateway - '+err);
          });
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

      getSelectedInfo : function(gateway){
        var deferred = $q.defer();
        $log.debug('Gateway - Retrieving gateway info (' + gateway + ')');
        // return local adapters, if exists
        var info = findGatewayInfoById(gateway);
        if((gatewaysInfo.length !== 0) && info){
          $log.debug('Gateway - Getting local gateway info');
          deferred.resolve(info);
        }else{
          $log.debug('Gateway - Getting gateway info from server');
          $translate('OVERVIEW.DESCRIPTION.LOADING_INFO').then(function(text){
            loadingModal.show(text);
          });
          //else get from server
          $http.get('api/v1/gateway/' + gateway).
            then(function(data){
              lodash.mergeById(gatewaysInfo,data.data,"aid");
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
        $log.debug('Gateway - trying to update info about gateway (' + info.aid + ')');
        $http.put('api/v1/gateway/' + info.aid,info).
          then(function(){
            var oldInfo = findGatewayInfoById(info.aid);
            oldInfo.aname = info.aname;
            oldInfo.utc = info.utc;
            oldInfo = findGatewayById(info.aid);
            oldInfo.aname = info.aname;
            oldInfo.utc = info.utc;
            deferred.resolve();
          },function(err){
            deferred.reject(err);
          });
        return deferred.promise;
      },

      getSelectedRole : function () {
        return lodash.result(lodash.find(gatewaysInfo,{'aid':this.getSelectedId()}),'role');
      },

      getAccounts : function() {
        var deferred = $q.defer();
        var id = this.getSelectedId();
        var gateway = findGatewayInfoById(id);
        if((gatewaysInfo.length !== 0) && gateway && gateway.accounts){
          $log.debug('Gateways - getting local accounts');
          deferred.resolve(gateway.accounts)
        }else{
          $log.debug('Gateway - getting accounts from server (' + id + ')');
          $translate('OVERVIEW.DESCRIPTION.LOADING_ACCOUNTS').then(function(text){
            loadingModal.show(text);
          });
          $http.get('api/v1/gateway/' + id + '/accounts').
            then(function(res){
              gateway.accounts = [];
              gateway.accounts = res.data;
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
        return lodash.result(lodash.find(gateways,{'aid':selectedGateway}),'role') === 'superuser';
      },

      addAccount : function(account) {
        //TODO store accounts info
        var deferred = $q.defer();
        var id = this.getSelectedId();
        $log.debug('Gateway - trying add account for (' + id + ')');
        $http.post('api/v1/gateway/' + id + '/accounts',account).
          then(function(){
            account.name = account.email;
            findGatewayInfoById(id).accounts.push(account);
            deferred.resolve();
          },function(err){
            deferred.reject(err);
          });
        return deferred.promise;
      },

      deleteGateway : function (gateway) {
        var deferred = $q.defer();
        var id = this.getSelectedId();
        $log.debug('Gateway - trying add account for (' + id + ')');
        $http.delete('api/v1/gateway/' + id).
          then(function(){
            lodash.remove(gateways,function(g){
              return g.aid === id;
            });
            lodash.remove(gatewaysInfo,function(g){
              return g.aid === id;
            });
            $log.debug(gatewaysInfo);
            deferred.resolve();
          },function(err){
            deferred.reject(err);
          });
        return deferred.promise;
      }
    }
  }]);
