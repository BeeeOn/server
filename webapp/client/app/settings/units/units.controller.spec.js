'use strict';

describe('Controller: UnitsCtrl', function () {

  // load the controller's module
  beforeEach(module('beeeOnWebApp'));

  var UnitsCtrl, scope;

  // Initialize the controller and a mock scope
  beforeEach(inject(function ($controller, $rootScope) {
    scope = $rootScope.$new();
    UnitsCtrl = $controller('UnitsCtrl', {
      $scope: scope
    });
  }));

  it('should ...', function () {
    expect(1).toEqual(1);
  });
});
