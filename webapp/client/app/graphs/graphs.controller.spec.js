'use strict';

describe('Controller: GraphsCtrl', function () {

  // load the controller's module
  beforeEach(module('beeeOnWebApp'));

  var GraphsCtrl, scope;

  // Initialize the controller and a mock scope
  beforeEach(inject(function ($controller, $rootScope) {
    scope = $rootScope.$new();
    GraphsCtrl = $controller('GraphsCtrl', {
      $scope: scope
    });
  }));

  it('should ...', function () {
    expect(1).toEqual(1);
  });
});
