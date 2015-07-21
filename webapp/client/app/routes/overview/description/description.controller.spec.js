'use strict';

describe('Controller: DescriptionCtrl', function () {

  // load the controller's module
  beforeEach(module('beeeOnWebApp'));

  var DescriptionCtrl, scope;

  // Initialize the controller and a mock scope
  beforeEach(inject(function ($controller, $rootScope) {
    scope = $rootScope.$new();
    DescriptionCtrl = $controller('DescriptionCtrl', {
      $scope: scope
    });
  }));

  it('should ...', function () {
    expect(1).toEqual(1);
  });
});
