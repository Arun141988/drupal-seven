(function ($) {

  Drupal.gceFront = Drupal.gceFront || {};

  $(document).ready(function () {
    Drupal.gceFront.previousWidth = $(window).width();
    Drupal.gceFront.width = 992;
    setTimeout(function () {
      var path = Drupal.settings.akm_front.slider_callback;
      if (Drupal.gceFront.previousWidth < Drupal.gceFront.width) {
        Drupal.gceFront.replaceSlider(path + 'mobile');
      }
    }, 1000);
  });

  $(window).resize(function () {
    var slug;
    var path = Drupal.settings.akm_front.slider_callback;

    if (slug = Drupal.gceFront.checkViewUpdate()) {
      Drupal.gceFront.replaceSlider(path + slug);
    }
    Drupal.gceFront.previousWidth = $(window).width();
  });

  /**
   * Helper function check is need to replace slider.
   */
  Drupal.gceFront.checkViewUpdate = function () {
    var width = Drupal.gceFront.width;
    var previousWidth = Drupal.gceFront.previousWidth;
    var currentWidth = $(window).width();
    if (currentWidth < width && previousWidth >= width) {
      return 'mobile';
    }
    else {
      if (currentWidth >= width && previousWidth < width) {
        return 'desktop';
      }
    }
    return false;
  }

  /**
   * Helper function replace the slider.
   */
  Drupal.gceFront.replaceSlider = function (path) {
    var selector = Drupal.settings.akm_front.slider_element;
    var element = $(selector);

    var base = element.attr('id');
    var element_settings = {
      url: path,
      event: 'update_slider',
      progress: {
        type: 'throbber'
      }
    };

    Drupal.ajax[base] = new Drupal.ajax(base, element, element_settings);
    Drupal.ajax[base].eventResponse(element, {});
  }
})(jQuery);