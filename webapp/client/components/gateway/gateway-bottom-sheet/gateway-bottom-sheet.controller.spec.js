'use strict';

describe('Controller: GatewayBottomSheetCtrl', function () {

  // load the controller's module
  beforeEach(module('beeeOnWebApp'));

  var GatewayBottomSheetCtrl, scope;

  // Initialize the controller and a mock scope
  beforeEach(inject(function ($controller, $rootScope) {
    scope = $rootScope.$new();
    GatewayBottomSheetCtrl = $controller('GatewayBottomSheetCtrl', {
      $scope: scope
    });
  }));

  it('should ...', function () {
    expect(1).toEqual(1);
  });
});
