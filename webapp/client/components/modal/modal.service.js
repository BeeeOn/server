'use strict';

angular.module('beeeOnWebApp')
  .factory('Modal', function ($rootScope, $modal,$log) {

    /**
     * Opens a modal
     * @param  {Object} modalOptions  - options to be added to default options of $modal.open()
     * @param  {Object} options       - options used in modal template (modal.html)
     * @param  {String} modalClass    - (optional) class(es) to be applied to the modal
     * @return {Object}               - the instance $modal.open() returns
     */
    function openModal(modalOptions,options, modalClass) {
      var modalScope = $rootScope.$new();
      options = options || {};
      modalClass = modalClass || 'modal-default';

      angular.extend(modalScope, options);

      var defaultModalOptions = {
        templateUrl: 'components/modal/modal.html',
        windowClass: modalClass,
        scope: modalScope,
        controller: ['$scope',function($scope){
          $log.debug($scope);
          $scope.$on('CHANGE_MESSAGE',function(event,newMessage){
            $scope.$parent.modal.text = newMessage;
          })
        }]
      }
      modalOptions = modalOptions || {};

      angular.extend(defaultModalOptions,modalOptions);

      return $modal.open(defaultModalOptions);
    }

    // Public API here
    return {
      /* information modals */
      information: {

        /**
         * Create a function to open a progress modal (ex. ng-click='myModalFn(name, arg1, arg2...)')
         * @return {Function}     - the function to open the modal (ex. myModalFn)
         */
        progress: function(msg) {
          /**
           * Open a delete confirmation modal
           * @param  {String} message   - message to be shown
           */
          return openModal(
              {
                backdrop: 'static'
              },
              {
                modal: {
                  header: false,
                  dismissable: false,
                  progress_circular: true,
                  text: msg
                }
              }, 'modal-default');
        }
      }
    };
  });
