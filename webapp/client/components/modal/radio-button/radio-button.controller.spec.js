'use strict';

describe('Controller: RadioButtonCtrl', function () {

  // load the controller's module
  beforeEach(module('beeeOnWebApp'));

  var RadioButtonCtrl, scope;

  // Initialize the controller and a mock scope
  beforeEach(inject(function ($controller, $rootScope) {
    scope = $rootScope.$new();
    RadioButtonCtrl = $controller('RadioButtonCtrl', {
      $scope: scope
    });
  }));

  it('should ...', function () {
    expect(1).toEqual(1);
  });
});
