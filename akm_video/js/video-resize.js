/*--------------------------------------------------------------*/
/*-- JS Document --*/
/*--------------------------------------------------------------*/
(function ($) {
  Drupal.behaviors.videoFrames = {
    attach: function (context, settings) {
      $(window).resize(function () {
        videoFrames.resize();
      });
      videoFrames.resize();
    }
  }

  var videoFrames = {
    proportion: 3 / 4, // height/width
    elementsSelector: "iframe, embed, object",
    elements: {},
    column: ".video-content",
    width: 0,
    height: 0,
    init: function () {
      videoFrames.elements = $(videoFrames.elementsSelector);
      videoFrames.width = $(videoFrames.elements).first().parent().width();
      videoFrames.height = Math.round(videoFrames.width * videoFrames.proportion);
    },
    resize: function () {
      videoFrames.init();
      $(videoFrames.elements).width(videoFrames.width).height(videoFrames.height);
    }
  }
})(jQuery);

