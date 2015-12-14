'use strict';

describe('Controller: ActiveGatewayCtrl', function () {

  // load the controller's module
  beforeEach(module('beeeOnWebApp'));

  var ActiveGatewayCtrl, scope;

  // Initialize the controller and a mock scope
  beforeEach(inject(function ($controller, $rootScope) {
    scope = $rootScope.$new();
    ActiveGatewayCtrl = $controller('ActiveGatewayCtrl', {
      $scope: scope
    });
  }));

  it('should ...', function () {
    expect(1).toEqual(1);
  });
});
