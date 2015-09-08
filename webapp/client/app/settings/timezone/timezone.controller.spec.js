'use strict';

describe('Controller: TimezoneCtrl', function () {

  // load the controller's module
  beforeEach(module('beeeOnWebApp'));

  var TimezoneCtrl, scope;

  // Initialize the controller and a mock scope
  beforeEach(inject(function ($controller, $rootScope) {
    scope = $rootScope.$new();
    TimezoneCtrl = $controller('TimezoneCtrl', {
      $scope: scope
    });
  }));

  it('should ...', function () {
    expect(1).toEqual(1);
  });
});
