'use strict';

describe('Controller: DeleteCtrl', function () {

  // load the controller's module
  beforeEach(module('beeeOnWebApp'));

  var DeleteCtrl, scope;

  // Initialize the controller and a mock scope
  beforeEach(inject(function ($controller, $rootScope) {
    scope = $rootScope.$new();
    DeleteCtrl = $controller('DeleteCtrl', {
      $scope: scope
    });
  }));

  it('should ...', function () {
    expect(1).toEqual(1);
  });
});
