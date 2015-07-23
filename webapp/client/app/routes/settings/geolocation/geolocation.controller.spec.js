'use strict';

describe('Controller: GeolocationCtrl', function () {

  // load the controller's module
  beforeEach(module('beeeOnWebApp'));

  var GeolocationCtrl, scope;

  // Initialize the controller and a mock scope
  beforeEach(inject(function ($controller, $rootScope) {
    scope = $rootScope.$new();
    GeolocationCtrl = $controller('GeolocationCtrl', {
      $scope: scope
    });
  }));

  it('should ...', function () {
    expect(1).toEqual(1);
  });
});
