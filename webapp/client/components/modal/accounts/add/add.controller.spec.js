'use strict';

describe('Controller: AddCtrl', function () {

  // load the controller's module
  beforeEach(module('beeeOnWebApp'));

  var AddCtrl, scope;

  // Initialize the controller and a mock scope
  beforeEach(inject(function ($controller, $rootScope) {
    scope = $rootScope.$new();
    AddCtrl = $controller('AddCtrl', {
      $scope: scope
    });
  }));

  it('should ...', function () {
    expect(1).toEqual(1);
  });
});
