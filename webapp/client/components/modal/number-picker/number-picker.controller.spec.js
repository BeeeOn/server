'use strict';

describe('Controller: NumberPickerCtrl', function () {

  // load the controller's module
  beforeEach(module('beeeOnWebApp'));

  var NumberPickerCtrl, scope;

  // Initialize the controller and a mock scope
  beforeEach(inject(function ($controller, $rootScope) {
    scope = $rootScope.$new();
    NumberPickerCtrl = $controller('NumberPickerCtrl', {
      $scope: scope
    });
  }));

  it('should ...', function () {
    expect(1).toEqual(1);
  });
});
