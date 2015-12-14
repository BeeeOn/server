'use strict';

describe('Controller: AddGatewayCtrl', function () {

  // load the controller's module
  beforeEach(module('beeeOnWebApp'));

  var AddGatewayCtrl, scope;

  // Initialize the controller and a mock scope
  beforeEach(inject(function ($controller, $rootScope) {
    scope = $rootScope.$new();
    AddGatewayCtrl = $controller('AddGatewayCtrl', {
      $scope: scope
    });
  }));

  it('should ...', function () {
    expect(1).toEqual(1);
  });
});
