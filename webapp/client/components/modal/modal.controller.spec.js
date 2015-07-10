'use strict';

describe('Controller: ModalCtrl', function () {

  // load the controller's module
  beforeEach(module('beeeOnWebApp'));

  var ModalCtrl, scope;

  // Initialize the controller and a mock scope
  beforeEach(inject(function ($controller, $rootScope) {
    scope = $rootScope.$new();
    ModalCtrl = $controller('ModalCtrl', {
      $scope: scope
    });
  }));

  it('should ...', function () {
    expect(1).toEqual(1);
  });
});
