'use strict';

describe('Controller: LoadingModalCtrl', function () {

  // load the controller's module
  beforeEach(module('beeeOnWebApp'));

  var LoadingModalCtrl, scope;

  // Initialize the controller and a mock scope
  beforeEach(inject(function ($controller, $rootScope) {
    scope = $rootScope.$new();
    LoadingModalCtrl = $controller('LoadingModalCtrl', {
      $scope: scope
    });
  }));

  it('should ...', function () {
    expect(1).toEqual(1);
  });
});
