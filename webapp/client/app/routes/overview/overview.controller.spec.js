'use strict';

describe('Controller: OverviewCtrl', function () {

  // load the controller's module
  beforeEach(module('beeeOnWebApp'));

  var OverviewCtrl, scope;

  // Initialize the controller and a mock scope
  beforeEach(inject(function ($controller, $rootScope) {
    scope = $rootScope.$new();
    OverviewCtrl = $controller('OverviewCtrl', {
      $scope: scope
    });
  }));

  it('should ...', function () {
    expect(1).toEqual(1);
  });
});
